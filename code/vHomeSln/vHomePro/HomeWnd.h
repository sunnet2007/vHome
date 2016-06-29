#pragma once
#include "duilib.h"
#include "AnayListUI.h"
#include "User.h"
#include "Teacher.h"
#include <vector>
using namespace std;

// 主界面窗口
class CHomeWnd : public WindowImplBase
{
public:
	CHomeWnd(void);
	~CHomeWnd(void);

public:
	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void RemoveControl( CControlUI *pContrl ); // 移动控件
	virtual void InitWindow();// 初始化窗口

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnPrepare(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClass);

public:
	CUser& GetUser() ;
	void SetUser(CUser& user);

protected:
	void UpdateAnayList();
	void UpdateMsgList();
	void UpdateCareList();
	void UpdateTeacherList();
	void UpdateSpecialist();
//	void ShowSearchWnd();
	void ShowInfoWnd(int nSelected, const CTeacher& teacher);
	void ShowExTeachersInfo(int nExid);
	void OnCancelCare(int nTid);

protected:
	CUser m_user;
	vector<CTeacher> m_vecTeacher;

};


