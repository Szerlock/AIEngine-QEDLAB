// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIEngineGameMode.h"
#include "AIEnginePlayerController.h"
#include "GridManager.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "AIEngineCharacter.h"

AAIEngineGameMode::AAIEngineGameMode()
{

}

void AAIEngineGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitializeGrid();

	AAIEngineCharacter* Player = Cast<AAIEngineCharacter>(
		UGameplayStatics::GetPlayerCharacter(this, 0)
	);

	if (Player)
	{
		Player->SetGridManager(GridManager);
	}
}


void AAIEngineGameMode::OnGridStateChanged()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Grid state changed"));
}
void AAIEngineGameMode::OnPathReCalculated(const TArray<FVector>& Path, const TArray<FVector>& ExploredNodes)
{
	FString Message = FString::Printf(TEXT("New path calculated with %d points, %d nodes explored"), Path.Num(), ExploredNodes.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Message);
}

void AAIEngineGameMode::InitializeGrid()
{
	TArray<AActor*> FoundGridManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridManager::StaticClass(), FoundGridManagers);

	if (FoundGridManagers.Num() > 0)
	{
		GridManager = Cast<AGridManager>(FoundGridManagers[0]);

		if (FoundGridManagers.Num() > 1)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
				FString::Printf(TEXT("Multiple GridManagers found. Keeping first one, destroying %d others."),
					FoundGridManagers.Num() - 1));

			for (int32 i = 1; i < FoundGridManagers.Num(); i++)
			{
				if (FoundGridManagers[i])
				{
					FoundGridManagers[i]->Destroy();
				}
			}
		}
	}
	else
	{
		if (!GridManagerClass)
		{
			return;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GridManager = GetWorld()->SpawnActor<AGridManager>(GridManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}

	if (GridManager)
	{
		GridManager->OnGridChanged.AddDynamic(this, &AAIEngineGameMode::OnGridStateChanged);
		GridManager->OnPathUpdated.AddDynamic(this, &AAIEngineGameMode::OnPathReCalculated);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Grid Manager initialized successfully"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to initialize Grid Manager"));
	}
}
