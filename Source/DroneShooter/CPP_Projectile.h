#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class ACPP_Projectile : public AActor
{
	GENERATED_BODY()

#pragma region Components
	// Sphere collision component
	USphereComponent* CollisionComp;
	// Projectile movement component
	UProjectileMovementComponent* ProjectileMovement;
	// Static mesh
	UStaticMeshComponent* MeshComponent;
#pragma endregion Components

	float DefaultDamage{ 10.f };

public:
	ACPP_Projectile();

	// Called when projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};