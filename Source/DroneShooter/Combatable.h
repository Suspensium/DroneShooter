#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Combatable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatable : public UInterface
{
	GENERATED_BODY()
};

class UWorld;
class ACPP_Projectile;

class DRONESHOOTER_API ICombatable
{
	GENERATED_BODY()

protected:
	float Health;
	float MaxHealth;

public:
	float GetHealth() { return Health; }
	float GetMaxHealth() { return MaxHealth; }

	virtual void Shoot(UWorld* const World, const FRotator& SpawnRotation, const FVector& SpawnLocation, TSubclassOf<ACPP_Projectile>& ProjectileSubclass);

	virtual void TakeOnHitDamage(float Damage);
	virtual void Die() = 0;
};