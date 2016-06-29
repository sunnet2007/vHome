#include "RegisterWnd.h"
#include "UserManagerLogic.h"
#include <regex>
#include <string>
#include "StringUtil.h"
#include "LoginWnd.h"
#include "RegisterInfo.h"
using namespace std;

CRegisterWnd::CRegisterWnd(void)
{
}


CRegisterWnd::~CRegisterWnd(void)
{
}

LPCTSTR CRegisterWnd::GetWindowClassName() const
{
	return _T("RegisterWindow");
}

CDuiString CRegisterWnd::GetSkinFile()
{
	return _T("RegisterWnd.xml");
}

CDuiString CRegisterWnd::GetSkinFolder()
{
	return _T("");
}

void CRegisterWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
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
		else if(msg.pSender->GetName() == _T("btnReg")) // 注册
		{
			OnRegister();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnSend")) // 发送验证码
		{
			OnSend();
			return;
		}
	}
	__super::Notify(msg);
}

// 发送验证码
void CRegisterWnd::OnSend()
{
	CEditUI* pEditPhone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPhone")));
	if (pEditPhone)
	{
		CDuiString strPhone = pEditPhone->GetText();
		if (strPhone.GetLength() > 0)
		{
			// 发送注册码
			CUserManagerLogic userManLogic;
			string strCode;

			string szPhone = StringFromLPCTSTR(strPhone.GetData());
			string strInfo;
			userManLogic.SendCode(szPhone, strInfo);
		}
	}

}

// 注册
void CRegisterWnd::OnRegister()
{
	// 根据控件名字，获取控件
	CEditUI* pEditPhone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPhone")));
	CEditUI* pEditCode = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editCode")));
	CEditUI* pEditPassword = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPassword")));
	CEditUI* pEditRePassword = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editRePassword")));
	
	if (!pEditPhone || !pEditCode || !pEditPassword || !pEditRePassword)
	{
		return;
	}

	// 获取编辑框内容
	CDuiString strPhone = pEditPhone->GetText();
	CDuiString strCode = pEditCode->GetText();
	CDuiString strPassword = pEditPassword->GetText();
	CDuiString strRePass = pEditRePassword->GetText();

	if (strPhone.GetLength() > 0 && strCode.GetLength() > 0 && strPassword.GetLength() > 0)
	{
		// 将获取的内容从CDuistring转化为string
		string szPhone = StringFromLPCTSTR(strPhone.GetData());
		string szCode = StringFromLPCTSTR(strCode.GetData());
		string szPwd = StringFromLPCTSTR(strPassword.GetData());
		string szRePwd = StringFromLPCTSTR(strRePass.GetData());

		CRegisterInfo regInfo;
		regInfo.SetPhone(szPhone);
		regInfo.SetPasswd(szPwd);
		regInfo.SetYzm(szCode);
		regInfo.SetRepasswd(szRePwd);

		// 注册
		CUserManagerLogic userManLogic;
		string strInfo;
		int nRes = userManLogic.Register(regInfo,strInfo);
		if (nRes == 1)
		{
			MessageBox(*this, _T("注册成功"), _T(""), 0);
			this->Close();
			return;
		}
		else
		{
			//string strErrorMsg = regInfo.GetMsg();
			wstring s = s2ws(strInfo);

			::MessageBox(*this, s.c_str() , _T(""),0);
		}
	}
	return;
}