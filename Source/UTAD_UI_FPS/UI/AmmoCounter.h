// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoCounter.generated.h"

/**
 *
 */
UCLASS()
class UTAD_UI_FPS_API UAmmoCounter : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TotalAmmo;

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

private:

	void UpdateCurrentAmmo(int NewCurrentAmmo);

	void UpdateTotalAmmo(int NewTotalAmmo);
};
