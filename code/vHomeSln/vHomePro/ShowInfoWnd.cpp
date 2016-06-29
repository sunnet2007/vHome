#include "ShowInfoWnd.h"
#include "StringUtil.h"
#include "TeacherLogic.h"
#include "ResMsg.h"

CShowInfoWnd::CShowInfoWnd(void)
{

}

CShowInfoWnd::CShowInfoWnd(int nSelectedTab)
{
	m_nSelectedTab = nSelectedTab;
}

CShowInfoWnd::~CShowInfoWnd(void)
{
}

LPCTSTR CShowInfoWnd::GetWindowClassName() const
{
	return _T("ShowInfoWindow");
}
CDuiString CShowInfoWnd::GetSkinFile()
{
	return _T("ShowInfoWnd.xml");
}
CDuiString CShowInfoWnd::GetSkinFolder()
{
	return _T("");
}

void CShowInfoWnd::OnPrepare(TNotifyUI& msg)
{
	CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
	pTabControl->SelectItem(m_nSelectedTab);
	
	CLabelUI* pLabelIcon = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelIcon")));
	CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelName"))); 
	//CLabelUI* pLabelLevel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelLevel")));
	CLabelUI* pLabelNum = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelNum"))); 
	if (pLabelIcon)
	{
		CDuiString strImg;
		strImg.Format(_T("teacherimage_%d.png"), m_teacher.GetUid());
		pLabelIcon->SetBkImage(strImg);
	}
	if (pLabelName)
	{
		string strName = m_teacher.GetName();
		TCHAR tcName[48];
		memset(tcName, 0, 48);
		CharToTchar(strName.c_str(), tcName);
		pLabelName->SetText(tcName);
	}
	if (pLabelNum)
	{
		CDuiString strName;
		strName.Format(_T("%d"), m_teacher.GetNum());
		pLabelNum->SetText(strName);
	}
	if (m_nSelectedTab == 0)
	{
		CLabelUI* pLabelName1 = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelName1"))); 
		CLabelUI* pLabelSchool = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelSchool"))); 
		CLabelUI* pLabelLocation = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelLocation"))); 
		CLabelUI* pLabelEdu = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelEdu")));

		if (pLabelName1)
		{
			string strName = m_teacher.GetName();
			TCHAR tcName[48];
			memset(tcName, 0, 48);
			CharToTchar(strName.c_str(), tcName);
			pLabelName1->SetText(tcName);
		}
		if (pLabelSchool)
		{
			string strSchool = m_teacher.GetSchool();
			TCHAR tcSchool[48];
			memset(tcSchool, 0, 48);
			CharToTchar(strSchool.c_str(), tcSchool);
			pLabelSchool->SetText(tcSchool);
		}
	}

}

void CShowInfoWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			this->Close();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnCancelCare"))
		{
			OnCancelCare();
			return;
		}
	}
	else if (msg.sType == _T("selectchanged"))
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
		if (msg.pSender->GetName() == _T("opPerInfo"))
		{
			pTabControl->SelectItem(0);
			return;
		}
		else if (msg.pSender->GetName() == _T("opArticle"))
		{
			pTabControl->SelectItem(1);
			return;
		}
		else if (msg.pSender->GetName() == _T("opTalk"))
		{
			pTabControl->SelectItem(2);
			return;
		}
	}
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
	}
	__super::Notify(msg);
}

void CShowInfoWnd::SetTeacher(const CTeacher& teacher)
{
	m_teacher = teacher;
}
CTeacher CShowInfoWnd::GetTeacher()
{
	return m_teacher;
}

// È¡Ïû¹Ø×¢
void CShowInfoWnd::OnCancelCare()
{
	CTeacherLogic teacherLogic;
	string strToken;
	CResMsg msg;
	teacherLogic.DelAttention(m_teacher.GetUid(), strToken, msg);
}