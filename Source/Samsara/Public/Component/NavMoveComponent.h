// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NavMoveComponent.generated.h"

class UNavPathComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMSARA_API UNavMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNavMoveComponent();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 변수
private:
	UPROPERTY()
	TObjectPtr<UNavPathComponent> NavPathComponent;

	// Get/Set 함수
public:
	void SetNavPathComponent(UNavPathComponent* InNavPathComponent) { NavPathComponent = InNavPathComponent; }
	UNavPathComponent* GetNavPathComponent() const { return NavPathComponent; }
};
