#pragma once

#include "CoreMinimal.h"
#include "CPP_Pickup.h"
#include "CPP_PickupAmmo.generated.h"

UCLASS()
class DRONESHOOTER_API ACPP_PickupAmmo : public ACPP_Pickup
{
	GENERATED_BODY()
	
	unsigned int AmountOfAmmoToRestore{ 10 };

public:
	ACPP_PickupAmmo();

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
