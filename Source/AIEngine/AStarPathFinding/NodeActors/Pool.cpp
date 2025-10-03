// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarPathFinding/NodeActors/Pool.h"

// Sets default values
APool::APool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APool::BeginPlay()
{
	Super::BeginPlay();
	
}

void APool::InitializePool()
{
	for (int32 i = 0; i < PoolSize; i++)
	{
		AGridNodeActorBase* WallNode = GetWorld()->SpawnActor<AGridNodeActorBase>(WallNodeClass);
		AGridNodeActorBase* PathNode = GetWorld()->SpawnActor<AGridNodeActorBase>(PathNodeClass);
		WallPool.Enqueue(WallNode);
		PathPool.Enqueue(PathNode);
	}

	StartNode = GetWorld()->SpawnActor<AGridNodeActorBase>(StartNodeClass);
	GoalNode = GetWorld()->SpawnActor<AGridNodeActorBase>(GoalNodeClass);

}

void APool::DoublePoolSize()
{
	int32 CurrentSize = IncrementPoolSize;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Doubling Pool Size from %d to %d"), CurrentSize, CurrentSize * 2));
	for (int32 i = 0; i < CurrentSize; i++)
	{
		AGridNodeActorBase* WallNode = GetWorld()->SpawnActor<AGridNodeActorBase>(WallNodeClass);
		AGridNodeActorBase* PathNode = GetWorld()->SpawnActor<AGridNodeActorBase>(PathNodeClass);
		WallPool.Enqueue(WallNode);
		PathPool.Enqueue(PathNode);
	}
}

void APool::GetWallNodeFromPool(AGridNodeActorBase*& OutNode)
{
	if (!WallPool.Dequeue(OutNode))
	{
		DoublePoolSize();
		WallPool.Dequeue(OutNode);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Wall Pool Size Decreased")));
}

void APool::GetPathNodeFromPool(AGridNodeActorBase*& OutNode)
{
	if (!PathPool.Dequeue(OutNode))
	{
		DoublePoolSize();
		PathPool.Dequeue(OutNode);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Path Pool Size Decreased")));
}

void APool::GetNodeFromPool(EGridActorType NodeType, AGridNodeActorBase*& OutNode)
{
	switch (NodeType)
	{
	case EGridActorType::Start:
		OutNode = StartNode;
		break;
	case EGridActorType::Goal:
		OutNode = GoalNode;
		break;
	case EGridActorType::Wall:
		GetWallNodeFromPool(OutNode);
		break;
	case EGridActorType::Path:
		GetPathNodeFromPool(OutNode);
		break;
	default:
		OutNode = nullptr;
	}
}




