// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "PathComponent.generated.h"

class UStaticMesh;
class UMaterialInterface;
class USplineMeshComponent;

UCLASS()
class SAMSARA_API UPathComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	UPathComponent();

protected:
	void UpdatePathVisual(float CurDist);
	void BuildPathTo(const FVector& Destination);
	void HidePathVisual();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Path")
	TObjectPtr<UStaticMesh> PathMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Path")
	TObjectPtr<UMaterialInterface> PathMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Path", meta = (ClampMin = "10.0"))
	float SegmentLength = 100.f;
	UPROPERTY(Transient)
	TArray<TObjectPtr<USplineMeshComponent>> PathSegments;
	UPROPERTY(EditDefaultsOnly, Category = "Path", meta = (ClampMin = "0.01"))
	float PathWidthScale = 0.15f;
	UPROPERTY(EditDefaultsOnly, Category = "Path")
	float PathHeightOffset = 8.f;
};
