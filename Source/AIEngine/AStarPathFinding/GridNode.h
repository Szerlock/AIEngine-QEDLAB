#pragma once

#include "CoreMinimal.h"
#include "GridNode.generated.h"

USTRUCT(BlueprintType)
struct FGridNode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldPosition;

	//// State field
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCrossable;

	//////// CONSTRUCTOR ////////
	FGridNode() : WorldPosition(FVector::ZeroVector), IsCrossable(true)
	{

	}
};
