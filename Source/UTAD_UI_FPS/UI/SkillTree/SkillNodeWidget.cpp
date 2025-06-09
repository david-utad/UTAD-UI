#include "SkillNodeWidget.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "SkillTreeWidget.h"

void USkillNodeWidget::NativeConstruct()
{
  Super::NativeConstruct();
  m_tState = ENodeState::UnFocused;
  if (m_pSkillCostText != nullptr)
  {
    m_pSkillCostText->SetText(FText::AsNumber(m_iSkillCost));
  }
}

void USkillNodeWidget::SetupNode(USkillTreeWidget* _pSkillTree, USkillNodeWidget* _pParent)
{
  m_pSkillTreeWidget = _pSkillTree;
  if (_pParent != nullptr)
  {
    m_tState = ENodeState::Locked;
    _pParent->OnLockNode.AddUObject(this, &USkillNodeWidget::SetNodeLocked);
  }
  UpdateVisualState();
}

void USkillNodeWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
  Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
  if (m_tState == ENodeState::UnFocused)
  {
    m_tState = ENodeState::Focused;
    UpdateVisualState();
    if (m_pSkillTreeWidget != nullptr)
    {
      m_pSkillTreeWidget->PlaySound(ESoundType::Focus);
    }
  }
}

void USkillNodeWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
  Super::NativeOnMouseLeave(InMouseEvent);
  if (m_tState == ENodeState::Focused)
  {
    m_tState = ENodeState::UnFocused;
    UpdateVisualState();
  }
}

FReply USkillNodeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
  if ((m_tState != ENodeState::Locked) && (m_tState != ENodeState::Acquired))
  {
    if (m_tState == ENodeState::Selected)
    {
      m_tState = ENodeState::Focused;
      if (m_pSkillTreeWidget != nullptr)
      {
        m_pSkillTreeWidget->ModifyAvailablePoints(m_iSkillCost);
        m_pSkillTreeWidget->PlaySound(ESoundType::Deselect);
      }
    }
    else if (m_pSkillTreeWidget->IsThereEnoughtPoints(m_iSkillCost))
    {
      m_tState = ENodeState::Selected;
      if (m_pSkillTreeWidget != nullptr)
      {
        m_pSkillTreeWidget->ModifyAvailablePoints(-m_iSkillCost);
        m_pSkillTreeWidget->PlaySound(ESoundType::Select);
      }
    }
    else if (m_pSkillTreeWidget != nullptr)
    {
      m_pSkillTreeWidget->SetFeedbackText(FText::FromString("There isn't enought points to acquire the skill."));
      m_pSkillTreeWidget->PlaySound(ESoundType::Error);
    }
    UpdateVisualState();
    OnLockNode.Broadcast(m_tState != ENodeState::Selected);
  }
  else
  {
    if (m_pSkillTreeWidget != nullptr)
    {
      if (m_tState == ENodeState::Locked)
      {
        m_pSkillTreeWidget->SetFeedbackText(FText::FromString("This skill is locked. Unlock the parent skill to make it available."));
        m_pSkillTreeWidget->PlaySound(ESoundType::Error);
      }
      else if (m_tState == ENodeState::Acquired)
      {
        m_pSkillTreeWidget->SetFeedbackText(FText::FromString("You have already acquired this skill."));
        m_pSkillTreeWidget->PlaySound(ESoundType::Error);
      }
    }
  }
  return FReply::Handled();
}

void USkillNodeWidget::DeselectNode()
{
  m_tState = ENodeState::UnFocused;
  m_pSkillTreeWidget->ModifyAvailablePoints(m_iSkillCost);
  UpdateVisualState();
  OnLockNode.Broadcast(true);
}

void USkillNodeWidget::AcquireSkill()
{
  m_tState = ENodeState::Acquired;
  UpdateVisualState();
}

FName USkillNodeWidget::GetId()
{
  return m_sId;
}

FName USkillNodeWidget::GetParentId()
{
  return m_sParentId;
}

ENodeState USkillNodeWidget::GetNodeState()
{
  return m_tState;
}

void USkillNodeWidget::UpdateVisualState()
{
  if (m_pSkillImage != nullptr)
  {
    FLinearColor tColor;

    switch (m_tState)
    {
      case ENodeState::Focused:
        {
          tColor = FLinearColor(1.0f, 1.0f, 0.5f, 1.0f); // Yellow
        }
        break;
      case ENodeState::Selected:
        {
          tColor = FLinearColor(0.f, 0.5f, 1.0f, 1.0f); // Blue
        }
        break;
      case ENodeState::Acquired:
        {
          tColor = FLinearColor(0.f, 1.0f, 0.5f, 1.0f); // Green
        }
        break;
      case ENodeState::Locked:
        {
          tColor = FLinearColor(0.25f, 0.25f, 0.25f, 1.0f); // Grey
        }
        break;
      default:
        {
        tColor = FLinearColor::White;
        }
    }
    m_pSkillImage->SetColorAndOpacity(tColor);
    
    if (m_pSkillCostText != nullptr)
    {
      if ((m_tState == ENodeState::Focused) || (m_tState == ENodeState::Selected))
      {
        m_pSkillCostText->SetVisibility(ESlateVisibility::Visible);
      }
      else
      {
        m_pSkillCostText->SetVisibility(ESlateVisibility::Collapsed);
      }
    }
  }
}

void USkillNodeWidget::SetNodeLocked(bool _bIsLocked)
{
  if (_bIsLocked && (m_tState == ENodeState::Selected))
  {
    m_pSkillTreeWidget->ModifyAvailablePoints(m_iSkillCost);
  }
  m_tState = _bIsLocked ? ENodeState::Locked : ENodeState::UnFocused;
  if (_bIsLocked)
  {
    OnLockNode.Broadcast(_bIsLocked);
  }
  UpdateVisualState();
}