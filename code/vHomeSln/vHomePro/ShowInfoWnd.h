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

	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

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
	int m_nSelectedTab; // ��ʾ��tab
	CTeacher m_teacher;	// ��ʦ
	CUser m_user;	// �û�
	vector<COptionUI*> m_vecOp;
	vector<CArticle> m_vecArt;
	vector<CCategory> m_vecCtgy;
};

