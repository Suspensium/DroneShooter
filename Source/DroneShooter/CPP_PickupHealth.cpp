#include "CPP_PickupHealth.h"
#include "CPP_PlayerCharacter.h"

ACPP_PickupHealth::ACPP_PickupHealth() : Super(TEXT("/Game/DroneShooter/Assets/S_Pickup_Health"))
{
}

void ACPP_PickupHealth::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Pickup::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	PlayerCharacter->RestoreHealth(AmountOfHealthToRestore);
	Destroy();
}