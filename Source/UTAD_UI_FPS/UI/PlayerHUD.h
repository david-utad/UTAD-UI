// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UAmmoCounter;
class UCrosshair;
class UPlayerHealthBar;
class UReloadBar;

/**
 *
 */
UCLASS()
class UTAD_UI_FPS_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Binding of widgets that must be displayed in the HUD */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UAmmoCounter* AmmoCounterWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCrosshair* CrosshairWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UPlayerHealthBar* PlayerHealthBarWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UReloadBar* ReloadBarWidget;


	/*
	El minimapa lo implementamos con un RenderTexture y un SceneCaptureComponent2D

	Pasos para montarlo:
	- Añadir un SpringArm al BP_FirstPersonCharacter y rotarlo
	- Añadir un SceneCaptureComponent2D y rotarlo
	- Cambiar Projection Type a Orthographic para ignorar la profundidad
	- En Scene Capture > Texture Target crear un Render Target
	- A partir del Render Target creado (RT_Minimap) creamos un Material (Mat_Minimap)
	- En un Widget, añadir un Image y asignar Mat_Minimap

	Esta implementación es funcional pero no demasiado eficiente, y tenemos menos control sobre ello.
	Por lo general, es conveniente reducir el número de cámaras que tengamos en juego al mínimo posible (menos cámaras, menos renderizado).
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUserWidget* MinimapWidget;

	/** Show the Minimap and the PlayerHealthBar */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void ShowNoWeapon();

	/** Show all the widgets */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void ShowAll();

	/** Hide all the widgets */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

};
