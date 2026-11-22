#include "Component/RouteComponent.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "LogTest.h"
#include "PlayerController/PvPPlayerController.h"
#include "Character/BaseCharacter.h"

void URouteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	APvPPlayerController* PC = Cast<APvPPlayerController>(Character->GetController());

	if (PC && PC->IsLocalController())
	{
		FHitResult Hit;
		Hit = PC->GetUnderCursorHit();
		TargetLocation = Hit.Location;
		BuildPathTo(TargetLocation);

		float GroundSpeed = Character->GetCharacterMovement()->GetMaxSpeed();
		if (GetSplineLength() <= GroundSpeed)
		{
			UpdatePathVisual(0.f);
			bSetTarget = true;
		}
		else
		{
			HidePathVisual();
			bSetTarget = false;
		}
	}
}

void URouteComponent::Active()
{
	SetComponentTickEnabled(true);

}

void URouteComponent::DeActive()
{
	SetComponentTickEnabled(false);
	HidePathVisual();
	ClearSplinePoints(true);
}