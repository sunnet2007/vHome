#pragma once
#include "duilib.h"
#include "Exchange.h"
#include "User.h"

// 查找窗口
class CSearchWnd : public WindowImplBase
{
public:
	CSearchWnd(void);
	~CSearchWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

	CControlUI* CreateControl(LPCTSTR pstrClass);

public:
	void SetExid(int nExid);
	int GetExid();

	void SetUser(const CUser& user);
	CUser GetUser();

protected:
	void OnCare(int nUid);

private:
	void ShowExchangeInfo();	// 显示交易所信息
	void ShowAllExTeachers();	// 显示交易所讲师信息

private:
	int m_nExid;	// 交易所
	CUser m_user;
};

