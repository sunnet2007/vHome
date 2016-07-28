#pragma once
#include "duilib.h"

class CAboutWnd : public WindowImplBase
{
public:
	CAboutWnd(void);
	~CAboutWnd(void);
	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
};

