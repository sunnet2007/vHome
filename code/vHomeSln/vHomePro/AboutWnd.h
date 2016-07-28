#pragma once
#include "duilib.h"

class CAboutWnd : public WindowImplBase
{
public:
	CAboutWnd(void);
	~CAboutWnd(void);
	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
};

