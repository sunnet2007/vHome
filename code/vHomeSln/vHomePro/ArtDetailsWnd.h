#pragma once
#include "duilib.h"
#include "User.h"

class CArtDetailsWnd : public WindowImplBase
{
public:
	CArtDetailsWnd(void);
	CArtDetailsWnd(int nArtId, CUser user);
	~CArtDetailsWnd(void);
	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径
	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

private:
	int m_nArtId;
	CUser m_user;
};

