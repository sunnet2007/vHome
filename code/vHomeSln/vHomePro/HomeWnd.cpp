#include "HomeWnd.h"
#include "SearchWnd.h"
#include "ShowInfoWnd.h"
#include "StringUtil.h"
#include "ExchangeLogic.h"
#include "CareListUI.h"
#include "Teacher.h"
#include "TeacherLogic.h"
#include "SettingWnd.h"
#include "ActiveWnd.h"
#include "ArtListUI.h"
#include "AppConfig.h"
#include "ArtDetailsWnd.h"
#include "AboutWnd.h"
#include "resource.h"
HANDLE hMutex;
// 线程函数
DWORD WINAPI FuncProc(LPVOID lpParamter)
{
	CHomeWnd* homeWnd = static_cast<CHomeWnd*>(lpParamter);
	homeWnd->UpdateAnayList();

	return 0;
}


CHomeWnd::CHomeWnd(CUser& user)
{
	m_nSelectItemIndex = 0;
	m_user = user;
	m_nType = m_user.GetType();
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

#define WM_TRAY (WM_USER + 100)  
#define WM_TASKBAR_CREATED RegisterWindowMessage(TEXT("TaskbarCreated"))  

#define APP_NAME    TEXT("V家金服")  
#define APP_TIP     TEXT("Win32 API 实现系统托盘程序") 

#define IDI_TRAY    105  
#define ID_SHOW     40001  
#define ID_EXIT     40002  

NOTIFYICONDATA nid;     //托盘属性  
HMENU hMenu;            //托盘菜单  
//实例化托盘  
void InitTray(HINSTANCE hInstance, HWND hWnd)  
{  
	nid.cbSize = sizeof(NOTIFYICONDATA);  
	nid.hWnd = hWnd;  
	nid.uID = IDI_ICON;  
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;  
	nid.uCallbackMessage = WM_TRAY;  
	nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));  
	lstrcpy(nid.szTip, APP_NAME);  

	hMenu = CreatePopupMenu();//生成托盘菜单  
	//为托盘菜单添加两个选项  
	AppendMenu(hMenu, MF_STRING, ID_SHOW, TEXT("显示主界面"));  
	AppendMenu(hMenu, MF_STRING, ID_EXIT, TEXT("退出"));  

	Shell_NotifyIcon(NIM_ADD, &nid);  
}  

//演示托盘气泡提醒  
void ShowTrayMsg()  
{  
	lstrcpy(nid.szInfoTitle, APP_NAME);  
	lstrcpy(nid.szInfo, TEXT("收到一条消息！"));  
	nid.uTimeout = 1000;  
	Shell_NotifyIcon(NIM_MODIFY, &nid);  
}  



LRESULT CHomeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)  
	{  
	case WM_TRAY:  
		switch(lParam)  
		{  
		case WM_RBUTTONDOWN:  
			{  
				//获取鼠标坐标  
				POINT pt; GetCursorPos(&pt);  

				//解决在菜单外单击左键菜单不消失的问题  
				SetForegroundWindow(this->GetHWND());  

				//使菜单某项变灰  
				//EnableMenuItem(hMenu, ID_SHOW, MF_GRAYED);      

				//显示并获取选中的菜单  
				int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, this->GetHWND(), NULL);  
				if(cmd == ID_SHOW)
					ShowWindow(true);
					//::MessageBox(this->GetHWND(), APP_TIP, APP_NAME, MB_OK);  
				if(cmd == ID_EXIT)   
					::PostMessage(this->GetHWND(), WM_DESTROY, NULL, NULL);  
			}  
			break;  
		case WM_LBUTTONDOWN:  
			//::MessageBox(this->GetHWND(), APP_TIP, APP_NAME, MB_OK);  
			ShowWindow(true);
			break;  
		case WM_LBUTTONDBLCLK:  
			break;  
		}  
		break;
	case WM_DESTROY:  
		//窗口销毁时删除托盘  
		Shell_NotifyIcon(NIM_DELETE, &nid);  
		PostQuitMessage(0);  
		break;  
	case WM_TIMER:  
		//ShowTrayMsg();  
		//KillTimer(this->GetHWND(), wParam);  
		break; 
	}  
	if (uMsg == WM_TASKBAR_CREATED)  
	{  
		//系统Explorer崩溃重启时，重新加载托盘  
		Shell_NotifyIcon(NIM_ADD, &nid);  
	}  
	return __super::HandleMessage(uMsg,wParam,lParam);
}

