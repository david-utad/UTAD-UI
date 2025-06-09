// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadBar.h"
#include "../UTAD_UI_FPSCharacter.h"
#include "../TP_WeaponComponent.h"
#include "Components/ProgressBar.h"

void UReloadBar::Show()
{
	AUTAD_UI_FPSCharacter* Character = Cast< AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter());
	if (Character)
	{
		UTP_WeaponComponent* WeaponComponent = Character->GetAttachedWeaponComponent();
		if (WeaponComponent)
		{
			WeaponComponent->OnReloading.BindUObject(this, &UReloadBar::UpdateReloadBarValue);
		}
	}
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UReloadBar::Hide()
{
	AUTAD_UI_FPSCharacter* Character = Cast< AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter());
	if (Character)
	{
		UTP_WeaponComponent* WeaponComponent = Character->GetAttachedWeaponComponent();
		if (WeaponComponent)
		{
			WeaponComponent->OnReloading.Unbind();
		}
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UReloadBar::UpdateReloadBarValue(float NewValue)
{
	ReloadBar->SetVisibility(NewValue > 0.f && NewValue < 1.f ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	ReloadBar->SetPercent(NewValue);
}
