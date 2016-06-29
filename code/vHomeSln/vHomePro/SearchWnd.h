#pragma once
#include "duilib.h"
#include "Exchange.h"
#include "User.h"

// ���Ҵ���
class CSearchWnd : public WindowImplBase
{
public:
	CSearchWnd(void);
	~CSearchWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

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
	void ShowExchangeInfo();	// ��ʾ��������Ϣ
	void ShowAllExTeachers();	// ��ʾ��������ʦ��Ϣ

private:
	int m_nExid;	// ������
	CUser m_user;
};

