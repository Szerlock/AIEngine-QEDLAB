// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarPathFinding.h"


const TArray<TPair<int32, int32>> AStarPathFinding::Directions =
{
	{0, 1}, {-1, 0}, {1, 0}, {0, -1}
};

TArray<FVector> AStarPathFinding::ComputePath(const FGridNode& Grid, int32 GridSizeX, int32 GridSizeY, int32 StartX, int32 StartY, int32 GoalX, int32 GoalY, float CellSize, TArray<FVector>& OutExploredNodes)
{
	OutExploredNodes.Empty();

	if (ValidateInputs(StartX, StartY, GoalX, GoalY, GridSizeX, GridSizeY))
	{
		return TArray<FVector>(); // Invalid path so return empty path
	}

	TArray<FPathNode> PathNodes;
	InitializePathNodes(PathNodes, GridSizeX, GridSizeY);

	TArray<FPathNode*> NodesToExplore;
	SetupStartNode(PathNodes, NodesToExplore, StartX, StartY, GoalX, GoalY);

	const int32 MaxIterations = GridSizeX * GridSizeY;
	int32 IterationCount = 0;

	while (!NodesToExplore.IsEmpty())
	{
		if (IterationCount >= MaxIterations)
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

		}
	}

}

bool AStarPathFinding::ValidateInputs(int32 StartX, int32 StartY, int32 GoalX, int32 GoalY, int32 GridSizeX, int32 GridSizeY)
{
	return false;
}

void AStarPathFinding::InitializePathNodes(TArray<FPathNode>& PathNodes, int32 GridSizeX, int32 GridSizeY)
{
}

void AStarPathFinding::InitializeNode(FPathNode& Node, int32 X, int32 Y)
{
}

void AStarPathFinding::SetupStartNode(TArray<FPathNode>& PathNodes, TArray<FPathNode*>& NodesToExplore, int32 StartX, int32 StartY, int32 GoalX, int32 GoalY)
{
}

AStarPathFinding::FPathNode* AStarPathFinding::FindNodeWithLowestCost(TArray<FPathNode*>& NodesToExplore)
{
	return nullptr;
}

bool AStarPathFinding::IsGoalNode(const FPathNode* Node, int32 GoalX, int32 GoalY)
{
	return false;
}

bool AStarPathFinding::ProcessNeighborNode(const TPair<int32, int32>& Direction, FPathNode* CurrentNode, TArray<FPathNode>& PathNodes, const TArray<FGridNode*>& Grid, int32 GridSizeX, int32 GridSizeY, int32 GoalX, int32 GoalY, TArray<FPathNode*>& NodesToExplore)
{
	return false;
}

void AStarPathFinding::UpdateNeighborNode(FPathNode* NeighborNode, FPathNode* CurrentNode, int32 NewCostFromStart, int32 GoalX, int32 GoalY, TArray<FPathNode*>& NodesToExplore)
{
}

int32 AStarPathFinding::CalculateDistanceToGoal(int32 FromX, int32 FromY, int32 GoalX, int32 GoalY)
{
	return int32();
}

void AStarPathFinding::IsNodeCrossable(const TArray<FGridNode>& Grid, int32 GridSizeX, int32 GridSizeY, int32 X, int32 Y)
{
}

TArray<FVector> AStarPathFinding::ReconstructPath(FPathNode* GoalNode, float CellSize)
{
	return TArray<FVector>();
}