HRESULT CHomeWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_MINIMIZE)
	{
		::ShowWindow(*this, SW_HIDE);
		//ShowWindow(false);
		bHandled = true;
	}
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled) ;
}
CControlUI* CHomeWnd::CreateControl(LPCTSTR pstrClass)
{
	if (m_nType == 0)	// 学生
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
		else if (_tcsicmp(pstrClass, _T("Celelist")) == 0) // 名人榜单
		{
			CCareListUI* pCeleList = new CCareListUI(m_PaintManager);
			return pCeleList;
		}
	}
	else if (m_nType == 1)	// 讲师
	{
		if (_tcsicmp(pstrClass, _T("TeacherAnayList")) == 0) // 财经分析
		{
			CAnayListUI* pList= new CAnayListUI(m_PaintManager);
			return pList;
		}
		else if (_tcsicmp(pstrClass, _T("ArtList")) == 0) // 我的文章
		{
			CArtListUI* pArtList = new CArtListUI(m_PaintManager,0);
			return pArtList;
		}
	}

	return NULL;
}

void CHomeWnd::OnPrepare(TNotifyUI& msg)
{
	InitTray(m_PaintManager.GetInstance(), this->GetHWND());          //实例化托盘  
	SetTimer(this->GetHWND(), 3, 1000, NULL);      //定时发消息，演示气泡提示功能 
	CLabelUI* labelIcon = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelIcon")));
	if (labelIcon)
	{
		string strImg = m_user.GetIcon();
		wstring wstrImg = s2ws(strImg); 
		labelIcon->SetBkImage(wstrImg.c_str());
	}


	CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelName")));
	if (pLabelName)
	{
		string strName = m_user.GetNicName();
		wstring wstr = s2ws(strName);
		pLabelName->SetText(wstr.c_str());
	}


	if (m_nType == 0)	// 学员
	{
		CHorizontalLayoutUI* pHlTeacherOp = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hlTeacherOp")));
		if (pHlTeacherOp)
		{
			pHlTeacherOp->SetVisible(false);
		}

		CHorizontalLayoutUI* pHlTeacherTab = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hlTeacherTab")));
		if (pHlTeacherTab)
		{
			pHlTeacherTab->SetVisible(false);
		}

		COptionUI* pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optionFinacialAnay")));
		pOption->Selected(true);
	}
	else	// 讲师
	{
		CHorizontalLayoutUI* pHlStuOp = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hlStuOp")));
		if (pHlStuOp)
		{
			pHlStuOp->SetVisible(false);
		}

		CHorizontalLayoutUI* pHlStuTab = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hlStuTab")));
		if (pHlStuTab)
		{
			pHlStuTab->SetVisible(false);
		}

		CButtonUI* pBtnActivity = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnActive")));
		if (pBtnActivity)
		{
			pBtnActivity->SetVisible(false);
		}

		COptionUI* pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opTeacherAnay")));
		pOption->Selected(true);
	}
}

