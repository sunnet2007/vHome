#pragma once
#include "duilib.h"
#include <vector>
#include "User.h"
using namespace std;

// ��¼������
class CLoginWnd : public WindowImplBase
{
public:
	CLoginWnd(void);
	~CLoginWnd(void);

public:
	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��
	virtual void InitWindow();// ��ʼ������
	void OnPrepare(TNotifyUI& msg);

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& bHandle);
	//LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	//virtual UINT GetClassStyle() const; 


protected:
	CLabelUI*		m_pLabelAccount;
	CEditUI*		m_pEditAccount;
	CButtonUI*		m_pBtnOpenKeyBoard;
	CLabelUI*		m_pLabelPassword;
	CEditUI*		m_pEditPassword;
	CButtonUI*		m_pBtnLogin;
	CCheckBoxUI*	m_pChRemPwd;
	CCheckBoxUI*	m_pChAutoLogin;

private:
	void OnLogin();
	void OnRegister();
	void OnForgetPassword();
	void ShowHomeWnd(CUser& user);
	void SaveRemember(const string& strPhone, const string& strPwd);


	vector<Remember> m_vecRemember;

};

