#include "SkillBranchWidget.h"
#include <Components/CanvasPanel.h>
#include "SkillTreeWidget.h"
#include "SkillNodeWidget.h"

void USkillBranchWidget::NativeConstruct()
{
  Super::NativeConstruct();
}

void USkillBranchWidget::SetupBranch(USkillTreeWidget* _pSkillTree)
{
  TArray<UWidget*> lWidget = m_pPanel->GetAllChildren();
  USkillNodeWidget* pSkillNode = nullptr;
  for (UWidget* pWidget : lWidget)
  {
    pSkillNode = Cast<USkillNodeWidget>(pWidget);
    if (pSkillNode != nullptr)
    {
      m_mSkillNode.Add(pSkillNode->GetId(), pSkillNode);
    }
  }
  pSkillNode = nullptr;
  for (const TPair<FName, USkillNodeWidget*>& oPair : m_mSkillNode)
  {
    pSkillNode = oPair.Value;
    if (pSkillNode->GetParentId().IsNone())
    {
      pSkillNode->SetupNode(_pSkillTree, nullptr);
    } else if (USkillNodeWidget** ParentNode = m_mSkillNode.Find(pSkillNode->GetParentId()))
    {
      pSkillNode->SetupNode(_pSkillTree, *ParentNode);
    }
  }
}

void USkillBranchWidget::ConfirmSkills(bool _bAcquire)
{
  USkillNodeWidget* pSkillNode = nullptr;
  for (const TPair<FName, USkillNodeWidget*>& oPair : m_mSkillNode)
  {
    pSkillNode = oPair.Value;
    if (pSkillNode->GetNodeState() == ENodeState::Selected)
    {
      if (_bAcquire)
      {
        pSkillNode->AcquireSkill();
      }
      else
      {
        pSkillNode->DeselectNode();
      }
    }
  }
}