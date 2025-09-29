// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridNodeActorBase.generated.h"


UENUM(BlueprintType)
enum class EGridActorType : uint8
{
	Start   UMETA(DisplayName = "Start Point"),
	Goal    UMETA(DisplayName = "Goal Point"),
	Wall    UMETA(DisplayName = "Wall"),
	Path	UMETA(DisplayName = "Path"),
	None    UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ENodeState : uint8
{
	Default     UMETA(DisplayName = "Default"),
	ToExplore   UMETA(DisplayName = "To Explore"),
	Explored    UMETA(DisplayName = "Explored"),
	Path        UMETA(DisplayName = "Path"),
	Neighbor    UMETA(DisplayName = "Neighbor")
};

UCLASS()
class AIENGINE_API AGridNodeActorBase : public AActor
{
	GENERATED_BODY()
	
public:
	AGridNodeActorBase();


	////FIELDS////
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	// Grid Fields
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	EGridActorType NodeType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	ENodeState NodeState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	int32 GridX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	int32 GridY;

	////METHODS////
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetupNodeColor(EGridActorType Type, ENodeState State = ENodeState::Default);

	void UpdatePathFindingNodeColor(ENodeState State);

private:
	UPROPERTY()
	bool bIsBaseColorSet = false;

	void LockBaseColor() { bIsBaseColorSet = true; }

	bool IsBaseColorSet() const { return bIsBaseColorSet; }
};
