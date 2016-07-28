#pragma once
#include "duilib.h"
#include "User.h"

class CActivityInfoWnd: public WindowImplBase
{
public:
	CActivityInfoWnd(void);
	CActivityInfoWnd(int nId, const CUser& user);
	~CActivityInfoWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

private:
	void OnBtnApply();

private:
	int m_nId;
	CUser m_user;
};

