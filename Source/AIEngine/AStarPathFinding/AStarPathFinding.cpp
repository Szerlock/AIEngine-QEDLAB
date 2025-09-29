// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarPathFinding.h"
#include "GridManager.h"


const TArray<TPair<int32, int32>> AStarPathFinding::Directions =
{
	{0, 1}, {-1, 0}, {1, 0}, {0, -1}
};

TArray<FVector> AStarPathFinding::ComputePath(const TArray<FGridNode>& Grid, int32 GridSizeX, int32 GridSizeY, int32 StartX, int32 StartY, int32 GoalX, int32 GoalY, float CellSize, TArray<FVector>& OutExploredNodes)
{
	OutExploredNodes.Empty();

	if (!ValidateInputs(StartX, StartY, GoalX, GoalY, GridSizeX, GridSizeY))
	{
		return TArray<FVector>(); // Invalid path so return empty path
	}

	TArray<FPathNode> PathNodes;
	InitializePathNodes(PathNodes, GridSizeX, GridSizeY);

	TArray<FPathNode*> NodesToExplore;
	SetupStartNode(PathNodes, NodesToExplore, StartX, StartY, GoalX, GoalY, GridSizeX);

	const int32 MaxIterations = GridSizeX * GridSizeY;
	int32 IterationCount = 0;

	while (!NodesToExplore.IsEmpty())
	{
		if (++IterationCount > MaxIterations)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PathFinder : Maximum iterations reached, path not found"));
			return TArray<FVector>(); // return empty path
		}

		FPathNode* CurrentNode = FindNodeWithLowestCost(NodesToExplore);

		if (!CurrentNode->IsExplored)
		{
			const FVector WorldPos(
				(CurrentNode->X + 0.5f) * CellSize,
				(CurrentNode->Y + 0.5f) * CellSize,
				0.0f
			);
			OutExploredNodes.Add(WorldPos);
		}

		if (IsGoalNode(CurrentNode, GoalX, GoalY))
		{
			return ReconstructPath(CurrentNode, CellSize);
		}

		CurrentNode->IsExplored = true;

		for (const auto& Direction : Directions)
		{
			ProcessNeighborNode(Direction, CurrentNode, PathNodes, Grid, GridSizeX, GridSizeY, GoalX, GoalY, NodesToExplore);
		}
	}

	return TArray<FVector>(); // No path
}

bool AStarPathFinding::ValidateInputs(int32 StartX, int32 StartY, int32 GoalX, int32 GoalY, int32 GridSizeX, int32 GridSizeY)
{
	return AGridManager::StaticIsValidPos(StartX, StartY, GridSizeX, GridSizeY)
		&& AGridManager::StaticIsValidPos(GoalX, GoalY, GridSizeX, GridSizeY);
}

void AStarPathFinding::InitializePathNodes(TArray<FPathNode>& PathNodes, int32 GridSizeX, int32 GridSizeY)
{
	PathNodes.SetNum(GridSizeX * GridSizeY);

	for (int32 Y = 0; Y < GridSizeY; Y++)
	{
		for (int32 X = 0; X < GridSizeX; X++)
		{
			FPathNode& Node = PathNodes[AGridManager::StaticGetIndexFromXY(X, Y, GridSizeX)];
			InitializeNode(Node, X, Y);
		}
	}
}


void AStarPathFinding::InitializeNode(FPathNode& Node, int32 X, int32 Y)
{
	Node.X = X;
	Node.Y = Y;
	Node.CostFromStart = INT32_MAX;
	Node.EstimatedCostToGoal = 0;
	Node.PreviousNode = nullptr;
	Node.IsExplored = false;
}

void AStarPathFinding::SetupStartNode(TArray<FPathNode>& PathNodes, TArray<FPathNode*>& NodesToExplore, int32 StartX, int32 StartY, int32 GoalX, int32 GoalY, int32 GridSizeX)
{
	FPathNode& StartNode = PathNodes[AGridManager::StaticGetIndexFromXY(StartX, StartY, GridSizeX)];
	StartNode.CostFromStart = 0;
	StartNode.EstimatedCostToGoal = CalculateDistanceToGoal(StartX, StartY, GoalX, GoalY);
	NodesToExplore.Add(&StartNode);
}


