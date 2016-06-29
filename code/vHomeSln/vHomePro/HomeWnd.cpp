#include "HomeWnd.h"
#include "SearchWnd.h"
#include "ShowInfoWnd.h"
#include "StringUtil.h"
#include "ExchangeLogic.h"
#include "CareListUI.h"
#include "Teacher.h"
#include "TeacherLogic.h"


CHomeWnd::CHomeWnd(void)
{
}


CHomeWnd::~CHomeWnd(void)
{
}

LPCTSTR CHomeWnd::GetWindowClassName() const
{
	return _T("HomeWindow");
}
CDuiString CHomeWnd::GetSkinFile()
{
	return _T("HomeWnd.xml");
}
CDuiString CHomeWnd::GetSkinFolder()
{
	return _T("");
}

CControlUI* CHomeWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("AnayList")) == 0) // 财经分析
	{
		CAnayListUI* pList= new CAnayListUI(m_PaintManager);
		return pList;
	}
	else if (_tcsicmp(pstrClass, _T("MsgList")) == 0) // 消息列表
	{
		return NULL;
	}
	else if (_tcsicmp(pstrClass, _T("CareList")) == 0) // 关注列表
	{
		CCareListUI* pCareList = new CCareListUI(m_PaintManager);
		return pCareList;
	}
	else if (_tcsicmp(pstrClass, _T("TeacherList"))== 0) // 推荐讲师
	{
		CCareListUI* pTeacherList = new CCareListUI(m_PaintManager);
		return pTeacherList;
	}
	else if (_tcsicmp(pstrClass, _T("Specialist")) == 0) // 专家会诊
	{
		return NULL;
	}
	return NULL;
}

void CHomeWnd::OnPrepare(TNotifyUI& msg)
{
	CButtonUI* btnIcon = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnIcon")));
	char cBkImage[32];
	memset(cBkImage, 0, 32);
	sprintf_s(cBkImage, 32, "icon_%d.png", m_user.GetUid());

	TCHAR tcBkImage[512];
	memset(tcBkImage, 0, 512);
	CharToTchar(cBkImage, tcBkImage);

	btnIcon->SetBkImage(tcBkImage);

	CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelName")));
	string strName = m_user.GetNicName();
	TCHAR tcName[48];
	memset(tcName, 0, 48);
	CharToTchar(strName.c_str(), tcName);
	pLabelName->SetText(tcName);

	UpdateAnayList();
	UpdateMsgList();
	UpdateCareList();
	UpdateTeacherList();
	UpdateSpecialist();
}

void CHomeWnd::InitWindow()
{
	//::MessageBox(NULL, _T("hello"), _T("msg"),0);
}

