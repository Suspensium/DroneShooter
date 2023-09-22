#pragma once

#include "CoreMinimal.h"
#include "CPP_Pickup.h"
#include "CPP_PickupHealth.generated.h"

UCLASS()
class DRONESHOOTER_API ACPP_PickupHealth : public ACPP_Pickup
{
	GENERATED_BODY()

	float AmountOfHealthToRestore{ 30.f };

public:
	ACPP_PickupHealth();

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};