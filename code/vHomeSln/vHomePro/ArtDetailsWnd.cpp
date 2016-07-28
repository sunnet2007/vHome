#include "ArtDetailsWnd.h"
#include "StringUtil.h"
#include "AppConfig.h"
#include "TeacherDao.h"


CArtDetailsWnd::CArtDetailsWnd(void)
{
}

CArtDetailsWnd::CArtDetailsWnd(int nArtId, CUser user)
{
	m_nArtId = nArtId;
	m_user = user;
}
CArtDetailsWnd::~CArtDetailsWnd(void)
{
}

LPCTSTR CArtDetailsWnd::GetWindowClassName() const
{
	return _T("ArtDetailsWnd");
}
CDuiString CArtDetailsWnd::GetSkinFile()
{
	return _T("ArtDetailsWnd.xml");
}
CDuiString CArtDetailsWnd::GetSkinFolder()
{
	return _T("");
}

void CArtDetailsWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			this->Close();
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
	__super::Notify(msg);
}

void CArtDetailsWnd::OnPrepare(TNotifyUI& msg)
{
	//CTeacherDao dao;
	//string strHtml;
	//CResMsg Resmsg;
	//dao.GetArtDetails(m_user.GetToken(), m_nArtId, strHtml,Resmsg);
	char cid[32];
	memset(cid, 0, 32);
	itoa(m_nArtId, cid, 10);
	string strId(cid);
	string strContent = CAppConfig::NOS_TEACHER_ARTICLEINFO + strId + "/" + m_user.GetToken();
	wstring wstrContent = s2ws(strContent);
	CWebBrowserUI* pActiveXUI = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("webContent")));
	if (pActiveXUI)
	{
		pActiveXUI->Navigate2(wstrContent.c_str());
		//pActiveXUI->NavigateUrl(wstrContent.c_str());
	}

	return;
}

