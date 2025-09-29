// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeActors/StartNodeActor.h"

// Sets default values
AStartNodeActor::AStartNodeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStartNodeActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetupNodeColor(NodeType, ENodeState::Default);
}

// Called every frame
void AStartNodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

