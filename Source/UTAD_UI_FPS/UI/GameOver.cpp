// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOver.h"
#include "Components/Button.h"
#include "../UTAD_UI_FPSCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOver::Show()
{
	APlayerController* PlayerController = GetOwningPlayer<APlayerController>();
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
	PlayerController->Pause();
}

void UGameOver::NativeConstruct()
{
	TryAgainButton->OnClicked.AddDynamic(this, &UGameOver::TryAgain);
	QuitButton->OnClicked.AddDynamic(this, &UGameOver::Quit);
}

void UGameOver::TryAgain()
{
	APlayerController* PlayerController = GetOwningPlayer<APlayerController>();
	PlayerController->RestartLevel();
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
}

void UGameOver::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Quit, true);
}
