#include "CPP_PlayerController.h"
#include "CPP_GameState.h"
#include <Kismet/GameplayStatics.h>
#include "CPP_EnemyTurret.h"

void ACPP_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	ACPP_GameState* GameState = GetWorld()->GetGameState<ACPP_GameState>();

	if (GameState)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_EnemyTurret::StaticClass(), FoundActors);

		GameState->SetTargetsCount(FoundActors.Num());
	}
}