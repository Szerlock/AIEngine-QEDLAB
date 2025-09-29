// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridNodeActorBase.h"
#include "StartNodeActor.generated.h"

UCLASS()
class AIENGINE_API AStartNodeActor : public AGridNodeActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStartNodeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
