// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "DrawDebugHelpers.h"
#include "NodeActors/PathNodeActor.h"
#include "NodeActors/NodePooling.h"
#include "Kismet/GameplayStatics.h"
#include "AStarPathFinding.h"

AGridManager::AGridManager()
	: GridSizeX(DEFAULT_GRID_SIZE)
	, GridSizeY(DEFAULT_GRID_SIZE)
	, CellSize(DEFAULT_CELL_SIZE)
	, InteractionDistance(0), InteractionRate(0), CurrentPlacementType(), LastHighlightedNodeX(-1)
	, LastHighlightedNodeY(-1)
	, bHasHighlightedNode(false), StartNode(nullptr), GoalNode(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AGridManager::StaticIsValidPos(int32 X, int32 Y, int32 GridSizeX, int32 GridSizeY)
{
	return static_cast<uint32>(X) < static_cast<uint32>(GridSizeX) && static_cast<uint32>(Y) < static_cast<uint32>(GridSizeY);
}

int32 AGridManager::StaticGetIndexFromXY(int32 X, int32 Y, int32 GridSizeX)
{
	return Y * GridSizeX + X;
}

FVector AGridManager::GetWorldPositionFromCell(int32 X, int32 Y) const
{
	return GridOrigin + FVector((X + 0.5f) * CellSize,(Y + 0.5f) * CellSize, 0.0f);
}

void AGridManager::DrawDebugCell(int32 X, int32 Y, FColor Color, float Duration)
{
	if (!IsValidPos(X, Y) || !GetWorld())
	{
		return;
	}

	FVector BoxPos = GetWorldPositionFromCell(X, Y);
	BoxPos += FVector(0.0f + .5f, 0.0f + .5f, CellSize * 0.5f);
	FVector BoxExtent = FVector(CellSize * 0.45f, CellSize * 0.45f, CellSize * .5f);

	DrawDebugBox(GetWorld(), BoxPos, BoxExtent, Color, false, Duration);
	
	UpdateHighlightedCell(X, Y);
}

FVector AGridManager::GetHighlightedCellWorldPosition() const
{
	return GetWorldPositionFromCell(LastHighlightedNodeX, LastHighlightedNodeY);
}

EGridActorType AGridManager::GetNodeTypeAtPosition(const FVector& WorldPosition) const
{
	int32 GridX, GridY;

	if(!GetCellFromWorldPosition(WorldPosition, GridX, GridY))
	{
		return EGridActorType::None;
	}

	if (StartNode && StartNode->GridX == GridX && StartNode->GridY == GridY)
	{
		return EGridActorType::Start;
	}

	if (GoalNode && GoalNode->GridX == GridX && GoalNode->GridY == GridY)
	{
		return EGridActorType::Goal;
	}

	if (WallNodes.Contains(FIntPoint(GridX, GridY)))
	{
		return EGridActorType::Wall;
	}

	return EGridActorType::None;
}

bool AGridManager::GetCellFromWorldPosition(const FVector& WorldPosition, int32& OutX, int32& OutY) const
{
	FVector relativePos = WorldPosition - GridOrigin;
	OutX = FMath::FloorToInt(relativePos.X / CellSize);
	OutY = FMath::FloorToInt(relativePos.Y / CellSize);
	return IsValidPos(OutX, OutY);
}

void AGridManager::PlaceNode()
{
	ToggleNodeActorInGrid(GetHighlightedCellWorldPosition());
}

bool AGridManager::ToggleNodeActorInGrid(const FVector& WorldPosition)
{
	int32 GridX, GridY;
	if (!GetCellFromWorldPosition(WorldPosition, GridX, GridY))
	{
		return false;
	}

	FIntPoint Point(GridX, GridY);

	EGridActorType ExistingType = GetNodeTypeAtPosition(WorldPosition);
	if (ExistingType == CurrentPlacementType)
	{
		RemoveExistingNodeActorAtCell(GridX, GridY);
		OnGridChanged.Broadcast();
		UpdatePathFinding();
		return true;
	}

	RemoveExistingNodeActorAtCell(GridX, GridY);
	GetNode(GridX, GridY).IsCrossable = true;

	//TSubclassOf<AGridNodeActorBase> ClassToSpawn = nullptr;
	AGridNodeActorBase* NewActor = nullptr;
	switch (CurrentPlacementType)
	{
		case EGridActorType::Start:
			//if (StartNode) StartNode->Destroy();
			//ClassToSpawn = StartNodeClass;
			NodePool->GetNodeFromPool(EGridActorType::Start, NewActor);
			break;
		case EGridActorType::Goal:
			//if (GoalNode) GoalNode->Destroy();
			//ClassToSpawn = GoalNodeClass;
			NodePool->GetNodeFromPool(EGridActorType::Goal, NewActor);
			break;
		case EGridActorType::Wall:
			//ClassToSpawn = WallNodeClass;
			NodePool->GetNodeFromPool(EGridActorType::Wall, NewActor);
			break;
		default:
			break;
	}
	
	//AGridNodeActorBase* NewActor = SpawnNodeActorAtCell(ClassToSpawn, GridX, GridY);
	if (!NewActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pooling Failed"));
		return false;
	}
	MoveNodeToPostition(NewActor, GridX, GridY);


	if (!NewActor) return false;

	switch (CurrentPlacementType)
	{
		case EGridActorType::Start:
			StartNode = NewActor;
			break;
		case EGridActorType::Goal:
			GoalNode = NewActor;
			break;
		case EGridActorType::Wall:
			WallNodes.Add(Point, NewActor);
			GetNode(GridX, GridY).IsCrossable = false;
			break;
		default:
			break;
	}

	OnGridChanged.Broadcast();
	if (bUseAsyncPathFinding)
	{
		UpdatePathFindingAsync();
	}
	else
	{
		UpdatePathFinding();
	}
	return true;
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	GridOrigin = GetActorLocation();
	GridOrigin = FVector::ZeroVector;	
	NodePool = Cast<ANodePooling>(UGameplayStatics::GetActorOfClass(GetWorld(), ANodePooling::StaticClass()));
	Initialize();
	DrawGrid();
}

void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AGridManager::GetIndexFromXY(int32 X, int32 Y) const
{
	return StaticGetIndexFromXY(X, Y, GridSizeX);
}

bool AGridManager::IsValidPos(int32 X, int32 Y) const
{
	return StaticIsValidPos(X, Y, GridSizeX, GridSizeY);
}

FGridNode& AGridManager::GetNode(int32 X, int32 Y)
{
	return Grid[GetIndexFromXY(X, Y)];
}

void AGridManager::Initialize()
{
	GridOrigin = FVector::ZeroVector;

	const int32 GridSize = GridSizeX * GridSizeY;
	Grid.Empty(GridSize);
	Grid.SetNum(GridSize);

	for (int32 y = 0; y < GridSizeY; y++)
	{
		const float YPos = GridOrigin.Y + (y * CellSize);

		for (int32 x = 0; x < GridSizeX; x++)
		{
			FGridNode& Node = GetNode(x, y);
			Node.WorldPosition = FVector(
				GridOrigin.X + (x * CellSize),
				YPos,
				GridOrigin.Z
			);
			Node.IsCrossable = true;
		}
	}
}


void AGridManager::ClearDebugLines()
{
	if (GetWorld())
	{
		FlushPersistentDebugLines(GetWorld());
	}
}


bool AGridManager::IsNodeAlreadyHighlighted(int32 X, int32 Y) const
{
	return bHasHighlightedNode && LastHighlightedNodeX == X && LastHighlightedNodeY == Y;
}

void AGridManager::UpdateHighlightedCell(int32 X, int32 Y)
{
	LastHighlightedNodeX = X;
	LastHighlightedNodeY = Y;
	bHasHighlightedNode = true;
}

AGridNodeActorBase* AGridManager::GetNodeActorAtCell(int32 X, int32 Y) const
{
	if (StartNode && StartNode->GridX == X && StartNode->GridY == Y)
	{
		return StartNode;
	}
	
	if (GoalNode && GoalNode->GridX == X && GoalNode->GridY == Y)
	{
		return GoalNode;
	}

	return WallNodes.FindRef(FIntPoint(X, Y));
}

void AGridManager::RemoveExistingNodeActorAtCell(int32 X, int32 Y)
{
	if (StartNode && StartNode->GridX == X && StartNode->GridY == Y)
	{
		//StartNode->Destroy();
		StartNode = nullptr;
		return;
	}
	if (GoalNode && GoalNode->GridX == X && GoalNode->GridY == Y)
	{
		//GoalNode->Destroy();
		GoalNode = nullptr;
		return;
	}
	FIntPoint Point(X, Y);
	if (AGridNodeActorBase* ExistingWall = WallNodes.FindRef(Point))
	{
		//ExistingWall->Destroy();
		NodePool->ReturnWallNode(ExistingWall);
		WallNodes.Remove(Point);
		GetNode(X, Y).IsCrossable = true;
	}
}

void AGridManager::MoveNodeToPostition(AGridNodeActorBase* NodeActor, int32 X, int32 Y)
{
	FVector NewLocation = GetWorldPositionFromCell(X, Y);
	FTransform SpawnTransform(FRotator::ZeroRotator, NewLocation);
	NodeActor->SetActorLocation(NewLocation);
	NodeActor->GridX = X;
	NodeActor->GridY = Y;
}

AGridNodeActorBase* AGridManager::SpawnNodeActorAtCell(TSubclassOf<AGridNodeActorBase> ActorClass, int32 X, int32 Y)
{
	if(!ActorClass || !GetWorld())
	{
		return nullptr;
	}
	
	FVector Location = GetWorldPositionFromCell(X, Y);
	FTransform SpawnTransform(FRotator::ZeroRotator, Location);

	AGridNodeActorBase* NewActor = GetWorld()->SpawnActor<AGridNodeActorBase>(ActorClass, SpawnTransform);
	if (NewActor)
	{
		NewActor->GridX = X;
		NewActor->GridY = Y;
	}

	return NewActor;
}

void AGridManager::SpawnPathNode(int32 X, int32 Y, bool bisFinalPath)
{
	if (GetNodeActorAtCell(X, Y) != nullptr)
	{
		return;
	}

	FIntPoint Point(X, Y);

	if (APathNodeActor* ExistingNode = Cast<APathNodeActor>(PathNodes.FindRef(Point)))
	{
		ExistingNode->SetPathNodeType(bisFinalPath);
		return;
	}

	FVector Location = GetWorldPositionFromCell(X, Y);
	FTransform SpawnTransform(FRotator::ZeroRotator, Location);

	AGridNodeActorBase* Node;
	NodePool->GetNodeFromPool(EGridActorType::Path, Node);
	if (!Node)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PathNodePoolingFailed"));
		return;
	}
	MoveNodeToPostition(Node, X, Y);

	if (Node)
	{
		if (APathNodeActor* NewNode = Cast<APathNodeActor>(Node))
		{
			NewNode->GridX = X;
			NewNode->GridY = Y;
			NewNode->SetPathNodeType(bisFinalPath);
			PathNodes.Add(Point, NewNode);
		}
	}
}

