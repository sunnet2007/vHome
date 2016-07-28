#include "AboutWnd.h"
#include "AppConfig.h"
#include "StringUtil.h"

CAboutWnd::CAboutWnd(void)
{
}


CAboutWnd::~CAboutWnd(void)
{
}

LPCTSTR CAboutWnd::GetWindowClassName() const
{
	return _T("AboutWindow");
}
CDuiString CAboutWnd::GetSkinFile()
{
	return _T("AboutWnd.xml");
}
CDuiString CAboutWnd::GetSkinFolder()
{
	return _T("");
}

void CAboutWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			Close();
			return;
		}
	}
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
		return;
	}
	return __super::Notify(msg);
}

void CAboutWnd::OnPrepare(TNotifyUI& msg)
{
	string strContent = CAppConfig::NOS_ABOUTUS ;
	wstring wstrContent = s2ws(strContent);
	CWebBrowserUI* pActiveXUI = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("webContent")));
	if (pActiveXUI)
	{
		pActiveXUI->NavigateUrl(wstrContent.c_str());
	}
}