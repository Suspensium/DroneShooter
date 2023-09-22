#include "CPP_PickupAmmo.h"
#include "CPP_PlayerCharacter.h"

ACPP_PickupAmmo::ACPP_PickupAmmo() : Super(TEXT("/Game/DroneShooter/Assets/S_Pickup_Ammo"))
{
}

void ACPP_PickupAmmo::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Pickup::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	PlayerCharacter->RestoreAmmo(AmountOfAmmoToRestore);
	Destroy();
}