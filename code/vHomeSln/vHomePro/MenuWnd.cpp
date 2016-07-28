#include "stdafx.h"
#include "MenuWnd.h"
#include "MainFrame.h"

CMenuWnd::CMenuWnd( )
	:m_hWndMsg(NULL)
	,m_pMenuList(NULL)
	,m_iPopPos(0)
{
}

CMenuWnd::~CMenuWnd(void)
{

}

bool CMenuWnd::CreateMenu( HWND hParent,LPCTSTR xml,int PopPos  )
{
	CMenuWnd *pMenuWnd = new CMenuWnd();
	if ( pMenuWnd )
	{
		pMenuWnd->m_iPopPos = PopPos;
		pMenuWnd->m_hWndMsg = hParent;
		pMenuWnd->InitWinParam( xml );
		pMenuWnd->Create(hParent, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
		::ShowWindow(pMenuWnd->m_hWnd, SW_SHOW);
		::SendMessage(hParent, WM_NCACTIVATE, TRUE, 0L);
		return TRUE;
	}
	return FALSE;
}
void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(m_hWnd);
	delete this;
}

void CMenuWnd::InitAfterCreate()
{
	m_pMenuList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("menulist")));
}
void CMenuWnd::AdjustPostion()
{
	DuiLib::CPoint ptCur(0, 0);			
	::GetCursorPos(&ptCur);
	RECT rcWnd;
	::GetWindowRect( m_hWnd,&rcWnd );
	int iWidth = rcWnd.right-rcWnd.left;
	int iHeight = rcWnd.bottom-rcWnd.top;
	int iLeft = 0;
	int iTop = 0;
	switch ( m_iPopPos )
	{
	case letfTop:
		iLeft = ptCur.x-iWidth;
		iTop  = ptCur.y-iHeight-20;
		break;
	case letfBottom:
		iLeft = ptCur.x-iWidth;
		iTop  = ptCur.y;
		break;
	case rightTop:
		iLeft = ptCur.x;
		iTop  = ptCur.y-iHeight-20;
		break;
	case rightBottom:
	default:
		iLeft = ptCur.x;
		iTop  = ptCur.y;
		break;
	}
	::SetWindowPos(m_hWnd, HWND_TOPMOST,iLeft,iTop ,iWidth , iHeight, SWP_NOSIZE | SWP_NOACTIVATE );
}

void CMenuWnd::EnableItem( UINT index,bool bDisable)
{
	if ( index<0 || index >= m_pMenuList->GetCount() )
	{
		return;
	}
	m_pMenuList->GetItemAt( index )->SetEnabled( bDisable );
}

void CMenuWnd::CheckItem ( UINT index,bool bCheck )
{
	if ( index<0 || index >= m_pMenuList->GetCount() )
	{
		return;
	}
	m_pMenuList->GetItemAt( index )->SetBkImage(_T("file='check.png' dest='2,4,15,17'") );
}


void CMenuWnd::ItemSelectNotify( TNotifyUI& msg )
{
	Close();
}

void CMenuWnd::ItemClickNotify( TNotifyUI& msg )
{
	//--traymenu.xml--------------------------------------------------------
	if ( msg.pSender->GetName() == _T("menu_tray_show")  )
	{		
		::PostMessage(m_hWndMsg, UM_MENU_CLICK, 1, 0);

	}
	else if ( (msg.pSender->GetName() == _T("menu_tray_exit"))  )
	{
		::PostMessage(m_hWndMsg, UM_MENU_CLICK, 2, 0);
	}
	//--custommenu.xml------------------------------------------------------
	else if ( msg.pSender->GetName() == _T("menu_custom_skin") )
	{
		::PostMessage(m_hWndMsg, UM_MENU_CLICK, 3, 0);
	}
	else if ( msg.pSender->GetName() == _T("menu_custom_about") )
	{
		::PostMessage(m_hWndMsg, UM_MENU_CLICK, 4, 0);
	}
	//--operatemenu.xml----------------------------------------------------
	else if ( msg.pSender->GetName() == _T("menu_operate_down") )
	{
		::PostMessage(m_hWndMsg, UM_MENU_CLICK, 5, 0);
	}
	else if ( msg.pSender->GetName() == _T("menu_operate_stop") )
	{
		::PostMessage(m_hWndMsg, UM_MENU_CLICK, 6, 0);
	}
	//--multioperatemenu.xml-----------------------------------------------
	else if ( msg.pSender->GetName() == _T("menu_multioperate_delete") )
	{
		::PostMessage(m_hWndMsg, UM_MENU_CLICK, 7, 0);
	}
	
}


LRESULT CMenuWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_hWnd != (HWND) wParam ) PostMessage(WM_CLOSE);
	return 0;
}





