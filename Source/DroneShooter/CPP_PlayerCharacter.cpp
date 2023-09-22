#include "CPP_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "CPP_PlayerController.h"
#include "CPP_Projectile.h"
#include "Blueprint/UserWidget.h"
#include "CPP_UserWidget.h"

ACPP_PlayerCharacter::ACPP_PlayerCharacter()
{
	Health = 100.f;
	MaxHealth = 100.f;

	GetCapsuleComponent()->InitCapsuleSize(48.f, 64.f);

	// Projectile class
	ProjectileClass = ACPP_Projectile::StaticClass();

	// Create a Camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	CameraComponent->bUsePawnControlRotation = true;

	// Create a Mesh component
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CharacterMesh->SetOnlyOwnerSee(true);
	CharacterMesh->SetupAttachment(CameraComponent);
	CharacterMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Input setup
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/DroneShooter/Input/IMC_Default"));
	MoveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/DroneShooter/Input/Actions/IA_Move"));
	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/DroneShooter/Input/Actions/IA_Look"));
	FlyAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/DroneShooter/Input/Actions/IA_Fly"));
	ShootAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/DroneShooter/Input/Actions/IA_Shoot"));
}

void ACPP_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ACPP_PlayerController>(Controller);

	// Set movement mode so the character can fly
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		Subsystem->AddMappingContext(DefaultMappingContext, 0);

	// Setting up widget
	UserWidget = CreateWidget<UCPP_UserWidget>(PlayerController,
		LoadClass<UCPP_UserWidget>(nullptr, TEXT("/Game/DroneShooter/UI/WBP_UserWidget.WBP_UserWidget_C")));
	if (UserWidget)
		UserWidget->AddToViewport();

	UserWidget->SetHealth(Health, MaxHealth);
	UserWidget->SetAmmo(Ammo);
	UserWidget->SetTargetsLeft();
}

void ACPP_PlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent == nullptr)
		return;

	// Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::Move);

	// Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::Look);

	// Flying
	EnhancedInputComponent->BindAction(FlyAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::Fly);

	// Shooting
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::HandleShooting);
}

void ACPP_PlayerCharacter::TakeOnHitDamage(float Damage)
{
	ICombatable::TakeOnHitDamage(Damage);
	UserWidget->SetHealth(Health, MaxHealth);
}

void ACPP_PlayerCharacter::Die()
{
	// LOSE
	PlayerController->ConsoleCommand("quit");
}

void ACPP_PlayerCharacter::RestoreHealth(float HealthAmount)
{
	Health = FMath::Clamp(Health + HealthAmount, 0.f, MaxHealth);
	if (UserWidget)
		UserWidget->SetHealth(Health, MaxHealth);
}

void ACPP_PlayerCharacter::RestoreAmmo(unsigned int AmmoAmount)
{
	Ammo += AmmoAmount;
	if (UserWidget)
		UserWidget->SetAmmo(Ammo);
}

void ACPP_PlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (PlayerController == nullptr)
		return;

	// Add movement
	AddMovementInput(GetActorRightVector(), MovementVector.X);
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
}

void ACPP_PlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (PlayerController == nullptr)
		return;

	// Add yaw and Pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ACPP_PlayerCharacter::Fly(const FInputActionValue& Value)
{
	float FlyingVector = Value.Get<float>();

	if (PlayerController == nullptr)
		return;

	// Add flying movement
	AddMovementInput(GetActorUpVector(), FlyingVector, true);
}

void ACPP_PlayerCharacter::HandleShooting()
{
	if (Ammo <= 0 || !bCanShoot || ProjectileClass == nullptr)
		return;

	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	ICombatable::Shoot(World, SpawnRotation, SpawnLocation, ProjectileClass);

	UserWidget->SetAmmo(--Ammo);

	// Add a delay between shots
	bCanShoot = false;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]() { bCanShoot = true; }, DelayBetweenShots, false);
}