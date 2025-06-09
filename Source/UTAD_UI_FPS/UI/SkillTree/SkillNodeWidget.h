/************************************************************************
 * @description: Skill node widget class to show in the skill tree screen.
 * @author: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillNodeWidget.generated.h"

class USkillTreeWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLockNode, bool /* _bLocked */);

UENUM()
enum class ESkillType : uint8
{
  None UMETA(DisplayName = "None"),
  Movement UMETA(DisplayName = "Movement"),
  Health UMETA(DisplayName = "Health"),
  Damage UMETA(DisplayName = "Damage")
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
   * The reference to the tree skill parent.
   */
  UPROPERTY(VisibleAnywhere, Category = "Skill Node", meta = (DisplayName = "Skill Tree"))
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
  UPROPERTY(EditInstanceOnly, Category = "Skill Node", meta = (DisplayName = "Panel", BindWidget))
  class UCanvasPanel* m_pPanel;

  /**
   * It is the node image in the widget.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Node", meta = (DisplayName = "Image", BindWidget))
  class UImage* m_pSkillImage;

  /**
   * It is the text for the skill cost.
   */
  UPROPERTY(EditInstanceOnly, Category = "Skill Node", meta = (DisplayName = "Point cost Text", BindWidget))
  class UTextBlock* m_pSkillCostText;

  /**
   * It is the cost in points to acquire the skill.
   */
  UPROPERTY(EditAnywhere, Category = "Skill Node", meta = (DisplayName = "Type"))
  ESkillType m_tType;

  /**
   * It is the node state.
   */
  UPROPERTY(VisibleAnywhere, Category = "Skill Node", meta = (DisplayName = "State"))
  ENodeState m_tState;

  /**
   * It is the cost in points to adquire the skill.
   */
  UPROPERTY(EditAnywhere, Category = "Skill Node", meta = (DisplayName = "Points cost"))
  int32 m_iSkillCost;
};