void CHomeWnd::RemoveControl( CControlUI *pContrl )
{

}
void CHomeWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			//this->Close();
			::PostQuitMessage(0);
			return;
		}
		else if (msg.pSender->GetName() == _T("btnMin"))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
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
		if (msg.pSender->GetName() == _T("optionFinacialAnay"))
		{
			//UpdateAnayList();
			pTabControl->SelectItem(0);
			return;
		}
		else if (msg.pSender->GetName() == _T("optionMsgList"))
		{
			pTabControl->SelectItem(1);
			return;
		}
		else if (msg.pSender->GetName() == _T("optionMyCare"))
		{
			//UpdateCareList();
			pTabControl->SelectItem(2);
			return;
		}
		else if (msg.pSender->GetName() == _T("optionReTeacher"))
		{
			pTabControl->SelectItem(3);
			return;
		}
		else if (msg.pSender->GetName() == _T("optionConsult"))
		{
			pTabControl->SelectItem(4);
			return;
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMACTIVATE)
	{
		CDuiString name= msg.pSender->GetName();
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
		if (pTabControl != NULL)
		{	 
			int nCurSel = pTabControl->GetCurSel();
			switch (nCurSel)
			{
			case 0: // 财经分析
				{
					// 显示交易所所有教师
					CAnayListUI* pAnayList = static_cast<CAnayListUI*>(m_PaintManager.FindControl(_T("lstAnay")));
					int nIndex = pAnayList->GetItemIndex(msg.pSender);
					CListContainerElementUI* pListElement= static_cast<CListContainerElementUI*>(pAnayList->GetItemAt(nIndex));
					CDuiString strId = pListElement->GetUserData();
					string strID = StringFromLPCTSTR(strId);
					int nId = atoi(strID.c_str());
					ShowExTeachersInfo(nId);
					//ShowSearchWnd();
				}
				break;
			case 1: // 消息列表
				break;
			case 2: // 我的关注
				break;
			case 3:  // 推荐讲师
				break;
			case 4: // 专家会诊
				break;
			default:
				break;
			}
		}
		//ShowInfoWnd();

		return;
	}
	else if (msg.sType == _T("menu")) // 菜单
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
		if (pTabControl != NULL)
		{	 
			int nCurSel = pTabControl->GetCurSel();
			switch (nCurSel)
			{
			case 0: // 财经分析
				break;
			case 1: // 消息列表
				break;
			case 2: // 我的关注
				{
					CDuiString strName = msg.pSender->GetName();
					POINT pt;
					pt.x = msg.ptMouse.x;
					pt.y = msg.ptMouse.y;
					CDuiString name = msg.pSender->GetName();
					ClientToScreen(*this, &pt);
					CCareListUI* pCareList = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCare")));
					int nIndex = pCareList->GetItemIndex(msg.pSender);
					CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("MenuWnd.xml"),pt, &m_PaintManager);
					pMenu->ShowWindow();
				}
				break;
			case 3:  // 推荐讲师
				break;
			case 4: // 专家会诊
				break;
			default:
				break;
			}
		}
	}

	__super::Notify(msg);;
}

LRESULT CHomeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LRESULT lRes = 0;
	//BOOL bHandled = TRUE;
	//switch (uMsg)
	//{
	//case WM_RBUTTONUP:
	//	{
	//		CPoint pt(0,0);
	//		GetCursorPos(&pt);
	//		//CMenuWnd* pMenu = new CMenuWnd;
	//		//pMenu->Create(*this, _T("MenuWnd"),UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, LOWORD(lParam), HIWORD(wParam));
	//		CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("MenuWnd.xml"),pt, &m_PaintManager);
	//		pMenu->ShowWindow();
	//	}
	//	break;
	//case WM_LBUTTONDBLCLK:
	//	//ShowSearchWnd();
	//	bHandled=FALSE;
	//	break;
	//default:
	//	bHandled = FALSE;
	//	break;
	//}
	//if(bHandled || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	//	return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CHomeWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		int nIndex = (int)lParam;
		CDuiString* pstrMenuName = (CDuiString*)wParam;
		CCareListUI* pCareList = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCare")));
		//int nIndex = pCareList->GetItemIndex(msg.pSender);
		CListContainerElementUI* pListElement= static_cast<CListContainerElementUI*>(pCareList->GetItemAt(nIndex));
		CDuiString strId = pListElement->GetUserData();
		string strID = StringFromLPCTSTR(strId);
		int nTid = atoi(strID.c_str());
		CTeacher teacher = m_vecTeacher[nIndex];
		//teacher.SetUid(nTid);

		if (*pstrMenuName == _T("menuCancel")) // 取消关注
		{
			//ShowInfoWnd(0, teacher);
			OnCancelCare(nTid);	//取消关注
		}
		else if (*pstrMenuName == _T("menuView")) // 查看资料
		{
			ShowInfoWnd(0, teacher);
		}
		else if (*pstrMenuName == _T("menuSend")) // 交谈
		{
			ShowInfoWnd(2, teacher);
		}
	}
	bHandled = FALSE;
	return 0;
}

// 更新财经分析列表
void CHomeWnd::UpdateAnayList()
{
	CAnayListUI* pAnayListWnd = static_cast<CAnayListUI*>(m_PaintManager.FindControl(_T("lstAnay")));
	if (pAnayListWnd != NULL)
	{
		if (pAnayListWnd->GetCount() > 0)
		{
			pAnayListWnd->RemoveAll();
		}

		CExchangeLogic exchangeLogic;
		vector<CExchange> vecExchange;
		CResMsg msg;
		int nRes = exchangeLogic.GetAllExchanges(vecExchange, msg);


		int nSize = vecExchange.size();
		for (int i = 0; i < nSize; i++)
		{
			CExchange exchange = vecExchange[i];
			pAnayListWnd->AddItem(exchange);
		}
	}
}

