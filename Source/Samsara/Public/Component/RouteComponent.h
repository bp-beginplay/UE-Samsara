// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/PathComponent.h"
#include "RouteComponent.generated.h"

class UStaticMesh;
class UMaterialInterface;
class USplineMeshComponent;

UCLASS(ClassGroup = (Custom))
class SAMSARA_API URouteComponent : public UPathComponent
{
	GENERATED_BODY()

	// 기본 함수
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 커스텀 함수
public:
	void Active();
	void DeActive();

private:
	FVector TargetLocation = FVector::ZeroVector;
	bool bSetTarget = false;

public:
	bool GetSetTarget() const { return bSetTarget; }
	FVector GetTargetLocation() const { return TargetLocation; }
};
