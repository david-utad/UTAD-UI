/************************************************************************
 * @description: Skill tree widget class to show / hide the screen.
 * @author: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UTextBlock;
class USoundBase;
class UButton;

UENUM()
enum class ESoundType : uint8
{
  OpenClose UMETA(DisplayName = "Open-Close Menu"),
  Confirm UMETA(DisplayName = "Confirm"),
  Focus UMETA(DisplayName = "Focus"),
  Select UMETA(DisplayName = "Select"),
  Deselect UMETA(DisplayName = "Deselect"),
  Error UMETA(DisplayName = "Error")
};

UCLASS()
class UTAD_UI_FPS_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

  /**
   * It returns if the widget is visible in the screen. 
   */
  bool IsVisible();

  /**
   * It shows the widget in the screen.
   */
  void Show();

  /**
   * It hides the widget.
   */
  UFUNCTION()
  void Hide();

  /**
   * It checks if there is enought available points to acquire the skils.
   * @param _iCost skill cost to add.
   */
  bool IsThereEnoughtPoints(int32 _iCost);

  /**
   * It adds the cost parameter to the available points.
   * @param _iCost skill cost to add. If it's negative, the available point will be reduced.
   */
  void ModifyAvailablePoints(int32 _iCost);

  /**
   * It shows the feedback text in the widget.
   * @param _sFeedbackText text to show.
   */
  void SetFeedbackText(FText _sFeedbackText);

  /**
   * It reproduces a sound in the widget.
   * @param _eType sound type to reproduce.
   */
  void PlaySound(ESoundType _eType);

  /**
   * It return if the skill tree is in held mode. 
   */
  bool IsInHeldMode();

  /**
   * It returns the necessary time to held the button to acquire the skill. 
   */
  double GetHeldTime();

protected:

  /**
   * Widget functions.
   */
  virtual void NativeConstruct() override;
  virtual void NativeDestruct() override;

private:

  /**
   * Function to hide the feedback text after the timer.
   */
  void HideFeedbackText();

  /**
   * Function to show the available points in the screen.
   */
  void ShowAvailablePoints();

  /**
   * Function to acquire all the selected skills. 
   */
  UFUNCTION()
  void ConfirmSkills();

  /**
   * The panel of the widget.
   */
  UPROPERTY(meta = (DisplayName = "Panel", BindWidget))
  class UCanvasPanel* m_pPanel;

  /**
   * The background image of the panel.
   */
  UPROPERTY(meta = (DisplayName = "Background Image", BindWidget))
  class UImage* m_pBackgroundImage;

  /**
   * The title of the panel.
   */
  UPROPERTY(meta = (DisplayName = "Title", BindWidget))
  UTextBlock* m_pTitleText;

  /**
   * The text to show the available points.
   */
  UPROPERTY(meta = (DisplayName = "Points Text", BindWidget))
  UTextBlock* m_pPointsText;

  /**
   * The text to show the available points value.
   */
  UPROPERTY(meta = (DisplayName = "Points Value Text", BindWidget))
  UTextBlock* m_pPointsValueText;

  /**
   * The button to acquire all the selected skills.
   */
  UPROPERTY(meta = (DisplayName = "Confirm Button", BindWidget))
  UButton* m_pConfirmButton;

  /**
   * The button to close the skill tree screen and deselect all the skills.
   */
  UPROPERTY(meta = (DisplayName = "Close Button", BindWidget))
  UButton* m_pCloseButton;

  /**
   * The feedback text when something wrong is happened.
   */
  UPROPERTY(meta = (DisplayName = "Feedback Text", BindWidget))
  UTextBlock* m_pFeedbackText;

  /**
   * The available sounds map to play.
   */
  UPROPERTY(EditAnywhere, Category = "Skill Tree", meta = (DisplayName = "Sound Map"))
  TMap<ESoundType, USoundBase*> m_mSound;

  /**
   * It is the necessary time to held the button to acquire the skill.
   */
  UPROPERTY(EditAnywhere, Category = "Skill Tree", meta = (DisplayName = "Held time"))
  double m_dHeldTime;

  /**
   * If it is checked the held mode is active and the skill nodes can't be selected, only acquired.
   */
  UPROPERTY(EditAnywhere, Category = "Skill Tree", meta = (DisplayName = "Held mode"))
  bool m_bHeldMode;

  /**
   * The skill branch children in the tree.
   */
  TArray<class USkillBranchWidget*> m_lSkillBranch;

  /**
   * Timer to hide the feedback text.
   */
  FTSTicker::FDelegateHandle m_tFeedbackTickerHandle;
};