// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TryAgainButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;

	void Show();

protected:

	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void TryAgain();

	UFUNCTION()
	void Quit();

};
