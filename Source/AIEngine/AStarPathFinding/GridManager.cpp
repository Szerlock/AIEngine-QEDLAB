// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "GridNode.h"

AGridManager::AGridManager()
{

}

void AGridManager::BeginPlay()
{
}

void AGridManager::Tick(float DeltaTime)
{
}


bool AGridManager::StaticIsValidPos(int32 X, int32 Y, int32 GridSizeX, int32 GridSizeY)
{
	return false;
}

int32 AGridManager::StaticGetIndexFromXY(int32 X, int32 Y, int32 GridSizeX, int32 GridSizeY)
{
	return int32();
}

FVector AGridManager::GetWorldPositionFromXY(int32 X, int32 Y) const
{
	return FVector();
}

bool AGridManager::GetCellFromWorldPosition(const FVector& WorldPosition, int32& OutX, int32& OutY) const
{
	return false;
}

void AGridManager::DrawDebugCell(int32 X, int32 Y, FColor Color, float Duration)
{
}

FVector AGridManager::GetHighlightedCellWorldPosition() const
{
	return FVector();
}

EGridActorType AGridManager::GetNodeTypeAtPosition(const FVector& WorldPosition) const
{
	return EGridActorType();
}

bool AGridManager::ToggleNodeActorInGrid(const FVector& WorldPosition)
{
	return false;
}


void AGridManager::Initialize()
{

}

void AGridManager::DrawGrid()
{
}

void AGridManager::ClearDebugLines()
{
}

int32 AGridManager::GetIndexFromXY(int32 X, int32 Y) const
{
	return int32();
}

bool AGridManager::IsValidPos(int32 X, int32 Y) const
{
	return false;
}

FGridNode* AGridManager::GetNodeAt(int32 X, int32 Y)
{
	return nullptr;
}

bool AGridManager::IsNodeAlreadyHighlighted(int32 X, int32 Y) const
{
	return false;
}

void AGridManager::UpdateHighlightedCell(int32 X, int32 Y)
{
}

AGridNodeActorBase* AGridManager::GetNodeActorAtCell(int32 X, int32 Y) const
{
	return nullptr;
}

void AGridManager::RemoveExistingNodeActorAtCell(int32 X, int32 Y)
{
}

AGridNodeActorBase* AGridManager::SpawnNodeActorAtCell(TSubclassOf<AGridNodeActorBase> ActorClass, int32 X, int32 Y)
{
	return nullptr;
}

void AGridManager::SpawnPathNodes(int32 X, int32 Y, bool bisFinalPath)
{
}

void AGridManager::ClearPathNodes()
{
}

void AGridManager::UpdatePathFinding()
{
}

