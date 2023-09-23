#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combatable.h"
#include "CPP_PlayerCharacter.generated.h"

class UInputComponent;
class ACPP_Projectile;
struct FInputActionValue;
class UInputAction;

UCLASS()
class ACPP_PlayerCharacter : public ACharacter, public ICombatable
{
	GENERATED_BODY()

#pragma region Components
	// Controller
	class ACPP_PlayerController* PlayerController;
	// Mesh
	UStaticMeshComponent* CharacterMesh;
	// Camera
	class UCameraComponent* CameraComponent;
	// Projectile class to spawn
	TSubclassOf<ACPP_Projectile> ProjectileClass;
	// Widget instance
	class UCPP_UserWidget* UserWidget;
#pragma endregion Components

#pragma region Inputs
	class UInputMappingContext* DefaultMappingContext;
	UInputAction* MoveAction;
	UInputAction* LookAction;
	UInputAction* FlyAction;
	UInputAction* ShootAction;
#pragma endregion Inputs

#pragma region Parameters
	// Offset to spawn projectiles
	FVector MuzzleOffset{ 100.f, 0.f, 0.f };
	// Player mechanics parameters
	UPROPERTY(EditAnywhere, meta = (AllowProvateAccess = true))
	unsigned int Ammo{ 10 };
	bool bCanShoot{ true };
	UPROPERTY(EditAnywhere, meta = (AllowProvateAccess = true))
	float DelayBetweenShots{ 0.3f };
#pragma endregion Parameters

#pragma region Player Methods
	// Methods for input actions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fly(const FInputActionValue& Value);
	void HandleShooting();
	void Die() override;
	// Player Input Controller setup
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
#pragma endregion Player Methods

protected:
	virtual void BeginPlay();

public:
	ACPP_PlayerCharacter();

	// Damage and death methods
	void TakeOnHitDamage(float Damage) override;

	// Methods for overlap components
	void RestoreHealth(float HealthAmount);
	void RestoreAmmo(unsigned int AmmoAmount);
};