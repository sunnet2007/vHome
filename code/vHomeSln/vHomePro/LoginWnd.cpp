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

void CLoginWnd::OnPrepare(TNotifyUI& msg)
{
	CUserManagerLogic userLogic;
	userLogic.LoadRememberInfo(m_vecRemember);
	int nSize = m_vecRemember.size();
	for (int i = 0; i < nSize; i++)
	{
		Remember rem = m_vecRemember[i];
		if (m_vecRemember[i].bAuto)
		{
		}
	}
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
		else if (msg.pSender->GetName() == _T("btnForPwd"))
		{
			 OnForgetPassword();
			 return;
		}
		else if (msg.pSender->GetName() == _T("btnLogin2"))
		{
			OnLogin();
			return;
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		if (msg.pSender->GetName() == _T("editAccount"))
		{
			//MessageBox(NULL, _T("xxx"), _T(""), 0);
			CEditUI* pEdidAccount = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editAccount")));
			
			CDuiString str = pEdidAccount->GetText();
			string strAccount = StringFromLPCTSTR(str.GetData());
			int nSize = m_vecRemember.size();
			for (int i = 0; i < nSize; i++)
			{
				Remember rem = m_vecRemember[i];
				
				if (strcmp(rem.strPhone, strAccount.c_str()) == 0)
				{
					CCheckBoxUI* pCbRem = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chRemPwd")));
					if (pCbRem && rem.bRemember)
					{
						pCbRem->Selected(true);
						CEditUI* pEditPwd = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPassword")));
						if (pEditPwd)
						{
							string strPwd = rem.strPwd;
							wstring wstr = s2ws(strPwd);
							pEditPwd->SetText(wstr.c_str());
						}
					}

					CCheckBoxUI* pCbAuto = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chAutoLogin")));
					if (pCbAuto && rem.bAuto)
					{
						pCbAuto->Selected(true);
						CEditUI* pEditPwd = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPassword")));
						if (pEditPwd)
						{
							string strPwd = rem.strPwd;
							wstring wstr = s2ws(strPwd);
							pEditPwd->SetText(wstr.c_str());
						}
						/*Sleep(5000);*/
						OnLogin();
					}
				}
			}

			return;
		}
	}
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
		return;
	}
	else if(msg.sType == _T("selectchanged"))
	{	
		CCheckBoxUI* pCbAuto = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chAutoLogin")));					
		CCheckBoxUI* pCbRem = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chRemPwd")));
		if (msg.pSender->GetName() == _T("chAutoLogin"))
		{
			if (pCbAuto)
			{
				if (pCbAuto->IsSelected()) // 如果选择自动登录，则记住密码
				{
					if (pCbRem )
						pCbRem->Selected(true);
				}
			}
		}
		else if (msg.pSender->GetName() == _T("chRemPwd"))
		{
			if (pCbRem && pCbAuto)
			{
				if (!pCbRem->IsSelected())
					pCbAuto->Selected(false);
			}
		}

	}
	__super::Notify(msg);
}

void CLoginWnd::InitWindow()
{
	//CCheckBoxUI* cbRemeber = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chRemPwd")));
	//if (cbRemeber)
	//{
	//	cbRemeber->Selected(true);
	//}

	//CCheckBoxUI* cbAuthLogin = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chAutoLogin")));
	//if (cbAuthLogin)
	//{
	//	cbAuthLogin->Selected(true);
	//}
}

LRESULT CLoginWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;
	switch (uMsg)
	{
	case WM_NCLBUTTONDBLCLK:
		return lRes;

	default:
		break;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}


LRESULT CLoginWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandle)
{
	return __super::MessageHandler(uMsg,wParam,lParam,bHandle);
}

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
		//MessageBox(*this, _T("请输入账号和密码"), _T("V家金服"), 0);
		CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelTip")));
		pLabel->SetVisible(true);
		pLabel->SetText(_T("请输入账号和密码"));
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
		SaveRemember(szPhone, szPassword);
		ShowHomeWnd(user);
		this->Close();
	}
	else
	{
		wstring strMsg = s2ws(strInfo);
		CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelTip")));
		pLabel->SetVisible(true);
		pLabel->SetText(strMsg.c_str());
	}
}

// 注册
void CLoginWnd::OnRegister()
{
	CRegisterWnd* pRegWnd = new CRegisterWnd(true);
	pRegWnd->Create(NULL, _T("RegisterWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	pRegWnd->ShowWindow();
	pRegWnd->CenterWindow();
}

// 忘记密码
void CLoginWnd::OnForgetPassword()
{
	CRegisterWnd* pRegWnd = new CRegisterWnd(false);
	pRegWnd->Create(NULL, _T("ForgetWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	pRegWnd->ShowWindow();
	pRegWnd->CenterWindow();
}

// 显示主界面
void CLoginWnd::ShowHomeWnd(CUser& user)
{
	CHomeWnd* pHomeWnd = new CHomeWnd(user);
	pHomeWnd->Create(NULL, _T("MainWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE,100,100,800,600);
	pHomeWnd->ShowWindow();
	::ShowWindow(*this, SW_HIDE);
	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*pHomeWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = nScreenWidth-nWndWidth;
	int y = (nScreenHeight - nWndHeight)/2;
	if (y <= 0)
	{
		y = 0;
	}
	::MoveWindow(*pHomeWnd, x, y, nWndWidth, nWndHeight,TRUE);
}

// 保存记住密码、自动登录信息
void CLoginWnd::SaveRemember(const string& strPhone, const string& strPwd)
{
	CCheckBoxUI* pAuto = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chAutoLogin")));
	CCheckBoxUI* pRem = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chRemPwd")));
	if (pAuto && pRem)
	{
		Remember rem;
		//rem.bLogin = true;
		// 记住密码
		if (pRem->IsSelected())
		{		
			strcpy(rem.strPhone, strPhone.data());
			strcpy(rem.strPwd, strPwd.data());
			rem.bRemember = true;
			if (pAuto->IsSelected())
				rem.bAuto = true;
			else 
				rem.bAuto = false;
		}
		else // 不记住密码
		{
			strcpy(rem.strPhone, strPhone.data());
			strcpy(rem.strPwd, strPwd.data());
			rem.bRemember = false;
			rem.bAuto = false;
		}

		// 是否已经存在
		int nSize = m_vecRemember.size();
		bool exsit = false;
		int nIndex = -1;
		for (int i = 0; i < nSize; i++)
		{
			Remember exsitRem = m_vecRemember[i];
			if (strcmp(rem.strPhone, exsitRem.strPhone) == 0)
			{
				exsit = true;
				nIndex = i;
				break;
			}
		}
		if (exsit)  // 存在，换上最新的
			m_vecRemember[nIndex] = rem;
		else  // 不存在，直接添加
			m_vecRemember.push_back(rem);
		CUserManagerLogic userLogic;
		userLogic.SaveRememberInfo(m_vecRemember);
	}
}