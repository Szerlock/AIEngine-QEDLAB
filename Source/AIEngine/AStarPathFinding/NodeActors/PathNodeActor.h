// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNodeActorBase.h"
#include "PathNodeActor.generated.h"

UCLASS()
class AIENGINE_API APathNodeActor : public AGridNodeActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathNodeActor();

	void SetPathNodeType(bool bIsFinalPath);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
