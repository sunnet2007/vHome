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
		else if(msg.pSender->GetName() == _T("btnReg")) // ע��
		{
			OnRegister();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnSend")) // ������֤��
		{
			OnSend();
			return;
		}
	}
	__super::Notify(msg);
}

// ������֤��
void CRegisterWnd::OnSend()
{
	CEditUI* pEditPhone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPhone")));
	if (pEditPhone)
	{
		CDuiString strPhone = pEditPhone->GetText();
		if (strPhone.GetLength() > 0)
		{
			// ����ע����
			CUserManagerLogic userManLogic;
			string strCode;

			string szPhone = StringFromLPCTSTR(strPhone.GetData());
			string strInfo;
			userManLogic.SendCode(szPhone, strInfo);
		}
	}

}

// ע��
void CRegisterWnd::OnRegister()
{
	// ���ݿؼ����֣���ȡ�ؼ�
	CEditUI* pEditPhone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPhone")));
	CEditUI* pEditCode = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editCode")));
	CEditUI* pEditPassword = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPassword")));
	CEditUI* pEditRePassword = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editRePassword")));
	
	if (!pEditPhone || !pEditCode || !pEditPassword || !pEditRePassword)
	{
		return;
	}

	// ��ȡ�༭������
	CDuiString strPhone = pEditPhone->GetText();
	CDuiString strCode = pEditCode->GetText();
	CDuiString strPassword = pEditPassword->GetText();
	CDuiString strRePass = pEditRePassword->GetText();

	if (strPhone.GetLength() > 0 && strCode.GetLength() > 0 && strPassword.GetLength() > 0)
	{
		// ����ȡ�����ݴ�CDuistringת��Ϊstring
		string szPhone = StringFromLPCTSTR(strPhone.GetData());
		string szCode = StringFromLPCTSTR(strCode.GetData());
		string szPwd = StringFromLPCTSTR(strPassword.GetData());
		string szRePwd = StringFromLPCTSTR(strRePass.GetData());

		CRegisterInfo regInfo;
		regInfo.SetPhone(szPhone);
		regInfo.SetPasswd(szPwd);
		regInfo.SetYzm(szCode);
		regInfo.SetRepasswd(szRePwd);

		// ע��
		CUserManagerLogic userManLogic;
		string strInfo;
		int nRes = userManLogic.Register(regInfo,strInfo);
		if (nRes == 1)
		{
			MessageBox(*this, _T("ע��ɹ�"), _T(""), 0);
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