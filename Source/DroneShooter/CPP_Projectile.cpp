#include "CPP_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Combatable.h"
#include "Components/StaticMeshComponent.h"

ACPP_Projectile::ACPP_Projectile()
{
	// Collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(20.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Binding event when projectile hits
	CollisionComp->OnComponentHit.AddDynamic(this, &ACPP_Projectile::OnHit);

	// Set up as root component
	RootComponent = CollisionComp;

	// Static mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/DroneShooter/Assets/S_Projectile"));
	if (MeshAsset.Succeeded())
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	MeshComponent->SetRelativeScale3D({ 0.25f, 0.25f, 0.25f });

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after half a second second by default
	InitialLifeSpan = 0.5f;
}

void ACPP_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;

	// DAMAGE LOGIC
	ICombatable* HitTarget = Cast<ICombatable>(OtherActor);
	if (HitTarget)
		HitTarget->TakeOnHitDamage(DefaultDamage);

	// Destroy after hit
	Destroy();
}