#include "Combatable.h"
#include "CPP_Projectile.h"

// Add default functionality here for any ICombatable functions that are not pure virtual.

void ICombatable::Shoot(UWorld* const World, const FRotator& SpawnRotation, const FVector& SpawnLocation, TSubclassOf<ACPP_Projectile>& ProjectileSubclass)
{
	// Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	World->SpawnActor<ACPP_Projectile>(ProjectileSubclass, SpawnLocation, SpawnRotation, ActorSpawnParams);
}

void ICombatable::TakeOnHitDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	if (Health == 0.f)
		Die();
}