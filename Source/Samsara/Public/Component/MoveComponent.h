// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/PathComponent.h"
#include "MoveComponent.generated.h"

class USplineMeshComponent;
class UStaticMesh;
class UMaterialInterface;
class URouteComponent;
class AGhostActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAMSARA_API UMoveComponent : public UPathComponent
{
	GENERATED_BODY()

	// 기본 함수
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// 커스텀 함수
public:
	void Active();
	void DeActive();
	bool SetTarget();
	bool Move();
	void StopMove();
private:
	void UpdateAutoMove(float DeltaTime, APawn* Pawn);

private:
	UPROPERTY(Transient)
	TObjectPtr<URouteComponent> RouteComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGhostActor> GhostActorClass;
	AGhostActor* GhostActor;

	UPROPERTY(EditDefaultsOnly, Category = "AutoMove", meta = (ClampMin = "10.0"))
	float LookAheadDistance = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "AutoMove", meta = (ClampMin = "1.0"))
	float AcceptanceRadius = 30.f;
	UPROPERTY(EditDefaultsOnly, Category = "AutoMove", meta = (ClampMin = "1.0"))
	float SlowDownDistance = 150.f;
};
