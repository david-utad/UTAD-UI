#include "SkillTreeWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundBase.h>
#include "SkillBranchWidget.h"

void USkillTreeWidget::NativeConstruct()
{
  Super::NativeConstruct();
  m_pConfirmButton->OnClicked.AddDynamic(this, &USkillTreeWidget::ConfirmSkills);
  m_pCloseButton->OnClicked.AddDynamic(this, &USkillTreeWidget::Hide);
  m_iAvailablePoints =  5;
  TArray<UWidget*> lWidget = m_pPanel->GetAllChildren();
  USkillBranchWidget* pSkillBranch = nullptr;
  for (UWidget* pWidget : lWidget)
  {
    pSkillBranch = Cast<USkillBranchWidget>(pWidget);
    if (pSkillBranch != nullptr)
    {
      pSkillBranch->SetupBranch(this);
      m_lSkillBranch.Add(pSkillBranch);
    }
  }
  SetVisibility(ESlateVisibility::Hidden);
}

bool USkillTreeWidget::IsVisible()
{
  return (GetVisibility() == ESlateVisibility::Visible);
}

void USkillTreeWidget::Show()
{
  APlayerController* m_pPlayerController = GetOwningPlayer<APlayerController>();
  if (m_pPlayerController != nullptr)
  {
    m_pPlayerController->SetPause(true);
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    InputMode.SetWidgetToFocus(this->TakeWidget());
    m_pPlayerController->SetInputMode(InputMode);
    m_pPlayerController->bShowMouseCursor = true;
  }
  if (m_pFeedbackText != nullptr)
  {
    m_pFeedbackText->SetVisibility(ESlateVisibility::Collapsed);
  }
  ShowAvailablePoints();
  PlaySound(ESoundType::OpenClose);
  SetVisibility(ESlateVisibility::Visible);
}

void USkillTreeWidget::Hide()
{
  for (USkillBranchWidget* pSkillBranch : m_lSkillBranch)
  {
    if (pSkillBranch != nullptr)
    {
      pSkillBranch->DeselectNodes();
    }
  }

  APlayerController* m_pPlayerController = GetOwningPlayer<APlayerController>();
  if (m_pPlayerController != nullptr)
  {
    m_pPlayerController->SetInputMode(FInputModeGameOnly());
    m_pPlayerController->bShowMouseCursor = false;
    m_pPlayerController->SetPause(false);
  }
  PlaySound(ESoundType::OpenClose);
  SetVisibility(ESlateVisibility::Hidden);
}

bool USkillTreeWidget::IsThereEnoughtPoints(int32 _iCost)
{
  return (m_iAvailablePoints >= _iCost);
}

void USkillTreeWidget::ModifyAvailablePoints(int32 _iCost)
{
  m_iAvailablePoints += _iCost;
  ShowAvailablePoints();
}

void USkillTreeWidget::ShowAvailablePoints()
{
  if (m_pPointsValueText != nullptr)
  {
    m_pPointsValueText->SetText(FText::AsNumber(m_iAvailablePoints));
  }
}

void USkillTreeWidget::ConfirmSkills()
{
  PlaySound(ESoundType::Confirm);
  for (USkillBranchWidget* pSkillBranch : m_lSkillBranch)
  {
    if (pSkillBranch != nullptr)
    {
      pSkillBranch->AcquireSkills();
    }
  }
}

void USkillTreeWidget::SetFeedbackText(FText _sFeedbackText)
{
  if (m_pFeedbackText != nullptr)
  {
    m_pFeedbackText->SetText(_sFeedbackText);
    m_pFeedbackText->SetVisibility(ESlateVisibility::Visible);

    if (m_tFeedbackTickerHandle.IsValid())
    {
      FTSTicker::GetCoreTicker().RemoveTicker(m_tFeedbackTickerHandle);
      m_tFeedbackTickerHandle.Reset();
    }

    m_tFeedbackTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float _fDeltaTime)
      {
        HideFeedbackText();
        m_tFeedbackTickerHandle.Reset();
        return false;
      }), 3.f);

  }
}

void USkillTreeWidget::HideFeedbackText()
{
  if (m_pFeedbackText != nullptr)
  {
    m_pFeedbackText->SetText(FText::FromString(""));
    m_pFeedbackText->SetVisibility(ESlateVisibility::Collapsed);
  }
}

void USkillTreeWidget::PlaySound(ESoundType _eType)
{
  if (USoundBase** pSound = m_mSound.Find(_eType))
  {
    if (*pSound != nullptr)
    {
      UGameplayStatics::PlaySound2D(GetWorld(), *pSound);
    }
  }
}