void AGridManager::ClearPathNodes()
{
	for (auto& Pair : PathNodes)
	{
		if (Pair.Value)
		{
			//Pair.Value->Destroy();
			NodePool->ReturnPathNode(Pair.Value);
		}
	}
	PathNodes.Empty();
}

void AGridManager::UpdatePathFinding()
{

	ClearPathNodes();
	CurrentPath.Empty();
	ExploredNodes.Empty();

	if (!StartNode || !GoalNode)
	{
		return;
	}

	AStarPathFinding::SetPartitioning(bParrallelAStar);

	double StartTime = FPlatformTime::Seconds();

	TArray<FVector> ExploredPositions;
	CurrentPath = AStarPathFinding::ComputePath(
		Grid,
		GridSizeX,
		GridSizeY,
		StartNode->GridX,
		StartNode->GridY,
		GoalNode->GridX,
		GoalNode->GridY,
		CellSize,
		ExploredNodes,
		NumThreads
	);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Pathfinding took %.6f seconds"), FPlatformTime::Seconds() - StartTime));


	for (const auto& ExploredPos : ExploredNodes)
	{
		int32 X, Y;
		if (GetCellFromWorldPosition(ExploredPos, X, Y))
		{
			SpawnPathNode(X, Y, false);
		}
	}

	for (const auto& PathPos : CurrentPath)
	{
		int32 X, Y;
		if (GetCellFromWorldPosition(PathPos, X, Y))
		{
			SpawnPathNode(X, Y, true);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Total Normal Pathfinding took %.6f seconds"), FPlatformTime::Seconds() - StartTime));

	OnPathUpdated.Broadcast(CurrentPath, ExploredNodes);
}

void AGridManager::UpdatePathFindingAsync()
{
	if (!StartNode || !GoalNode)
	{
		return;
	}


	TWeakObjectPtr<AGridManager> WeakThis(this);

	TArray<FGridNode>* GridPtr = &Grid;
	int32 GridXCopy = GridSizeX;
	int32 GridYCopy = GridSizeY;
	int32 StartXCopy = StartNode->GridX;
	int32 StartYCopy = StartNode->GridY;
	int32 GoalXCopy = GoalNode->GridX;
	int32 GoalYCopy = GoalNode->GridY;
	float CellSizeCopy = CellSize;
	int32 NumThreadsCopy = NumThreads;

	double StartTimeTotal = FPlatformTime::Seconds();

	Async(EAsyncExecution::Thread, [GridPtr, GridXCopy, GridYCopy, StartXCopy, StartYCopy, GoalXCopy, GoalYCopy, CellSizeCopy, WeakThis, StartTimeTotal, NumThreadsCopy]()
	{
		double StartTimeThread = FPlatformTime::Seconds();
		TArray<FGridNode> LocalGridCopy = *GridPtr;

		TArray<FVector> AsyncExploredNodes;
		TArray<FVector> AsyncPath = AStarPathFinding::ComputePath(
			LocalGridCopy,
			GridXCopy,
			GridYCopy,
			StartXCopy,
			StartYCopy,
			GoalXCopy,
			GoalYCopy,
			CellSizeCopy,
			AsyncExploredNodes,
			NumThreadsCopy
		);

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Async Pathfinding computation took %.6f seconds"), FPlatformTime::Seconds() - StartTimeThread));

		Async(EAsyncExecution::TaskGraphMainThread, [AsyncPath, AsyncExploredNodes, WeakThis, StartTimeTotal]()
			{
				double MergeTime = FPlatformTime::Seconds();
				if (AGridManager* GridManager = WeakThis.Get())
				{
					GridManager->ClearPathNodes();
					GridManager->CurrentPath = AsyncPath;
					GridManager->ExploredNodes = AsyncExploredNodes;

					for (const auto& ExploredPos : AsyncExploredNodes)
					{
						int32 X, Y;
						if (GridManager->GetCellFromWorldPosition(ExploredPos, X, Y))
						{
							GridManager->SpawnPathNode(X, Y, false);
						}
					}

					for (const auto& PathPos : AsyncPath)
					{
						int32 X, Y;
						if (GridManager->GetCellFromWorldPosition(PathPos, X, Y))
						{
							GridManager->SpawnPathNode(X, Y, true);
						}
					}

					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Async Pathfinding total update took %.6f seconds"), MergeTime - StartTimeTotal));
					GridManager->OnPathUpdated.Broadcast(GridManager->CurrentPath, GridManager->ExploredNodes);
				}
			});
	});
}

void AGridManager::DrawGrid()
{
	if (!GetWorld()) return;

	ClearDebugLines();

	const FColor LineColor = FColor::Red;
	const float LineThickness = 3.0f;
	static const float LifeTime = -1.0f;
	static const bool bPersistent = true;

	for (int32 y = 0; y <= GridSizeY; y++)
	{
		const FVector StartPoint = GridOrigin + FVector(0.0f, y * CellSize, 0.0f);
		const FVector EndPoint = StartPoint + FVector(GridSizeX * CellSize, 0.0f, 0.0f);
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, LineColor, bPersistent, LifeTime, 0, LineThickness);
	}

	for (int32 x = 0; x <= GridSizeX; x++)
	{
		const FVector StartPoint = GridOrigin + FVector(x * CellSize, 0.0f, 0.0f);
		const FVector EndPoint = StartPoint + FVector(0.0f, GridSizeY * CellSize, 0.0f);
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, LineColor, bPersistent, LifeTime, 0, LineThickness);
	}
}




