#pragma once
#include "duilib.h"
#include "Teacher.h"


class CShowInfoWnd : public WindowImplBase
{
public:
	CShowInfoWnd(void);
	CShowInfoWnd(int nSelectedTab);
	~CShowInfoWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void Notify(TNotifyUI& msg);

	void OnPrepare(TNotifyUI& msg);

	void OnCancelCare();

public:
	void SetTeacher(const CTeacher& teacher);
	CTeacher GetTeacher();

private:
	int m_nSelectedTab; // 显示的tab
	CTeacher m_teacher;	// 讲师
};

