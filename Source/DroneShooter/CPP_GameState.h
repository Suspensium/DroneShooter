#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CPP_GameState.generated.h"

UCLASS()
class DRONESHOOTER_API ACPP_GameState : public AGameStateBase
{
	GENERATED_BODY()

	// On turret destroyed event
	DECLARE_EVENT(ACPP_GameState, FTurretDestroyedEvent);

	// Targets left in the game
	unsigned int TargetsCount;

public:
	FTurretDestroyedEvent OnTurretDestroyed;

	void SetTargetsCount(unsigned int Count) { TargetsCount = Count; }
	unsigned int GetTargetsCount() { return TargetsCount; }
};