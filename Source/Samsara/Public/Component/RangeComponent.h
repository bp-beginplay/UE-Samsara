// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "RangeComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class ABaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAMSARA_API URangeComponent : public USphereComponent
{
	GENERATED_BODY()

	// 기본 함수
public:
	URangeComponent();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 커스텀 함수
public:
	void Active();
	void DeActive();
private:
	UFUNCTION()
	void BeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	void EndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);


	void SpawnTargetLinkEffect(const AActor* Target);
	void ClearTargetLinkEffects();

	// 변수
private:
	UPROPERTY(EditDefaultsOnly, Category = "Range", meta = (ClampMin = "0.0"))
	float RangeRadius = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> TargetLinkEffect;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> TargetLinkEffects;
	TArray<TObjectPtr<ABaseCharacter>> Arhats;

	// Get/Set 함수
public:
	void SetRangeRadius(float NewRadius) { RangeRadius = NewRadius; }
	float GetRangeRadius() const { return RangeRadius; }
};
