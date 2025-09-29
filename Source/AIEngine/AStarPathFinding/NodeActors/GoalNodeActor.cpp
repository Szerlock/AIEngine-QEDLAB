// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarPathFinding/NodeActors/GoalNodeActor.h"

AGoalNodeActor::AGoalNodeActor()
{
	NodeType = EGridActorType::Goal;
}

void AGoalNodeActor::BeginPlay()
{
	Super::BeginPlay();
	SetupNodeColor(NodeType, ENodeState::Default);
}