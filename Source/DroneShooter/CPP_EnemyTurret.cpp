#include "CPP_EnemyTurret.h"
#include "CPP_GameState.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "CPP_PlayerCharacter.h"
#include "CPP_Projectile.h"

// Sets default values
ACPP_EnemyTurret::ACPP_EnemyTurret()
{
	Health = 30.f;
	MaxHealth = 30.f;

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileClass = ACPP_Projectile::StaticClass();

	// Create collider component
	TurretCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	TurretCollider->InitCapsuleSize(72.f, 64.f);
	TurretCollider->BodyInstance.SetCollisionProfileName("Turret");

	RootComponent = TurretCollider;

	// Create a Mesh component
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TurretMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/DroneShooter/Assets/S_Turret"));
	if (MeshAsset.Succeeded())
		TurretMesh->SetStaticMesh(MeshAsset.Object);

	// Create player overlap collider component
	PlayerOverlapCollider = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerOverlapCollider"));
	PlayerOverlapCollider->InitSphereRadius(FireRadius);
	PlayerOverlapCollider->SetupAttachment(RootComponent);
	PlayerOverlapCollider->BodyInstance.SetCollisionProfileName("TurretTrigger");
	PlayerOverlapCollider->OnComponentBeginOverlap.AddDynamic(this, &ACPP_EnemyTurret::ScanArea);
	PlayerOverlapCollider->OnComponentEndOverlap.AddDynamic(this, &ACPP_EnemyTurret::StopScanArea);
}

// Called when the game starts or when spawned
void ACPP_EnemyTurret::BeginPlay()
{
	Super::BeginPlay();
	
	GameState = GetWorld()->GetGameState<ACPP_GameState>();
}

void ACPP_EnemyTurret::HandleShooting()
{
	if (!bCanShoot || ProjectileClass == nullptr) return;

	UWorld* const World = GetWorld();
	if (World == nullptr) return;

	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	ICombatable::Shoot(World, SpawnRotation, SpawnLocation, ProjectileClass);

	// Add a delay between bursting shots
	if (++TurretBurst >= BurtsShotsCount)
	{
		bCanShoot = false;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]() {TurretBurst = 0;  bCanShoot = true; }, DelayBetweenShots, false);
	}
}

void ACPP_EnemyTurret::Die()
{
	if (GameState)
	{
		GameState->SetTargetsCount(GameState->GetTargetsCount() - 1);
		GameState->OnTurretDestroyed.Broadcast();
	}
	Destroy();

	if (GameState->GetTargetsCount() == 0)
	{
		// WIN
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("WIN"));
	}
}

void ACPP_EnemyTurret::ScanArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_PlayerCharacter* PlayerCharacter = Cast<ACPP_PlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		bTargetInRange = true;
		TargetPlayerCharacter = PlayerCharacter;
	}
}

void ACPP_EnemyTurret::StopScanArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bTargetInRange = false;
	TargetPlayerCharacter = nullptr;
}

bool ACPP_EnemyTurret::RotateTowardsTarget()
{
	if (!TargetPlayerCharacter.IsValid()) return false;
	// Calculate the direction from the turret to the player
	FVector TurretLocation = GetActorLocation();
	FVector PlayerLocation = TargetPlayerCharacter->GetActorLocation();
	FVector TargetDirection = (PlayerLocation - TurretLocation).GetSafeNormal();

	// Calculate the rotation from the direction
	FRotator TargetRotation = TargetDirection.Rotation();
	TargetRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 7.f);

	SetActorRotation(TargetRotation);
	return FMath::IsNearlyEqual(GetActorRotation().Yaw, TargetRotation.Yaw, AngleTolerance);
}

// Called every frame
void ACPP_EnemyTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTargetInRange)
		if (RotateTowardsTarget())
			HandleShooting();
}