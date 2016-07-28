#pragma once
#include "duilib.h"
#include <vector>
#include <string>
#include "Activity.h"
#include "User.h"
using namespace std;

class CActiveWnd : public WindowImplBase
{
public:
	CActiveWnd(CUser& user);
	~CActiveWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClass);

public:
	void SetUser(CUser& user);

private:
	void UpdateNewActivityList();
	void UpdateMyActivityList();
	void OnBtnView(int nId);

private:
	vector<CActivity> m_vecNewActivity;
	vector<CActivity> m_vecMyActivity;
	CUser m_user;
};

