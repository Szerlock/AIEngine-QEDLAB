// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AStarPathFinding/GridNodeActorBase.h"
#include "GoalNodeActor.generated.h"

/**
 * 
 */
UCLASS()
class AIENGINE_API AGoalNodeActor : public AGridNodeActorBase
{
	GENERATED_BODY()
public:
	//////// CONSTRUCTOR ////////
	AGoalNodeActor();

protected:
	//////// UNREAL LIFECYCLE ////////
	virtual void BeginPlay() override;
};
