#include "SkillNodeWidget.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "SkillTreeWidget.h"

void USkillNodeWidget::NativeConstruct()
{
  Super::NativeConstruct();
  m_tState = ENodeState::UnFocused;
  m_dMouseDownTime = 0.0;
  m_bIsMouseHeld = false;
  if (m_pSkillCostText != nullptr)
  {
    m_pSkillCostText->SetText(FText::AsNumber(m_iSkillCost));
  }
  if (m_pRoundProgressBar != nullptr)
  {
    m_pRoundProgressBar->SetVisibility(ESlateVisibility::Hidden);
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
  if ((m_pSkillTreeWidget != nullptr) && (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton))
  {
    if ((m_tState != ENodeState::Locked) && (m_tState != ENodeState::Acquired))
    {
      if (!m_pSkillTreeWidget->IsInHeldMode())
      {
        HandleSelectSkill();
      }
      else
      {
        if (m_pSkillTreeWidget->IsThereEnoughtPoints(m_iSkillCost))
        {
          m_dMouseDownTime = FPlatformTime::Seconds();
          m_bIsMouseHeld = true;
          m_tHeldTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &USkillNodeWidget::HandleAcquireSkill));
        }
        else
        {
          m_pSkillTreeWidget->SetFeedbackText(FText::FromString("There isn't enought points to acquire the skill."));
          m_pSkillTreeWidget->PlaySound(ESoundType::Error);
        }
      }
    }
    else
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

FReply USkillNodeWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
  if ((InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) && m_pSkillTreeWidget->IsInHeldMode())
  {
    m_bIsMouseHeld = false;
    if (m_pRoundProgressBar != nullptr)
    {
      m_pRoundProgressBar->SetVisibility(ESlateVisibility::Hidden);
    }
    FTSTicker::GetCoreTicker().RemoveTicker(m_tHeldTickerHandle);
  }
  return FReply::Handled();
}

void USkillNodeWidget::HandleSelectSkill()
{
  if (m_pSkillTreeWidget != nullptr)
  {
    if (m_tState == ENodeState::Selected)
    {
      m_pSkillTreeWidget->ModifyAvailablePoints(m_iSkillCost);
      m_pSkillTreeWidget->PlaySound(ESoundType::Deselect);
    }
    else if (m_pSkillTreeWidget->IsThereEnoughtPoints(m_iSkillCost))
    {
      m_pSkillTreeWidget->ModifyAvailablePoints(-m_iSkillCost);
      m_pSkillTreeWidget->PlaySound(ESoundType::Select);
    }
    else
    {
      m_pSkillTreeWidget->SetFeedbackText(FText::FromString("There isn't enought points to acquire the skill."));
      m_pSkillTreeWidget->PlaySound(ESoundType::Error);
    }
  }
  m_tState = (m_tState == ENodeState::Selected) ? ENodeState::Focused : ENodeState::Selected;
  UpdateVisualState();
  OnLockNode.Broadcast(m_tState != ENodeState::Selected);
}

bool USkillNodeWidget::HandleAcquireSkill(float _fDeltaTime)
{
  bool bReturn = false;
  if (m_bIsMouseHeld && (m_pSkillTreeWidget != nullptr))
  {
    double dCurrentTime = FPlatformTime::Seconds();
    if ((m_pRoundProgressBar != nullptr) && (m_pSkillTreeWidget->GetHeldTime() > 0))
    {
      float fPercent = (dCurrentTime - m_dMouseDownTime) / m_pSkillTreeWidget->GetHeldTime();
      m_pRoundProgressBar->SetVisibility(ESlateVisibility::Visible);
      m_pRoundProgressBar->GetDynamicMaterial()->SetScalarParameterValue("Percent", fPercent);
    }
    if ((dCurrentTime - m_dMouseDownTime) >= m_pSkillTreeWidget->GetHeldTime())
    {
      m_pSkillTreeWidget->ModifyAvailablePoints(-m_iSkillCost);
      m_pSkillTreeWidget->PlaySound(ESoundType::Confirm);
      AcquireSkill();
      OnLockNode.Broadcast(false);
      m_bIsMouseHeld = false;
      m_pRoundProgressBar->SetVisibility(ESlateVisibility::Hidden);
      FTSTicker::GetCoreTicker().RemoveTicker(m_tHeldTickerHandle);
    }
    else
    {
      bReturn = true;
    }
  }
  return bReturn;
}

void USkillNodeWidget::DeselectNode()
{
  m_tState = ENodeState::UnFocused;
  if (m_pSkillTreeWidget != nullptr)
  {
    m_pSkillTreeWidget->ModifyAvailablePoints(m_iSkillCost);
  }
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
  if (_bIsLocked)
  {
    if ((m_tState == ENodeState::Selected) && (m_pSkillTreeWidget != nullptr))
    {
      m_pSkillTreeWidget->ModifyAvailablePoints(m_iSkillCost);
    }
    m_tState = ENodeState::Locked;
    OnLockNode.Broadcast(_bIsLocked);
  }
  else
  {
    m_tState = (m_tState == ENodeState::Selected) ? ENodeState::Selected : ENodeState::UnFocused;
  }
  UpdateVisualState();
}