void CHomeWnd::InitWindow()
{
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
		else if (msg.pSender->GetName() == _T("btnSetting"))
		{
			OnSetting(0);
			return;
		}
		else if (msg.pSender->GetName() == _T("btnActive"))
		{
			OnBtnActive();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnAbout"))
		{
			OnBtnAbout();
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
		CTabLayoutUI* pTabControl = NULL;
		if (m_nType == 0)
		{
			pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
			if (msg.pSender->GetName() == _T("optionFinacialAnay"))
			{
				pTabControl->SelectItem(0);
				//if (m_vecExchange.size() <= 0)
				//{
					UpdateAnayList();
					//hMutex = CreateMutex(NULL, FALSE, NULL);
					//CGifAnimUI* gif = static_cast<CGifAnimUI*>(m_PaintManager.FindControl(_T("loading")));
					//HANDLE hThread = CreateThread(NULL, 0, FuncProc, this, 0, NULL);
					//CloseHandle(hThread);
					////CloseHandle(hMutex);
					//WaitForSingleObject(hThread, INFINITE);
					//if (gif)
					//{
					//	gif->SetVisible(false);
					//}
					

				//}
				return;
			}
			else if (msg.pSender->GetName() == _T("optionMyCare"))
			{
				pTabControl->SelectItem(1);
				UpdateCareList();
				return;
			}
			else if (msg.pSender->GetName() == _T("optionReTeacher"))
			{
				pTabControl->SelectItem(2);
				UpdateTeacherList();
				return;
			}
			else if (msg.pSender->GetName() == _T("optionCele"))
			{
				pTabControl->SelectItem(3);
				UpdateCeleList();
				return;
			}
		}
		else 
		{
			pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("TeacherTabs")));
			if (msg.pSender->GetName() == _T("opTeacherAnay"))
			{
				pTabControl->SelectItem(0);
				UpdateAnayList();
				return;
			}
			else if (msg.pSender->GetName() == _T("opMyArt"))
			{
				pTabControl->SelectItem(1);
				UpdateArtList();
				return;
			}
			else if (msg.pSender->GetName() == _T("opNewArt"))
			{
				ShowSettingWnd(2, "");
				return;
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMACTIVATE)
	{
		CTabLayoutUI* pTabControl = NULL;
		if (m_nType == 0)
		{
			pTabControl	= static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
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
						CExchange ex = m_vecExchange[nIndex];
						ShowExTeachersInfo(ex);
					}
					break;
				case 1: // 我的关注
					{
						CCareListUI* pCareList = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCare")));
						int nIndex = pCareList->GetItemIndex(msg.pSender);
						CTeacher teacher = m_vecCareTeacher[nIndex];
						ShowInfoWnd(0, teacher);
					}
					break;
				case 2:  // 推荐讲师
					{
						CCareListUI* pTeacherList = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstTeacher")));
						int nIndex = pTeacherList->GetItemIndex(msg.pSender);
						CTeacher teacher = m_vecTeacher[nIndex];
						ShowInfoWnd(0, teacher);
					}
					break;
				case 3: // 名人榜单
					{
						CCareListUI* pCeleList = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCele")));
						int nIndex = pCeleList->GetItemIndex(msg.pSender);
						CTeacher teacher = m_vecCeleTeacher[nIndex];

						ShowInfoWnd(0, teacher);
					}
					break;
				default:
					break;
				}
			}
		}
		else
		{
			pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("TeacherTabs")));
			int nCurSel = pTabControl->GetCurSel();
			switch(nCurSel)
			{
			case 0:		// 讲师财经分析，显示发布文章
				{
					CAnayListUI* pAnayList = static_cast<CAnayListUI*>(m_PaintManager.FindControl(_T("lstTeacherAnay")));
					int nIndex = pAnayList->GetItemIndex(msg.pSender);
					CListContainerElementUI* pListElement= static_cast<CListContainerElementUI*>(pAnayList->GetItemAt(nIndex));
					CLabelUI* pLabelName =static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("labelName")));
					pListElement->Select(true);
					string strName = StringFromLPCTSTR(pLabelName->GetText().GetData());
					ShowSettingWnd(2, strName);
					break;
				}
			case 1:
				{
					CArtListUI* pArtList = static_cast<CArtListUI*>(m_PaintManager.FindControl(_T("lstMyArt")));
					int nIndex = pArtList->GetItemIndex(msg.pSender);
					CListContainerElementUI* pListElement = static_cast<CListContainerElementUI*>(pArtList->GetItemAt(nIndex));
					CDuiString strId = pListElement->GetUserData();

					string id = StringFromLPCTSTR(strId.GetData());
					int nArtId = atoi(id.c_str());
					ShowArtDetailsWnd(nArtId);
					return;
				}
			default:
				break;
			}
		}

		return;
	}
	else if (msg.sType == _T("menu")) // 菜单
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
		if (pTabControl != NULL)
		{	 
			int nCurSel = pTabControl->GetCurSel();
			if (nCurSel == 1)
			{
				CCareListUI* pCareList = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCare")));
				m_nSelectItemIndex = pCareList->GetItemIndex(msg.pSender);
				m_strMenu = _T("MenuWnd2.xml");
				ShowMenuWnd( msg.ptMouse.x, msg.ptMouse.y);
			}		
		}
	}

	__super::Notify(msg);
}

void CHomeWnd::ShowMenuWnd(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	ClientToScreen(*this, &pt);
	CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, m_strMenu.GetData() ,pt, &m_PaintManager);
	pMenu->ShowWindow();

}

