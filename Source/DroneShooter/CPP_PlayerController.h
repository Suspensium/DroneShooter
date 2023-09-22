#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

UCLASS()
class DRONESHOOTER_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()

	void BeginPlay() override;
};