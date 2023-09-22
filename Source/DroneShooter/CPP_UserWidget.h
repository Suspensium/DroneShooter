#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_UserWidget.generated.h"

class ACPP_GameState;
class UProgressBar;
class UTextBlock;

UCLASS()
class DRONESHOOTER_API UCPP_UserWidget : public UUserWidget
{
	GENERATED_BODY()

	ACPP_GameState* GameState;

public:
	virtual void NativeConstruct() override;

	// Update HUD with current health
	void SetHealth(float CurrentHealth, float MaxHealth);

	// Update HUD with current ammo
	void SetAmmo(unsigned int CurrentAmmo);

	// Update HUD with current targets left
	void SetTargetsLeft();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AmmoText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TargetsLeftText;
};