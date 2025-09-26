// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPathUpdated, const TArray<FVector>&, Path, const TArray<FVector>&, ExploredNodes);

struct FGridNode;
class AGridNodeActorBase;
UCLASS()
class AI_ENGINE_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	AGridManager();


	static bool StaticIsValidPos(int32 X, int32 Y, int32 GridSizeX, int32 GridSizeY);
	static int32 StaticGetIndexFromXY(int32 X, int32 Y, int32 GridSizeX, int32 GridSizeY);


	// Grid Delegates
	UPROPERTY(BlueprintAssignable, Category = "Grid|Events")
	FOnGridChanged OnGridChanged;
	UPROPERTY(BlueprintAssignable, Category = "Grid|Events")
	FOnPathUpdated OnPathUpdated;


	///FIELDS///
	// Grid Fields
	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
	int32 GridSizeX;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
	int32 GridSizeY;
	UPROPERTY(EditAnywhere, Category = "Grid|Settings")
	int32 CellSize;


	// Interaction Fields

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Settings")
	float InteractionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Settings")
	float InteractionRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Settings")
	EGridActorType CurrentPlacementType;

	// Nodes Fields
	UPROPERTY(EditDefaultsOnly, Category = "Grid|Nodes")
	TSubclassOf<AGridNodeActorBase> StartNodeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Grid|Nodes")
	TSubclassOf<AGridNodeActorBase> GoalNodeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Grid|Nodes")
	TSubclassOf<AGridNodeActorBase> WallNodeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Grid|Nodes")
	TSubclassOf<AGridNodeActorBase> PathNodeClass;

	///METHODS///	
	// Grid Methods
	// Implement all this in C++ instaed of BP
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetWorldPositionFromXY(int32 X, int32 Y) const;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetCellFromWorldPosition(const FVector& WorldPosition, int32& OutX, int32& OutY) const;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void DrawDebugCell(int32 X, int32 Y, FColor Color = FColor::Green, float Duration = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetHighlightedCellWorldPosition() const;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	EGridActorType GetNodeTypeAtPosition(const FVector& WorldPosition) const;

	// Nodes Methods
	UFUNCTION(BlueprintCallable, Category = "Node")
	bool ToggleNodeActorInGrid(const FVector& WorldPosition);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	///FIELDS///
	// Grid Fields
	static constexpr  float DEFAULT_CELL_SIZE = 10.f;
	static constexpr  int32  DEFAULT_GRID_SIZE = 10;

	UPROPERTY()
	TArray<FGridNode> Grid;
	FVector GridOrigin;

	int32 LastHighlightedNodeX;
	int32 LastHighlightedNodeY;
	bool bHasLastHighlightedNode;

	/// Node Fields
	UPROPERTY()
	AGridNodeActorBase* StartNode;
	UPROPERTY()
	AGridNodeActorBase* GoalNode;

	TMap<FIntPoint, AGridNodeActorBase*> WallNodes;
	TMap<FIntPoint, AGridNodeActorBase*> PathNodes;

	//PathFinding Fields
	TArray<FVector> CurrentPath;
	TArray<FVector> ExploredNodes;


	///METHODS///
	// Grid Methods
	void Initialize();
	void DrawGrid();
	void ClearDebugLines();

	// Grid Helper Methods
	int32 GetIndexFromXY(int32 X, int32 Y) const;
	bool IsValidPos(int32 X, int32 Y) const;
	FGridNode* GetNodeAt(int32 X, int32 Y);
	bool IsNodeAlreadyHighlighted(int32 X, int32 Y) const;
	void UpdateHighlightedCell(int32 X, int32 Y);
	AGridNodeActorBase* GetNodeActorAtCell(int32 X, int32 Y) const;

	// Nodes Methods
	void RemoveExistingNodeActorAtCell(int32 X, int32 Y);
	AGridNodeActorBase* SpawnNodeActorAtCell(TSubclassOf<AGridNodeActorBase> ActorClass, int32 X, int32 Y);

	void SpawnPathNodes(int32 X, int32 Y, bool bisFinalPath);
	void ClearPathNodes();

	void UpdatePathFinding();
};
