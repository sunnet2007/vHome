#pragma once
#include "duilib.h"

// 注册用户窗口
class CRegisterWnd : public WindowImplBase
{
public:
	CRegisterWnd(bool bRegister);
	~CRegisterWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void Notify(TNotifyUI& msg); // 消息处理
	void OnPrepare(TNotifyUI& msg);


protected:
	void OnSend(); // 发送验证码
	void OnRegister(); // 注册
	bool m_bRegister;	// true注册 false找回密码
};

