// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostActor.generated.h"

class USkeletalMeshComponent;
class UMaterialInterface;
class URangeComponent;

UCLASS()
class SAMSARA_API AGhostActor : public AActor
{
	GENERATED_BODY()

public:
	AGhostActor();
protected:
	virtual void BeginPlay() override;


	// 커스텀 함수
public:
	void CopyMeshFrom(const USkeletalMeshComponent* SourceMesh);

private:
	UPROPERTY(VisibleAnywhere, Category = "Ghost")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Ghost")
	TObjectPtr<USkeletalMeshComponent> GhostMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Ghost")
	TObjectPtr<UMaterialInterface> GhostMaterial;

	/** 고스트 위치를 중심으로 한 공격·스킬 사거리. 스폰되면 바로 켜진다. */
	UPROPERTY(VisibleAnywhere, Category = "Range")
	TObjectPtr<URangeComponent> RangeComponent;

	// Get/Set 함수
public:
	USkeletalMeshComponent* GetGhostMesh() const { return GhostMesh; }
};