void CHomeWnd::OnBtnAbout()
{
	CAboutWnd* pAboutWnd = new CAboutWnd();
	pAboutWnd->Create(*this, _T("AboutWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	pAboutWnd->ShowWindow();
	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*pAboutWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = (nScreenWidth-nWndWidth)/2;
	int y = (nScreenHeight - nWndHeight)/2;
	::MoveWindow(*pAboutWnd, x, y, nWndWidth, nWndHeight,TRUE);
	return;
}

void CHomeWnd::ShowArtDetailsWnd(int nArtId)
{
	CArtDetailsWnd* artDetailWnd = new CArtDetailsWnd(nArtId, m_user);
	artDetailWnd->Create(*this, _T("ArtDetailWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	artDetailWnd->ShowWindow();
	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*artDetailWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = (nScreenWidth-nWndWidth)/2;
	int y = (nScreenHeight - nWndHeight)/2;
	::MoveWindow(*artDetailWnd, x, y, nWndWidth, nWndHeight,TRUE);
}

void CHomeWnd::OnSetting(int nSel)
{
	CSettingWnd* pSettingWnd = new CSettingWnd(nSel, m_user);
	pSettingWnd->Create(*this, _T("SettingWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	pSettingWnd->ShowWindow();
	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*pSettingWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = (nScreenWidth-nWndWidth)/2;
	int y = (nScreenHeight - nWndHeight)/2;
	::MoveWindow(*pSettingWnd, x, y, nWndWidth, nWndHeight,TRUE);
	return;
}

void CHomeWnd::ShowSettingWnd(int nSel, const string& strName)
{
	CSettingWnd* pSettingWnd = new CSettingWnd(nSel, m_user, strName);
	pSettingWnd->Create(*this, _T("SettingWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	pSettingWnd->ShowWindow();
	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*pSettingWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = (nScreenWidth-nWndWidth)/2;
	int y = (nScreenHeight - nWndHeight)/2;
	::MoveWindow(*pSettingWnd, x, y, nWndWidth, nWndHeight,TRUE);
	return;
}

void CHomeWnd::OnBtnActive()
{
	CActiveWnd* pActiveWnd = new CActiveWnd(m_user);
	pActiveWnd->Create(*this, _T("ActivityWnd"), UI_WNDSTYLE_DIALOG|UI_WNDSTYLE_CHILD, WS_EX_WINDOWEDGE);
	pActiveWnd->ShowWindow();
	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*pActiveWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = (nScreenWidth-nWndWidth)/2;
	int y = (nScreenHeight - nWndHeight)/2;
	::MoveWindow(*pActiveWnd, x, y, nWndWidth, nWndHeight,TRUE);
	return;
}


LRESULT CHomeWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		CDuiString* pstrMenuName = (CDuiString*)wParam;
		CTeacher teacher;
		CCareListUI* pList = NULL;
		CListContainerElementUI* pListElement = NULL;
		if (*pstrMenuName == _T("menuCancel")) // 取消关注
		{
			pList = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCare")));
			pListElement= static_cast<CListContainerElementUI*>(pList->GetItemAt(m_nSelectItemIndex));
			CDuiString strId = pListElement->GetUserData();
			string strID = StringFromLPCTSTR(strId);
			int nTid = atoi(strID.c_str());
			OnCancelCare(nTid);	//取消关注
		}
		else if (*pstrMenuName == _T("menuView")) // 查看关注讲师资料
		{
			pList = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCare")));
			pListElement = static_cast<CListContainerElementUI*>(pList->GetItemAt(m_nSelectItemIndex));
			teacher = m_vecCareTeacher[m_nSelectItemIndex];
			ShowInfoWnd(0, teacher);
		}
	}
	bHandled = FALSE;
	return 0;
}



// 更新财经分析列表
void CHomeWnd::UpdateAnayList()
{
	CAnayListUI* pAnayListWnd;
	if (m_nType == 0)
	{
		pAnayListWnd = static_cast<CAnayListUI*>(m_PaintManager.FindControl(_T("lstAnay")));

	}
	else
	{
		pAnayListWnd = static_cast<CAnayListUI*>(m_PaintManager.FindControl(_T("lstTeacherAnay")));
	}
	if (pAnayListWnd != NULL)
	{
		if (pAnayListWnd->GetCount() > 0)
		{
			pAnayListWnd->RemoveAll();
		}
		if (m_vecExchange.size() > 0)
		{
			m_vecExchange.clear();
		}

		CExchangeLogic exchangeLogic;
		string strMsg;
		int nRes = exchangeLogic.GetAllExchanges(m_user.GetToken(), m_vecExchange, strMsg);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}

		int nSize = m_vecExchange.size();
		for (int i = 0; i < nSize; i++)
		{
			CExchange exchange = m_vecExchange[i];
			pAnayListWnd->AddItem(exchange);
		}
	}

}


vector<CExchange> CHomeWnd::GetExchangeVec()
{
	return m_vecExchange;
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
		if (m_vecCareTeacher.size() > 0)
		{
			m_vecCareTeacher.clear();
		}

		if (pCareListUI->GetCount() > 0)
		{
			pCareListUI->RemoveAll();
		}

		CTeacherLogic teacherLogic;
		//vector<CTeacher> vecTeacher;
		string strMsg;
		int nRes = teacherLogic.GetCareTeacher(m_user.GetToken(), m_vecCareTeacher, strMsg);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}


		int nSize = m_vecCareTeacher.size();
		for (int i = 0; i < nSize; i++)
		{
			//CExchange exchange = vecExchange[i];
			//CTeacher teacher vecTeacher;
			pCareListUI->AddItem(m_vecCareTeacher[i]);
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
		string strMsg;
		int nPage = 0;
		int nPageNum = 10;
		int nRes = teacherLogic.GetTopTeacher( m_user.GetToken(), m_vecTeacher, strMsg);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}

		int nSize = m_vecTeacher.size();
		for (int i = 0; i < nSize; i++)
		{
			//CExchange exchange = vecExchange[i];
			//CTeacher teacher vecTeacher;
			pCareListUI->AddItem(m_vecTeacher[i]);
		}
	}
}

// 更新名人榜单
void CHomeWnd::UpdateCeleList()
{
	CCareListUI* pCeleListUI = static_cast<CCareListUI*>(m_PaintManager.FindControl(_T("lstCele")));
	if (pCeleListUI != NULL)
	{
		if (m_vecCeleTeacher.size() > 0)
		{
			m_vecCeleTeacher.clear();
		}

		if (pCeleListUI->GetCount() > 0)
		{
			pCeleListUI->RemoveAll();
		}

		CTeacherLogic teacherLogic;
		//vector<CTeacher> vecTeacher;
		string strMsg;
		int nPage = 0;
		int nPageNum = 20;
		int nRes = teacherLogic.GetStarTeacher(nPage, nPageNum, m_user.GetToken(), m_vecCeleTeacher, strMsg);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}

		int nSize = m_vecCeleTeacher.size();
		for (int i = 0; i < nSize; i++)
		{
			//CExchange exchange = vecExchange[i];
			//CTeacher teacher vecTeacher;
			pCeleListUI->AddItem(m_vecCeleTeacher[i]);
		}
	}

}


// 更新我的文章
void CHomeWnd::UpdateArtList()
{
	CArtListUI* pArtList = static_cast<CArtListUI*>(m_PaintManager.FindControl(_T("lstMyArt")));
	if (pArtList != NULL)
	{
		if (pArtList->GetCount() > 0)
		{
			pArtList->RemoveAll();
		}
		CTeacherLogic teacherLogic;
		vector<CArticle> vecArt;
		string strInfo;
		int nRes = teacherLogic.GetAllArtOfTeacher(0,10,m_user.GetToken(), vecArt, strInfo);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}
		int nSize = vecArt.size();

		for (int i = 0; i < nSize; i++)
		{
			CArticle art = vecArt[i];
			pArtList->AddItem(art);
		}
	}

}

// 取消关注
void CHomeWnd::OnCancelCare(int nTid)
{
	CTeacherLogic teacherLogic;
	string strMsg;
	int nRes = teacherLogic.DelAttention(nTid, m_user.GetToken(), strMsg);
	if (nRes != 0)
	{
		MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
		return;
	}
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
void CHomeWnd::ShowExTeachersInfo(const CExchange& ex)
{
	CSearchWnd* pSearchWnd = new CSearchWnd(ex);
	//pSearchWnd->SetExid(nExid);
	pSearchWnd->SetUser(m_user);
	
	pSearchWnd->Create(*this, _T("SearchWnd"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 100,100,800,600);
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
	CShowInfoWnd* pInfoWnd = new CShowInfoWnd(nSelected, teacher, m_user);
	//pInfoWnd->SetTeacher(teacher);
	pInfoWnd->Create(*this, _T("ShowInfoWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
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

