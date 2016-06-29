#pragma once
#include "duilib.h"

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

	virtual void RemoveControl( CControlUI *pContrl ); // �ƶ��ؼ�
	virtual void InitWindow();// ��ʼ������

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& bHandle);
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

};

