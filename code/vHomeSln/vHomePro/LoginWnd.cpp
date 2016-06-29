#include "LoginWnd.h"
#include "HomeWnd.h"
#include "RegisterWnd.h"
#include "User.h"
#include "StringUtil.h"
#include "UserManagerLogic.h"

CLoginWnd::CLoginWnd(void)
{
}


CLoginWnd::~CLoginWnd(void)
{
}

LPCTSTR CLoginWnd::GetWindowClassName() const
{
	return _T("LoginWindow");
}
CDuiString CLoginWnd::GetSkinFile()
{
	return _T("LoginWnd.xml");
}
CDuiString CLoginWnd::GetSkinFolder()
{
	return _T("");
}

void CLoginWnd::InitWindow()
{
	//::MessageBox(NULL, _T("hello"), _T("msg"),0);
}

void CLoginWnd::RemoveControl( CControlUI *pContrl )
{

}
void CLoginWnd::Notify(TNotifyUI& msg)
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
		else if (msg.pSender->GetName() == _T("btnLogin"))
		{
			OnLogin();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnReg"))
		{
			OnRegister();
			return;
		}
	}
	__super::Notify(msg);
}

LRESULT CLoginWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LRESULT lRes = 0;
	//BOOL bHandled = TRUE;
	//switch (uMsg)
	//{
	////case WM_NCLBUTTONDBLCLK:
	////	return lRes;
	//default:
	//	bHandled = FALSE;
	//	break;
	//}
	//if (bHandled || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	//{
	//	return lRes;
	//}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CLoginWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandle)
{
	return __super::MessageHandler(uMsg,wParam,lParam,bHandle);
}
//UINT CLoginWnd::GetClassStyle() const
//{ 
//	return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; 
//}

// 登录
void CLoginWnd::OnLogin()
{
	// 获取控件
	CEditUI* pEditAccount = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editAccount")));
	CEditUI* pEditPassword = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPassword")));
	if (!pEditAccount || !pEditPassword)
	{
		return;
	}

	// 获取登录手机号和密码
	CDuiString strPhone = pEditAccount->GetText();
	CDuiString strPassword = pEditPassword->GetText();
	if (strPhone.GetLength() <= 0 || strPassword.GetLength() <= 0)
	{
		return;
	}
	string szPhone = StringFromLPCTSTR(strPhone.GetData());
	string szPassword = StringFromLPCTSTR(strPassword.GetData());

	CUser user;
	user.SetPhone(szPhone);
	user.SetPassword(szPassword);
	

	CUserManagerLogic userManLogic;
	string strInfo = "";
	if (userManLogic.Login(user, strInfo) == 0)
	{
		CHomeWnd* pHomeWnd = new CHomeWnd;
		pHomeWnd->Create(NULL, _T("MainWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE,100,100,800,600);
		pHomeWnd->SetUser(user);
		pHomeWnd->ShowWindow();
		::ShowWindow(*this, SW_HIDE);
		int nWidth= GetSystemMetrics(SM_CXFULLSCREEN);
		int nHeight= GetSystemMetrics(SM_CYFULLSCREEN);
		int x = nWidth-272;
		int y = nHeight-640;
		::MoveWindow(*pHomeWnd, x-100,y-300, 272, 640,TRUE);
	}
	//user.strPhone = pEditAccount->GetText().GetData();


	//this->Close();
}

// 注册
void CLoginWnd::OnRegister()
{
	CRegisterWnd* pRegWnd = new CRegisterWnd;
	pRegWnd->Create(NULL, _T("RegisterWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pRegWnd->ShowWindow();
	pRegWnd->CenterWindow();
}

// 忘记密码
void CLoginWnd::OnForgetPassword()
{

}