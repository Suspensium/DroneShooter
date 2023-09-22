#include "CPP_Pickup.h"
#include "Components/BoxComponent.h"
#include "CPP_PlayerCharacter.h"

// Sets default values
ACPP_Pickup::ACPP_Pickup(const FString& MeshPath)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapCollider"));
	BoxCollider->SetCollisionProfileName("Pickup");
	BoxCollider->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Pickup::OnOverlap);

	RootComponent = BoxCollider;

	// Create a Mesh component
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BoxMesh->SetupAttachment(RootComponent);
	BoxMesh->SetCollisionProfileName("NoCollision");
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(*MeshPath);
	if (MeshAsset.Succeeded())
		BoxMesh->SetStaticMesh(MeshAsset.Object);
	BoxMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	BoxMesh->SetCastShadow(false);
}

void ACPP_Pickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerCharacter = Cast<ACPP_PlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;
}