// 更新消息列表
void CHomeWnd::UpdateMsgList()
{

}

// 更新关注列表
void CHomeWnd::UpdateCareList()
{
	CCareListUI* pCareListUI = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCare")));
	if (pCareListUI != NULL)
	{
		if (m_vecTeacher.size() > 0)
		{
			m_vecTeacher.clear();
		}

		if (pCareListUI->GetCount() > 0)
		{
			pCareListUI->RemoveAll();
		}

		CTeacherLogic teacherLogic;
		//vector<CTeacher> vecTeacher;
		CResMsg msg;
		teacherLogic.GetCareTeacher(m_user.GetToken(), m_vecTeacher, msg);


		int nSize = m_vecTeacher.size();
		for (int i = 0; i < nSize; i++)
		{
			//CExchange exchange = vecExchange[i];
			//CTeacher teacher vecTeacher;
			pCareListUI->AddItem(m_vecTeacher[i]);
		}
	}
}

// 更新推荐讲师列表
void CHomeWnd::UpdateTeacherList()
{
	CCareListUI* pCareListUI = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstTeacher")));
	if (pCareListUI != NULL)
	{
		if (m_vecTeacher.size() > 0)
		{
			m_vecTeacher.clear();
		}

		if (pCareListUI->GetCount() > 0)
		{
			pCareListUI->RemoveAll();
		}

		CTeacherLogic teacherLogic;
		//vector<CTeacher> vecTeacher;
		CResMsg msg;
		int nRes = teacherLogic.GetTeachers(m_user.GetToken(), m_vecTeacher, msg);

		int nSize = m_vecTeacher.size();
		for (int i = 0; i < nSize; i++)
		{
			//CExchange exchange = vecExchange[i];
			//CTeacher teacher vecTeacher;
			pCareListUI->AddItem(m_vecTeacher[i]);
		}
	}
}
void CHomeWnd::UpdateSpecialist()
{

}

// 取消关注
void CHomeWnd::OnCancelCare(int nTid)
{
	CTeacherLogic teacherLogic;
	CResMsg msg;
	teacherLogic.DelAttention(nTid, m_user.GetToken(), msg);
	int nSize = m_vecTeacher.size();
	for (int i = 0; i < nSize; i++)
	{
		CTeacher tea = m_vecTeacher[i];
		int nReId = tea.GetUid();
		//int nDestId = teacher.GetUid();
		if (nReId == nTid)
		{
			std::vector<CTeacher>::iterator it = m_vecTeacher.begin()+i;
			m_vecTeacher.erase(it);
		}
	}
	UpdateCareList();
}

// 显示交易所所有讲师信息
void CHomeWnd::ShowExTeachersInfo(int nExid)
{
	CSearchWnd* pSearchWnd = new CSearchWnd;
	pSearchWnd->SetExid(nExid);
	pSearchWnd->SetUser(m_user);
	pSearchWnd->Create(*this, _T("SearchWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE,100,100,800,600);
	pSearchWnd->ShowWindow();
	int nScreenWidth= GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight= GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*pSearchWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = (nScreenWidth-nWndWidth)/2;
	int y = (nScreenHeight - nWndHeight)/2;
	::MoveWindow(*pSearchWnd, x, y, nWndWidth, nWndHeight,TRUE);
}

// 显示信息窗口
void CHomeWnd::ShowInfoWnd(int nSelected, const CTeacher& teacher)
{
	switch (nSelected)
	{
	case 0:	// 查看资料
		break;
	case 2:	// 发送消息
	default:
		break;
	} 


	CShowInfoWnd* pInfoWnd = new CShowInfoWnd(nSelected);
	pInfoWnd->Create(*this, _T("ShowInfoWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pInfoWnd->SetTeacher(teacher);
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
}

CUser& CHomeWnd::GetUser()
{
	return m_user;
}
void CHomeWnd::SetUser(CUser& user)
{
	m_user = user;
}

