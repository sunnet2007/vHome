#include "RegisterWnd.h"
#include "UserManagerLogic.h"
#include <regex>
#include <string>
#include "StringUtil.h"
#include "LoginWnd.h"
#include "RegisterInfo.h"
using namespace std;

CRegisterWnd::CRegisterWnd(bool bRegister)
{
	m_bRegister = bRegister;
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

void CRegisterWnd::OnPrepare(TNotifyUI& msg)
{
	CButtonUI* pBtnReg = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnReg")));
	if (pBtnReg)
	{
		if (m_bRegister)
			pBtnReg->SetText(_T("ע��"));

		else
		{
			COptionUI* pOpUser = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opUser")));
			if (pOpUser)
			{
				pOpUser->SetVisible(false);
			}
			COptionUI* pOpTeacher = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opTeacher")));
			if (pOpTeacher)
			{
				pOpTeacher->SetVisible(false);
			}

			pBtnReg->SetText(_T("�һ�����"));
		}
	}
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
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
		return;
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
			CUserManagerLogic userManLogic;
			string szPhone = StringFromLPCTSTR(strPhone.GetData());
			string strInfo;
			CLabelUI* pLabelMsg = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelTip")));

			int nRes = 0;
			if (m_bRegister)
			{
				// ����ע����
				nRes = userManLogic.SendCode(szPhone, strInfo);
			}
			else
			{
				// �����һ�������֤��
				nRes = userManLogic.SendZhYzm(szPhone, strInfo);
			}
			if (nRes != 0)
			{	
				wstring wstr = s2ws(strInfo);
				pLabelMsg->SetVisible(true);
				pLabelMsg->SetText(wstr.c_str());
			}
			else
			{
				pLabelMsg->SetVisible(false);
			}

		}
		else
		{
			CLabelUI* pLabelMsg = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelTip")));
			pLabelMsg->SetVisible(true);
			pLabelMsg->SetText(_T("�������ֻ���"));

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
	COptionUI* pOpUser = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opUser")));
	COptionUI* pOpTeacher = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opTeacher")));
	
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

		CUserManagerLogic userManLogic;
		string strInfo;
		if (m_bRegister)
		{
			CRegisterInfo regInfo;
			regInfo.SetPhone(szPhone);
			regInfo.SetPasswd(szPwd);
			regInfo.SetYzm(szCode);
			regInfo.SetRepasswd(szRePwd);
			if (pOpUser->IsSelected())
				regInfo.SetType(1); // �û�
			else
				regInfo.SetType(2);// ��ʦ

			// ע��
			int nRes = userManLogic.Register(regInfo,strInfo);
			if (nRes == 0)
			{
				MessageBox(*this, _T("ע��ɹ�"), _T("V�ҽ��"), 0);
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
		else
		{
			CForgetPwd forget;
			forget.SetPhone(szPhone);
			forget.SetPasswd(szPwd);
			forget.SetYzm(szCode);
			forget.SetRepasswd(szRePwd);

			// �һ�����
			CUserManagerLogic userManLogic;
			int nRes = userManLogic.GetPwd(forget, strInfo);
			if (nRes == 0)
			{
				MessageBox(*this, _T("�һ�����ɹ������μ�����������"), _T("V�ҽ��"), 0);
				this->Close();
				return;
			}
			else
			{
				//string strErrorMsg = regInfo.GetMsg();
				wstring s = s2ws(strInfo);

				::MessageBox(*this, s.c_str() , _T("V�ҽ��"),0);
			}
		}
	}
	else
		::MessageBox(*this, _T("��������ȷ����Ϣ"), _T("V�ҽ��"), 0);
	return;
}