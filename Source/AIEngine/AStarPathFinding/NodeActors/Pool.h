// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridNodeActorBase.h"
#include "Pool.generated.h"

UCLASS()
class AIENGINE_API APool : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
	TSubclassOf<AGridNodeActorBase> StartNodeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
	TSubclassOf<AGridNodeActorBase> GoalNodeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
	TSubclassOf<AGridNodeActorBase> WallNodeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
	TSubclassOf<AGridNodeActorBase> PathNodeClass;

	UPROPERTY(EditAnywhere)
	int32 PoolSize;

	UPROPERTY(EditAnywhere)
	int32 IncrementPoolSize;




	TQueue<AGridNodeActorBase*> WallPool;
	TQueue<AGridNodeActorBase*> PathPool;
	AGridNodeActorBase* StartNode;
	AGridNodeActorBase* GoalNode;

	void InitializePool();
	void DoublePoolSize();

	void GetWallNodeFromPool(AGridNodeActorBase*& OutNode);
	void GetPathNodeFromPool(AGridNodeActorBase*& OutNode);

public:	
	// Sets default values for this actor's properties
	APool();

	void GetNodeFromPool(EGridActorType NodeType, AGridNodeActorBase*& OutNode);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
