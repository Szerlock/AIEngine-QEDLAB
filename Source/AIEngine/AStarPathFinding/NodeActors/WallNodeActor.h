// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNodeActorBase.h"
#include "WallNodeActor.generated.h"

UCLASS()
class AIENGINE_API AWallNodeActor : public AGridNodeActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallNodeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
