#pragma once
#include "duilib.h"
#include "Teacher.h"


class CShowInfoWnd : public WindowImplBase
{
public:
	CShowInfoWnd(void);
	CShowInfoWnd(int nSelectedTab);
	~CShowInfoWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

	virtual void Notify(TNotifyUI& msg);

	void OnPrepare(TNotifyUI& msg);

	void OnCancelCare();

public:
	void SetTeacher(const CTeacher& teacher);
	CTeacher GetTeacher();

private:
	int m_nSelectedTab; // ��ʾ��tab
	CTeacher m_teacher;	// ��ʦ
};

