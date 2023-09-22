#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Pickup.generated.h"

class UBoxComponent;
class ACPP_PlayerCharacter;

UCLASS(Abstract)
class DRONESHOOTER_API ACPP_Pickup : public AActor
{
	GENERATED_BODY()

private:
#pragma region Components
	UBoxComponent* BoxCollider;
	UStaticMeshComponent* BoxMesh;
	FString PathToMesh;
#pragma endregion Components

protected:
	// Used in children
	ACPP_PlayerCharacter* PlayerCharacter;

public:
	// Sets default values for this actor's properties
	ACPP_Pickup() {};
	ACPP_Pickup(const FString& MeshPath);

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
