// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Net/UnrealNetWork.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Data/Input/DA_ArhatInput.h"
#include "Actor/SkillRange.h"
#include "PlayerController/PvPPlayerController.h"
#include "Component/MoveComponent.h"
#include "Component/RangeComponent.h"

#include "LogTest.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	GetMesh()->SetReceivesDecals(false);

	RangeComponent = CreateDefaultSubobject<URangeComponent>(TEXT("RangeComponent"));
	RangeComponent->SetupAttachment(RootComponent);

	//SpringArm->SocketOffset = FVector(0.f, 60.f, 70.f);
	//SpringArm->TargetArmLength = 500.f;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, TeamId);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector InterpolateLocation = UKismetMathLibrary::VInterpTo(SpringArm->TargetOffset, CameraLocation, DeltaTime, CameraSmoothing);
	SpringArm->TargetOffset = InterpolateLocation;
}

void ABaseCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	SpringArm->TargetOffset = FVector::ZeroVector;
	CameraLocation = FVector::ZeroVector;

	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//SkillRange = GetWorld()->SpawnActor<ASkillRange>(MoveRangeClass, GetActorTransform(), SpawnParams);

	//MoveComponent->Active();

	//RangeComponent->Active();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	DA_ArhatInput->AddMappingArhat(GetController());
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//EBindInput::Triggered(Input, DA_ArhatInput->IA_ArhatWASD, this, &ABaseCharacter::MoveCamera);
		EBindInput::Triggered(Input, DA_ArhatInput->IA_ArhatMouxeXY, this, &ABaseCharacter::MoveCamera);
		EBindInput::Complete(Input, DA_ArhatInput->IA_ArhatLMB, this, &ABaseCharacter::CompleteLMB);
		EBindInput::Started(Input, DA_ArhatInput->IA_ArhatSB, this, &ABaseCharacter::StartedSB);
		EBindInput::Started(Input, DA_ArhatInput->IA_ArhatA, this, &ABaseCharacter::StartedA);
		EBindInput::Started(Input, DA_ArhatInput->IA_ArhatS, this, &ABaseCharacter::StartedS);
	}
}

void ABaseCharacter::UnPossessed()
{
	ClientInitCharacter();

	Super::UnPossessed();
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::ClientInitCharacter_Implementation()
{
	//SkillRange->Destroy();
	MoveComponent->DeActive();
	RangeComponent->DeActive();
}

void ABaseCharacter::MoveCamera(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	CameraLocation += FVector(Input.Y, Input.X, 0.f) * CameraSpeed;
}

void ABaseCharacter::CompleteLMB()
{
	if (MoveComponent->SetTarget())
	{
		RangeComponent->DeActive();
	}
	else
	{
		LOG_TEXT("Fail SetTarget")
	}
}

void ABaseCharacter::StartedSB()
{
	if (MoveComponent->Move())
	{
		LOG_TEXT("Success Move");
		RangeComponent->DeActive();
	}
	else
	{
		LOG_TEXT("Fail Move")
	}
}

void ABaseCharacter::StartedA()
{
}

void ABaseCharacter::StartedS()
{
	MoveComponent->Active();
}
