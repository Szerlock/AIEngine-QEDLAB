// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIEngineCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AIEngine.h"

AAIEngineCharacter::AAIEngineCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AAIEngineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAIEngineCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AAIEngineCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAIEngineCharacter::Look);

		EnhancedInputComponent->BindAction(SelectStartNode, ETriggerEvent::Started, this, &AAIEngineCharacter::OnSelectStartNode);
		EnhancedInputComponent->BindAction(SelectGoalNode, ETriggerEvent::Started, this, &AAIEngineCharacter::OnSelectGoalNode);
		EnhancedInputComponent->BindAction(SelectWallNode, ETriggerEvent::Started, this, &AAIEngineCharacter::OnSelectWallNode);
		EnhancedInputComponent->BindAction(PlaceNode, ETriggerEvent::Completed, this, &AAIEngineCharacter::OnPlaceNode);

	}
	else
	{
		UE_LOG(LogAIEngine, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAIEngineCharacter::OnSelectStartNode()
{
	if (GridManager)
	{
		GridManager->CurrentPlacementType = EGridActorType::Start;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Start Node Selected"));
	}
}

void AAIEngineCharacter::OnSelectGoalNode()
{
	if (GridManager)
	{
		GridManager->CurrentPlacementType = EGridActorType::Goal;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Goal Node Selected"));
	}
}

void AAIEngineCharacter::OnSelectWallNode()
{
	if (GridManager)
	{
		GridManager->CurrentPlacementType = EGridActorType::Wall;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Wall Node Selected"));
	}
}

void AAIEngineCharacter::OnPlaceNode()
{
	if (GridManager)
	{
		GridManager->PlaceNode();
	}
}

void AAIEngineCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AAIEngineCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AAIEngineCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AAIEngineCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}