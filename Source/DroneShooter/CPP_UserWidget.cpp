#include "CPP_UserWidget.h"
#include "CPP_GameState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCPP_UserWidget::NativeConstruct()
{
	GameState = GetWorld()->GetGameState<ACPP_GameState>();
	if (GameState)
	{
		GameState->OnTurretDestroyed.AddUObject(this, &UCPP_UserWidget::SetTargetsLeft);
	}
}

void UCPP_UserWidget::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (!HealthBar) return;

	HealthBar->SetPercent(CurrentHealth / MaxHealth);
}

void UCPP_UserWidget::SetAmmo(unsigned int CurrentAmmo)
{
	if (!AmmoText) return;

	AmmoText->SetText(FText::AsNumber(CurrentAmmo));
}

void UCPP_UserWidget::SetTargetsLeft()
{
	if (!TargetsLeftText) return;

	FText Text = FText::Format(FText::FromString("Targets Left: {0}"), FText::AsNumber(GameState->GetTargetsCount()));
	TargetsLeftText->SetText(Text);
}