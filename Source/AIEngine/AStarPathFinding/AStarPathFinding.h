// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <mutex>
#include <unordered_set>
#include "AStarPathfinding/GridNode.h"

/**
 *
 */

struct FClosedListPartition
{
    std::mutex Lock;                     
    std::unordered_set<int> ExploredNodes; // Stores explored node indices
};

class AIENGINE_API AStarPathFinding
{

public:
    static constexpr int32 STRAIGHT_COST = 10;
    static constexpr int32 DIAGONAL_COST = 14;
    static const int NUM_PARTITIONS = 16; 

    static TArray<FClosedListPartition> ClosedListPartitions;
    static std::vector<std::thread> Threads;


    static int32 HashNode(int x, int y, int GridSizeX);

    static void SetPartitioning(bool bUsePartition)
    {
        bPartition = bUsePartition;
	}
    
    struct FPathNode
    {
        int32 X;
        int32 Y;
        int32 CostFromStart;
        int32 EstimatedCostToGoal;
        FPathNode* PreviousNode;
        bool IsExplored;

        int32 GetTotalCost() const
        {
            return CostFromStart + EstimatedCostToGoal;
        }
    };

    static TArray<FVector> ComputePath(
        const TArray<FGridNode>& Grid,
        int32 GridSizeX,
        int32 GridSizeY,
        int32 StartX,
        int32 StartY,
        int32 GoalX,
        int32 GoalY,
        float CellSize,
        TArray<FVector>& OutExploredNodes,
        int32 NumThreads
    );

private:

    inline static bool bPartition = false;

    static const TArray<TPair<int32, int32>> Directions;

    // Init Methods

    static bool ValidateInputs(int32 StartX, int32 StartY, int32 GoalX, int32 GoalY, int32 GridSizeX, int32 GridSizeY);

    static void InitializePathNodes(TArray<FPathNode>& PathNodes, int32 GridSizeX, int32 GridSizeY);

    static void InitializeNode(FPathNode& Node, int32 X, int32 Y);

    static void SetupStartNode(
        TArray<FPathNode>& PathNodes,
        TArray<FPathNode*>& NodesToExplore,
        int32 StartX,
        int32 StartY,
        int32 GoalX,
        int32 GoalY,
        int32 GridSizeX
    );

    // Pathfinding Methods
    static FPathNode* FindNodeWithLowestCost(TArray<FPathNode*>& NodesToExplore);

    static bool IsGoalNode(const FPathNode* Node, int32 GoalX, int32 GoalY);

    static bool ProcessNeighborNode(
        const TPair<int32, int32>& Direction,
        FPathNode* CurrentNode,
        TArray<FPathNode>& PathNodes,
        const TArray<FGridNode>& Grid,
        int32 GridSizeX,
        int32 GridSizeY,
        int32 GoalX,
        int32 GoalY,
        TArray<FPathNode*>& NodesToExplore,
		std::mutex& NodesMutex
    );

    static bool ProcessNeighborNode(
        const TPair<int32, int32>& Direction,
        FPathNode* CurrentNode,
        TArray<FPathNode>& PathNodes,
        const TArray<FGridNode>& Grid,
        int32 GridSizeX,
        int32 GridSizeY,
        int32 GoalX,
        int32 GoalY,
        TArray<FPathNode*>& NodesToExplore
    );

    static void UpdateNeighborNode(
        FPathNode& NeighborNode,
        FPathNode* CurrentNode,
        int32 NewCostFromStart,
        int32 GoalX,
        int32 GoalY,
        TArray<FPathNode*>& NodesToExplore
    );



    static int32 CalculateDistanceToGoal(int32 FromX, int32 FromY, int32 GoalX, int32 GoalY);

    static bool IsNodeCrossable(const TArray<FGridNode>& Grid, int32 GridSizeX, int32 X, int32 Y);

    static TArray<FVector> ReconstructPath(FPathNode* EndNode, float CellSize);

    static TArray<FVector> ComputePath_Sequential(
        const TArray<FGridNode>& Grid, int32 GridSizeX, int32 GridSizeY,
        int32 StartX, int32 StartY, int32 GoalX, int32 GoalY,
        float CellSize, TArray<FVector>& OutExploredNodes);

	static TArray<FVector> ComputePath_Partitioned(
        const TArray<FGridNode>& Grid, int32 GridSizeX, int32 GridSizeY,
        int32 StartX, int32 StartY, int32 GoalX, int32 GoalY,
		float CellSize, TArray<FVector>& OutExploredNodes, int32 NumThreads);
};
