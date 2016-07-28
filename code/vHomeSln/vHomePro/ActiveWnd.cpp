#include "ActiveWnd.h"
#include "ActivityListUI.h"
#include "ActivityLogic.h"
#include "ActivityInfoWnd.h"
#include "StringUtil.h"

CActiveWnd::CActiveWnd(CUser& user)
{
	m_user = user;
}


CActiveWnd::~CActiveWnd(void)
{
}

LPCTSTR CActiveWnd::GetWindowClassName() const
{
	return _T("ActiveWindow");
}
CDuiString CActiveWnd::GetSkinFile()
{
	return _T("ActiveWnd.xml");
}
CDuiString CActiveWnd::GetSkinFolder()
{
	return _T("");
}

CControlUI* CActiveWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("NewActive")) == 0) // 所有活动
	{
		CActivityListUI* pList= new CActivityListUI(m_PaintManager);
		return pList;
	}
	else if (_tcsicmp(pstrClass, _T("MyActive")) == 0)	// 我的活动
	{
		CActivityListUI* pList= new CActivityListUI(m_PaintManager);
		return pList;
	}
	return NULL;
}

void CActiveWnd::OnPrepare(TNotifyUI& msg)
{
	//UpdateNewActivityList();
	//UpdateMyActivityList();
	COptionUI* pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opNewActive")));
	pOption->Selected(true);

}

void CActiveWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			Close();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnView"))
		{
			CButtonUI* pButton = static_cast<CButtonUI*>(msg.pSender);
			CListContainerElementUI* pElement = static_cast<CListContainerElementUI*>(pButton->GetParent()->GetParent());
			CDuiString strId = pElement->GetUserData();
			string strID = StringFromLPCTSTR(strId);
			int nId = atoi(strID.c_str());
			OnBtnView(nId);
			return;
		}
	}
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
		return;
	}
	else if (msg.sType == _T("selectchanged"))
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
		if (msg.pSender->GetName() == _T("opNewActive"))
		{
			pTabControl->SelectItem(0);
			UpdateNewActivityList();
			return;
		}
		else if (msg.pSender->GetName() == _T("opMyActive"))
		{	
			pTabControl->SelectItem(1);
			UpdateMyActivityList();
			return;
		}
	}
	return __super::Notify(msg);
}

void CActiveWnd::UpdateNewActivityList()
{
	CActivityListUI* pNewActivityListUI = static_cast<CActivityListUI*>(m_PaintManager.FindControl(_T("lstNewActive")));
	if (pNewActivityListUI != NULL)
	{
		if (m_vecNewActivity.size() > 0)
		{
			m_vecNewActivity.clear();
		}
		if (pNewActivityListUI->GetCount() > 0)
		{
			pNewActivityListUI->RemoveAll();
		}

		CActivityLogic activityLogic;
		string strInfo;
		int nPage = 0;
		int nPageNum = 10;
		int nRes = activityLogic.GetAllActivity(nPage,nPageNum, m_vecNewActivity, strInfo);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}
		int nSize = m_vecNewActivity.size();
		for (int i = 0; i < nSize; i++)
		{
			CActivity activity = m_vecNewActivity[i];
			
			pNewActivityListUI->AddItem(activity);
		}
	}
}
void CActiveWnd::UpdateMyActivityList()
{
	CActivityListUI* pNewActivityListUI = static_cast<CActivityListUI*>(m_PaintManager.FindControl(_T("lstMyActive")));
	if (pNewActivityListUI != NULL)
	{
		if (m_vecMyActivity.size() > 0)
		{
			m_vecMyActivity.clear();
		}
		if (pNewActivityListUI->GetCount() > 0)
		{
			pNewActivityListUI->RemoveAll();
		}

		CActivityLogic activityLogic;
		string strInfo;
		int nPage = 0;
		int nPageNum = 10;
		int nRes = activityLogic.GetMyActivity(nPage, nPageNum, m_vecMyActivity, m_user.GetToken(), strInfo);
		if(nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}
		int nSize = m_vecMyActivity.size();
		for (int i = 0; i < nSize; i++)
		{
			CActivity activity = m_vecMyActivity[i];
			pNewActivityListUI->AddItem(activity);
		}
	}
}

void CActiveWnd::OnBtnView(int nId)
{
	CActivityInfoWnd* pInfoWnd = new CActivityInfoWnd(nId, m_user);
	pInfoWnd->Create(*this, _T("ActivityInfoWnd"), UI_WNDSTYLE_DIALOG|UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE);
	pInfoWnd->ShowWindow();
	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*pInfoWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = (nScreenWidth-nWndWidth)/2;
	int y = (nScreenHeight - nWndHeight)/2;
	::MoveWindow(*pInfoWnd, x, y, nWndWidth, nWndHeight,TRUE);
	return;
}

void CActiveWnd::SetUser(CUser& user)
{
	m_user = user;
}