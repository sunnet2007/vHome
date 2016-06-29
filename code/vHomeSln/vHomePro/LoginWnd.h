#pragma once
#include "duilib.h"

// 登录窗口类
class CLoginWnd : public WindowImplBase
{
public:
	CLoginWnd(void);
	~CLoginWnd(void);

public:
	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void RemoveControl( CControlUI *pContrl ); // 移动控件
	virtual void InitWindow();// 初始化窗口

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

