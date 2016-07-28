#pragma once
#include "duilib.h"
#include "Teacher.h"
#include "User.h"
#include "Article.h"

class CShowInfoWnd : public WindowImplBase
{
public:
	CShowInfoWnd(void);
	CShowInfoWnd(int nSelectedTab, const CTeacher& teacher, const CUser& user);

	CShowInfoWnd(int nSelectedTab);
	~CShowInfoWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void Notify(TNotifyUI& msg);

	void OnPrepare(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClass);
private:
	void InitTitle();
	void InitPersonInfo();
	void InitArtList();
	void OnCancelCare();
	void ShowArtDetailsWnd(int nArtId);

public:
	void SetTeacher(const CTeacher& teacher);
	CTeacher GetTeacher();

private:
	int m_nSelectedTab; // 显示的tab
	CTeacher m_teacher;	// 讲师
	CUser m_user;	// 用户
	vector<COptionUI*> m_vecOp;
	vector<CArticle> m_vecArt;
	vector<CCategory> m_vecCtgy;
};

