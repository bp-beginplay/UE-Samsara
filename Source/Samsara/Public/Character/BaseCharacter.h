// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/TeamInterface.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTexture2D;
class ABaseCharacter;
class UDA_ArhatInput;
struct FInputActionValue;
struct FInputActionInstance;
class ASkillRange;
class URangeComponent;
class UMoveComponent;
class UMoveRangeComponent;

USTRUCT()
struct FArhatIcons
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* ArhatIcon = nullptr;
};

UCLASS()
class SAMSARA_API ABaseCharacter : public ACharacter, public ITeamInterface
{
	GENERATED_BODY()

	// 기본 함수
public:
	ABaseCharacter();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void UnPossessed();
	virtual void BeginPlay() override;


	// 커스텀 함수
private:
	void MoveCamera(const FInputActionValue& Value);
	void CompleteLMB();
	void StartedSB();
	void StartedA();
	void StartedS();

	UFUNCTION(Reliable, Client)
	void ClientInitCharacter();

	// 변수
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraSmoothing = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraSpeed = 30.0f;
	FVector CameraLocation;

	UPROPERTY(EditDefaultsOnly)
	FArhatIcons ArhatIcons;
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 TeamId = INDEX_NONE;
	UPROPERTY(EditDefaultsOnly)
	UDA_ArhatInput* DA_ArhatInput;

	UPROPERTY(VisibleAnywhere, Category = "Range")
	TObjectPtr<URangeComponent> RangeComponent;
	UPROPERTY(VisibleAnywhere, Category = "Range")
	TObjectPtr<UMoveComponent> MoveComponent;
	/** 목적지를 고르는 동안 실제 이동 가능 범위를 바닥에 표시하는 데칼. */
	UPROPERTY(VisibleAnywhere, Category = "Range")
	TObjectPtr<UMoveRangeComponent> MoveRangeComponent;

	// Get/Set 함수
public:
	virtual void SetTeamId(int32 NewTeamId) override { TeamId = NewTeamId; }

	FArhatIcons GetArhatIcons() const { return ArhatIcons; }
	virtual int32 GetTeamId() const override { return TeamId; }
};
