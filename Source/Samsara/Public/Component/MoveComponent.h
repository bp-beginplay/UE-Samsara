// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

DECLARE_DELEGATE(FOnTickFunc);

class UNiagaraSystem;
class UNiagaraComponent;
class AGhostActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMSARA_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

	// 기본 함수
public:
	UMoveComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;




	// 커스텀 함수
public:
	void StartSetTarget();
	bool SetTarget();
	bool Active();

private:
	void UpdateMovePreview();
	void DestroyGhostActor();
	void DestroyPathEffects();
	void Move();
	void FinishMove();


	// 변수
private:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> PathEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float PathEffectHeightOffset = 10.f;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> PathEffects;
	TArray<FVector> PathPoints;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGhostActor> GhostActorClass;
	AGhostActor* GhostActor;

	FOnTickFunc OnTickFunc;

	bool bCanMove = false;
	UPROPERTY(EditDefaultsOnly, Category = "Move", meta = (ClampMin = "1.0"))
	float AcceptanceRadius = 30.f;
	int32 CurrentPointIndex = 0;

	// Get/Set 함수
public:
	const TArray<FVector>& GetPathPoints() const { return PathPoints; }
	AGhostActor* GetGhostActor() const { return GhostActor; }
};
