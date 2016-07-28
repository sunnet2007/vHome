#pragma once
#include "duilib.h"
#include "Exchange.h"
#include "User.h"
#include "Teacher.h"

// ���Ҵ���
class CSearchWnd : public WindowImplBase
{
public:
	CSearchWnd(const CExchange& ex );
	~CSearchWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

	CControlUI* CreateControl(LPCTSTR pstrClass);

public:
	//void SetExid(int nExid);
	//int GetExid();

	void SetUser(const CUser& user);
	CUser GetUser();

protected:
	void OnCare(int nUid);

private:
	void ShowExchangeInfo();	// ��ʾ��������Ϣ
	void ShowAllExTeachers();	// ��ʾ��������ʦ��Ϣ
	void OnSearch();			// �����������еĽ�ʦ

private:
	//int m_nExid;	// ������id
	CUser m_user;
	vector<CTeacher> m_vecTeacher; // ��������ʦ�б�
	CExchange m_exchange;
};

