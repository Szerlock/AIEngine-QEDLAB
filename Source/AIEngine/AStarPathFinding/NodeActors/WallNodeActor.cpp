// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeActors/WallNodeActor.h"

// Sets default values
AWallNodeActor::AWallNodeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWallNodeActor::BeginPlay()
{
	Super::BeginPlay();
	SetupNodeColor(NodeType, ENodeState::Default);

}

