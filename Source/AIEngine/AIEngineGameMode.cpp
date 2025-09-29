// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIEngineGameMode.h"
#include "AIEnginePlayerController.h"
#include "GridManager.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/SpectatorPawn.h"

AAIEngineGameMode::AAIEngineGameMode()
{

}

void AAIEngineGameMode::OnGridStateChanged()
{
}

void AAIEngineGameMode::OnPathReCalculated(const TArray<FVector>& Path, const TArray<FVector>& ExploredNodes)
{
}

void AAIEngineGameMode::InitializeGrid()
{
}
