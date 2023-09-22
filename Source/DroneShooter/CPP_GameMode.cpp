#include "CPP_GameMode.h"
#include "CPP_PlayerCharacter.h"
#include "CPP_PlayerController.h"
#include "CPP_GameState.h"
#include "UObject/ConstructorHelpers.h"

ACPP_GameMode::ACPP_GameMode() : Super()
{
	// Default Pawn class
	DefaultPawnClass = ACPP_PlayerCharacter::StaticClass();
	// Default Player Controller
	PlayerControllerClass = ACPP_PlayerController::StaticClass();
	// Default Game State
	GameStateClass = ACPP_GameState::StaticClass();
}