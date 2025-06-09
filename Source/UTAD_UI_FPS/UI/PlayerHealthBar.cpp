// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "../UTAD_UI_FPSCharacter.h"
#include "Components/ProgressBar.h"
#include "Math/Color.h"
#include "Styling/SlateColor.h"

#define BLINK_ANIMATION_TIME 1.f
#define BLINK_THRESHOLD 0.25f

void UPlayerHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bIsLowHealth)
	{
		BlinkTimer = FMath::Min(BlinkTimer + InDeltaTime, BLINK_ANIMATION_TIME);
		LowHealthBlink();
	}
}

void UPlayerHealthBar::Show()
{
	AUTAD_UI_FPSCharacter* Character = Cast< AUTAD_UI_FPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->OnPlayerHealthChanged.BindUObject(this, &UPlayerHealthBar::UpdatePlayerHealthBar);
	}
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerHealthBar::Hide()
{
	AUTAD_UI_FPSCharacter* Character = Cast< AUTAD_UI_FPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->OnPlayerHealthChanged.Unbind();
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHealthBar::UpdatePlayerHealthBar(int NewHealth, int MaxHealth)
{
	float Percent = static_cast<float>(NewHealth) / static_cast<float>(MaxHealth);
	PlayerHealthBar->SetPercent(Percent);
	if (!bIsLowHealth && Percent <= BLINK_THRESHOLD)
	{
		bIsLowHealth = true;
		BlinkTimer = 0.f;
	}
	else if (bIsLowHealth && Percent > BLINK_THRESHOLD)
	{
		bIsLowHealth = false;
	}
}

void UPlayerHealthBar::LowHealthBlink()
{
	FColor InitialColor = FColor(bBlinkTurningRed ? FColor::Red : FColor::Black);
	FColor FinalColor = FColor(bBlinkTurningRed ? FColor::Black : FColor::Red);

	FSlateColor CurrentColor = FLinearColor::LerpUsingHSV(InitialColor, FinalColor, BlinkTimer / BLINK_ANIMATION_TIME);

	FProgressBarStyle PBStyle = PlayerHealthBar->GetWidgetStyle();
	PBStyle.BackgroundImage.TintColor = CurrentColor;
	PlayerHealthBar->SetWidgetStyle(PBStyle);

	if (BlinkTimer == BLINK_ANIMATION_TIME)
	{
		bBlinkTurningRed = !bBlinkTurningRed;
		BlinkTimer = 0.f;
	}
}
