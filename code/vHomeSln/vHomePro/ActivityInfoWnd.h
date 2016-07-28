#pragma once
#include "duilib.h"
#include "User.h"

class CActivityInfoWnd: public WindowImplBase
{
public:
	CActivityInfoWnd(void);
	CActivityInfoWnd(int nId, const CUser& user);
	~CActivityInfoWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

private:
	void OnBtnApply();

private:
	int m_nId;
	CUser m_user;
};

