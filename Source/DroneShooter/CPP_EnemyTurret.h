#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Combatable.h"
#include "UObject/WeakObjectPtr.h"
#include "CPP_EnemyTurret.generated.h"

class ACPP_PlayerCharacter;
class ACPP_Projectile;

UCLASS()
class DRONESHOOTER_API ACPP_EnemyTurret : public APawn, public ICombatable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACPP_EnemyTurret();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Weak pointer to Target player so if it is destroyed it automatically resets pointer
	TWeakObjectPtr<ACPP_PlayerCharacter> TargetPlayerCharacter;

#pragma region Components
	// Mesh
	UStaticMeshComponent* TurretMesh;
	// Collider
	class UCapsuleComponent* TurretCollider;
	// Player overlapping collider
	class USphereComponent* PlayerOverlapCollider;
	// Projectile class to spawn
	TSubclassOf<ACPP_Projectile> ProjectileClass;
	class ACPP_GameState* GameState;
#pragma endregion Components

#pragma region Parameters
	// Offset to spawn projectiles
	FVector MuzzleOffset{ 100.f, 0.f, 0.f };
	bool bCanShoot{ true };
	float DelayBetweenShots{ 1.f };
	float FireRadius{ 1500.f };
	bool bTargetInRange{ false };
	float AngleTolerance{ 1.f };
	unsigned int TurretBurst{};
	unsigned int BurtsShotsCount{ 5 };
#pragma endregion Parameters

#pragma region Turret methods
	void HandleShooting();
	void Die() override;
	// Scan for player
	UFUNCTION()
	void ScanArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void StopScanArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Rotates turret towards target (player)
	bool RotateTowardsTarget();
#pragma endregion Turret methods
};