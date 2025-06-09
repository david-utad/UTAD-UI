/************************************************************************
 * @description: Skill branch widget class to show in the skill tree screen.
 * @author: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBranchWidget.generated.h"

class USkillTreeWidget;

UCLASS()
class UTAD_UI_FPS_API USkillBranchWidget : public UUserWidget
{
	GENERATED_BODY()

public:

  /**
   * It collects all the skill nodes in the widget and setup the skill tree parent.
   * @param _pSkillTree skill tree parent.
   */
  void SetupBranch(USkillTreeWidget* _pSkillTree);

  /**
   * It deselects all selected skill nodes.
   */
  void DeselectNodes();

  /**
   * It acquires all the selected skills.
   */
  void AcquireSkills();

protected:

  /**
   * Widget construct function.
   */
  virtual void NativeConstruct() override;

private:

  /**
   * The panel of the widget.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Branch", meta = (DisplayName = "Panel", BindWidget))
  class UCanvasPanel* m_pPanel;

  /**
   * The background image of the panel.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Branch", meta = (DisplayName = "Background Image", BindWidget))
  class UImage* m_pBackgroundImage;

  /**
   * The title of the panel.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Branch", meta = (DisplayName = "Title", BindWidget))
  class UTextBlock* m_pTitleText;

  /**
   * The skill node children in the branch.
   */
  TMap<FName, class USkillNodeWidget*> m_mSkillNode;
};