// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NavPathComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class AGhostActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMSARA_API UNavPathComponent : public UActorComponent
{
	GENERATED_BODY()

	// 기본 함수
public:
	UNavPathComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// 커스텀 함수
public:
	void Active();
	void DeActive();


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

	// Get/Set 함수
public:
	const TArray<FVector>& GetPathPoints() const { return PathPoints; }
};
