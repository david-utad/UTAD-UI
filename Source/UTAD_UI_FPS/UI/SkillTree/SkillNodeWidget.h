/************************************************************************
 * @description: Skill node widget class to show in the skill tree screen.
 * @author: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillNodeWidget.generated.h"

class USkillTreeWidget;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLockNode, bool /* _bLocked */);

UENUM()
enum class ESkillType : uint8
{
  None UMETA(DisplayName = "None"),
  Velocity UMETA(DisplayName = "Velocity"),
  Health UMETA(DisplayName = "Health"),
  Power UMETA(DisplayName = "Power")
};

UENUM()
enum class ENodeState : uint8
{
  UnFocused UMETA(DisplayName = "UnFocused"),
  Focused UMETA(DisplayName = "Focused"),
  Selected UMETA(DisplayName = "Selected"),
  Acquired UMETA(DisplayName = "Acquired"),
  Locked UMETA(DisplayName = "Locked")
};

UCLASS()
class UTAD_UI_FPS_API USkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()
public:

  /**
   * It sets the skill tree parent and links the skill node parent.
   * @param _pSkillTree skill tree parent.
   * @param _pParent parent node for this skill node.
   */
  void SetupNode(USkillTreeWidget* _pSkillTree, USkillNodeWidget* _pParent);

  /**
   * It deselects the skill node.
   */
  void DeselectNode();

  /**
   * It sets the skill as acquired.
   */
  void AcquireSkill();

  /**
   * It returns the node ID. 
   */
  FName GetId();

  /**
   * It returns the prent ID.
   */
  FName GetParentId();

  /**
   * It returns the node state. 
   */
  ENodeState GetNodeState();

  /**
   * Delegate to notify when the child nodes should be locked or unlocked.
   */
  FOnLockNode OnLockNode;

protected:

  /**
   * Widget functions.
   */
  virtual void NativeConstruct() override;
  virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
  virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
  virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
  virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:

  /**
   * It updates the state according to the node state.
   */
  void UpdateVisualState();

  /**
   * It sets when the button is locked or unlocked.
   * @param _bIsLocked parameter to lock/unlock the node.
   */
  void SetNodeLocked(bool _bIsLocked);

  /**
   * It selects or deselect the focused skill.
   */
  void HandleSelectSkill();

  /**
   * It acquires the skill if the button is held.
   * @param _fDeltaTime time passed since the last tick.
   * @return true if the timer still alive. False when the time ends.
   */
  bool HandleAcquireSkill(float _fDeltaTime);

  /**
   * The reference to the tree skill parent.
   */
  UPROPERTY(meta = (DisplayName = "Skill Tree"))
  USkillTreeWidget* m_pSkillTreeWidget;

  /**
   * It is the parent ID of this skill node.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Node", meta = (DisplayName = "Parent ID"))
  FName m_sParentId;

  /**
   * It is the ID of this skill node.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Node", meta = (DisplayName = "ID"))
  FName m_sId;

  /**
   * The panel of the widget.
   */
  UPROPERTY(meta = (DisplayName = "Panel", BindWidget))
  class UCanvasPanel* m_pPanel;

  /**
   * It is the node image in the widget.
   */
  UPROPERTY(meta = (DisplayName = "Image", BindWidget))
  UImage* m_pSkillImage;

  /**
   * It is the node skill type image in the widget.
   */
  UPROPERTY(meta = (DisplayName = "Skill Type Image", BindWidget))
  UImage* m_pSkillTypeImage;

  /**
   * It is the texture to draw in the skill type Image.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Node", meta = (DisplayName = "Skill Type Texture"))
  UTexture2D* m_pSkillTypeTexture;

  /**
   * It is the text for the skill cost.
   */
  UPROPERTY(meta = (DisplayName = "Point cost Text", BindWidget))
  class UTextBlock* m_pSkillCostText;

  /**
   * It is the round progress bar for the hel mode.
   */
  UPROPERTY(meta = (DisplayName = "Round Porgress Bar", BindWidget))
  UImage* m_pRoundProgressBar;

  /**
   * It is the cost in points to acquire the skill.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Node", meta = (DisplayName = "Type"))
  ESkillType m_tType;

  /**
   * It is the node state.
   */
  UPROPERTY(meta = (DisplayName = "State"))
  ENodeState m_tState;

  /**
   * It is the cost in points to adquire the skill.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Node", meta = (DisplayName = "Points cost"))
  int32 m_iSkillCost;

  /**
   * Timer to acquire the skill.
   */
  FTSTicker::FDelegateHandle m_tHeldTickerHandle;

  /**
   * Time passed with the button pressed.
   */
  double m_dMouseDownTime;

  /**
   * True if the button is held.
   */
  bool m_bIsMouseHeld;
};