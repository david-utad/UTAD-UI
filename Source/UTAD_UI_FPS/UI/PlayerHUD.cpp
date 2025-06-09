// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Crosshair.h"
#include "AmmoCounter.h"
#include "PlayerHealthBar.h"
#include "ReloadBar.h"

void UPlayerHUD::ShowNoWeapon()
{
	CrosshairWidget->Show();
	AmmoCounterWidget->Hide();
	PlayerHealthBarWidget->Show();
	ReloadBarWidget->Hide();
	MinimapWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerHUD::ShowAll()
{
	CrosshairWidget->Show();
	AmmoCounterWidget->Show();
	PlayerHealthBarWidget->Show();
	ReloadBarWidget->Show();
	MinimapWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerHUD::Hide()
{
	CrosshairWidget->Hide();
	AmmoCounterWidget->Hide();
	PlayerHealthBarWidget->Hide();
	ReloadBarWidget->Hide();
	MinimapWidget->SetVisibility(ESlateVisibility::Hidden);
}