AStarPathFinding::FPathNode* AStarPathFinding::FindNodeWithLowestCost(TArray<FPathNode*>& NodesToExplore)
{

	int32 CurrentIndex = 0;
	int32 LowestCost = MAX_int32;

	for (int32 i = 0; i < NodesToExplore.Num(); i++)
	{
		const int32 NodeCost = NodesToExplore[i]->GetTotalCost();
		if (NodeCost < LowestCost)
		{
			LowestCost = NodeCost;
			CurrentIndex = i;
		}
	}

	FPathNode* Node = NodesToExplore[CurrentIndex];
	NodesToExplore.RemoveAtSwap(CurrentIndex);
	return Node;
}

bool AStarPathFinding::IsGoalNode(const FPathNode* Node, int32 GoalX, int32 GoalY)
{
	return Node->X == GoalX && Node->Y == GoalY;
}

bool AStarPathFinding::ProcessNeighborNode(const TPair<int32, int32>& Direction, FPathNode* CurrentNode, TArray<FPathNode>& PathNodes, const TArray<FGridNode>& Grid, int32 GridSizeX, int32 GridSizeY, int32 GoalX, int32 GoalY, TArray<FPathNode*>& NodesToExplore)
{
	const int32 NeighborX = CurrentNode->X + Direction.Key;
	const int32 NeighborY = CurrentNode->Y + Direction.Value;

	if (!AGridManager::StaticIsValidPos(NeighborX, NeighborY, GridSizeX, GridSizeY) ||
		!IsNodeCrossable(Grid, GridSizeX, NeighborX, NeighborY))
	{
		return false;
	}

	FPathNode& NeighborNode = PathNodes[AGridManager::StaticGetIndexFromXY(NeighborX, NeighborY, GridSizeX)];
	if (NeighborNode.IsExplored)
	{
		return false;
	}

	const int32 MovementCost = STRAIGHT_COST;
	const int32 NewCostFromStart = CurrentNode->CostFromStart + MovementCost;

	if(NeighborNode.CostFromStart == MAX_int32 || NewCostFromStart < NeighborNode.CostFromStart)
	{
		UpdateNeighborNode(NeighborNode, CurrentNode, NewCostFromStart, GoalX, GoalY, NodesToExplore);
		return true;
	}

	return false;
}

void AStarPathFinding::UpdateNeighborNode(FPathNode& NeighborNode, FPathNode* CurrentNode, int32 NewCostFromStart, int32 GoalX, int32 GoalY, TArray<FPathNode*>& NodesToExplore)
{
	NeighborNode.CostFromStart = NewCostFromStart;
	NeighborNode.EstimatedCostToGoal = CalculateDistanceToGoal(NeighborNode.X, NeighborNode.Y, GoalX, GoalY);
	NeighborNode.PreviousNode = CurrentNode;
	
	bool bIsInList = false;
	for (FPathNode* Node : NodesToExplore)
	{
		if (Node == &NeighborNode)
		{
			bIsInList = true;
			break;
		}
	}

	if(!bIsInList)
	{
		NodesToExplore.Add(&NeighborNode);
	}
}

int32 AStarPathFinding::CalculateDistanceToGoal(int32 FromX, int32 FromY, int32 GoalX, int32 GoalY)
{
	// Manhattan distance
	const int32 DeltaX = FMath::Abs(FromX - GoalX);
	const int32 DeltaY = FMath::Abs(FromY - GoalY);
	return STRAIGHT_COST * (DeltaX + DeltaY);
}

bool AStarPathFinding::IsNodeCrossable(const TArray<FGridNode>& Grid, int32 GridSizeX, int32 X, int32 Y)
{
	if (!AGridManager::StaticGetIndexFromXY(X, Y, GridSizeX))
	{
		return false;
	}

	const int32 Index = AGridManager::StaticGetIndexFromXY(X, Y, GridSizeX);	
	return Grid[Index].IsCrossable;
}

TArray<FVector> AStarPathFinding::ReconstructPath(FPathNode* EndNode, float CellSize)
{
	TArray<FVector> Path;
	FPathNode* CurrentNode = EndNode;

	while (CurrentNode != nullptr)
	{
		const FVector WorldPos(
			(CurrentNode->X + 0.5f) * CellSize,
			(CurrentNode->Y + 0.5f) * CellSize,
			0.0f
		);

		Path.Insert(WorldPos, 0);
		CurrentNode = CurrentNode->PreviousNode;
	}

	return Path;
}

