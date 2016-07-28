#include "stdafx.h"
#include "MenuImplBase.h"
namespace DuiLib
{
	CMenuImplBase::CMenuImplBase(void)
		:m_hWndMsg(NULL)
		,m_pMenuList(NULL)
		,m_pCallback(NULL)
	{
	}

	CMenuImplBase::~CMenuImplBase(void)
	{
	}
	CDuiString CMenuImplBase::GetXmlFile()
	{
		return m_strXmlFile;
	}
	void CMenuImplBase::CreateMenu(HWND hWndMsg, LPCTSTR pstrXml, IDialogBuilderCallback* pCallback)
	{
		m_hWndMsg = hWndMsg;
		m_strXmlFile = pstrXml;
		m_pCallback = pCallback;
		GetCursorPos(&m_ptPos);

		Create(hWndMsg, NULL, WS_POPUP, WS_EX_TOOLWINDOW);
		HWND hWndParent = m_hWnd;
		while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
		::ShowWindow(m_hWnd, SW_SHOW);
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	}

	void CMenuImplBase::InitItem()
	{
		m_pMenuList = static_cast<CListUI*>(m_paintManager.FindControl(_T("menulist")));
	}
	void CMenuImplBase::AdjustPostion()
	{
		int nItemCount = GetItemCount();
		int nListElementHeight = GetItemElementHeght();

		DuiLib::CDuiRect rcWnd;
		GetWindowRect(m_hWnd, &rcWnd);
		int nWidth = rcWnd.GetWidth();
		int nHeight = nItemCount * nListElementHeight + 4;
		rcWnd.left = m_ptPos.x;
		rcWnd.right = rcWnd.left + nWidth;



		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		DuiLib::CDuiRect rcWork = oMonitor.rcMonitor;

		if( (m_ptPos.y+1+nHeight) > rcWork.bottom ) 
		{
			if( nHeight >= rcWork.GetHeight() ) 
			{
				rcWnd.top = 0;
				rcWnd.bottom = nHeight;
			}
			else 
			{
				rcWnd.bottom =  m_ptPos.y;
				rcWnd.top = rcWnd.bottom - nHeight;
			}
		}
		else
		{
			rcWnd.top = m_ptPos.y+1;
			rcWnd.bottom = rcWnd.top + nHeight;
		}

		if( (m_ptPos.x+nWidth) > rcWork.right ) 
		{
			if( nWidth >= rcWork.GetWidth() ) 
			{
				rcWnd.left = 0;
				rcWnd.right = nWidth;
			}
			else 
			{
				rcWnd.right = m_ptPos.x;
				rcWnd.left = rcWnd.right - nWidth;
			}
		}
		else
		{
			rcWnd.left = m_ptPos.x;
			rcWnd.right = rcWnd.left + nWidth;
		}

		::SetWindowPos(m_hWnd, HWND_TOPMOST, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_SHOWWINDOW);
		::SetForegroundWindow(m_hWnd);
		//::BringWindowToTop(m_hWnd);
	}

	LPCTSTR CMenuImplBase::GetWindowClassName() const 
	{ 
		return _T("MenuWnd");
	}
	void CMenuImplBase::OnFinalMessage(HWND /*hWnd*/) 
	{ 
		//m_paintManager.RemovePreMessageFilter(this);
		m_paintManager.RemoveNotifier(this);
		m_paintManager.ReapObjects(m_paintManager.GetRoot());
	}

	void CMenuImplBase::Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("itemselect") ) 
		{
			Close();
		}
		else if( msg.sType == _T("itemclick") )
		{
			::PostMessage(m_hWndMsg, UM_MENU_CLICK, msg.pSender->GetTag(), 0);
		}
	}

	LRESULT CMenuImplBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(m_hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_paintManager.Init(m_hWnd);
		//m_paintManager.AddPreMessageFilter(this);

		CDuiString tstrSkin =  GetXmlFile();

		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create( (LPCTSTR)tstrSkin, (UINT)0, m_pCallback, &m_paintManager);
		assert( pRoot && "Failed to parse XML" );
		m_paintManager.AttachDialog(pRoot);
		m_paintManager.AddNotifier(this);

		InitItem();

		AdjustPostion();
		return 0;
	}

	LRESULT CMenuImplBase::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		Close();
		return 0;
	}

	LRESULT CMenuImplBase::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == VK_ESCAPE )
			Close();
		else
			bHandled = false;
		return 0;
	}

	LRESULT CMenuImplBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = m_paintManager.GetRoundCorner();
		if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) 
		{
			DuiLib::CDuiRect rcWnd;
			::GetWindowRect(*this, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(*this, hRgn, TRUE);
			::DeleteObject(hRgn);
		}

		bHandled = FALSE;
		return 0;
	}

	LRESULT CMenuImplBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg )
		{
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEWHEEL:    break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
		}
		if( bHandled ) return lRes;
		if( m_paintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}
	void CMenuImplBase::Close(UINT nRet/*  = IDOK*/ )
	{
		CWindowWnd::Close( nRet );
	}
	int CMenuImplBase::GetItemCount()
	{
		if( m_pMenuList != NULL )
		{
			return m_pMenuList->GetCount();
		}
		return 0;
	}
	int CMenuImplBase::GetItemElementHeght()
	{
		if( GetItemCount() > 0 )
		{
			CControlUI* pControl = m_pMenuList->GetItemAt( 0 );
			if( pControl != NULL )
			{
				CListContainerElementUI *pListElement = static_cast<CListContainerElementUI *>(pControl);
				if( pListElement != NULL )
					return pListElement->GetFixedHeight();
			}
		}
		return 0;
	}
	void CMenuImplBase::AddListItem(CControlUI* pListElement)
	{
		m_pMenuList->Add( pListElement );
	}
}
