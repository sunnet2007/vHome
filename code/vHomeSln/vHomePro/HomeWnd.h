#pragma once
#include "duilib.h"
#include "AnayListUI.h"
#include "User.h"
#include "Teacher.h"
#include "Article.h"
#include <vector>
using namespace std;

// 主界面窗口
class CHomeWnd : public WindowImplBase
{
public:
	CHomeWnd(CUser& user);
	~CHomeWnd(void);

public:
	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径

	virtual void RemoveControl( CControlUI *pContrl ); // 移动控件
	virtual void InitWindow();// 初始化窗口

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	HRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	void OnPrepare(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClass);


public:
	CUser& GetUser() ;
	void SetUser(CUser& user);
	vector<CExchange> GetExchangeVec();

public:
	void UpdateAnayList();
	void UpdateMsgList();
	void UpdateCareList();
	void UpdateTeacherList();
	void UpdateCeleList();
//	void ShowSearchWnd();
	void ShowInfoWnd(int nSelected, const CTeacher& teacher);
	void ShowExTeachersInfo(const CExchange& ex);
	void OnCancelCare(int nTid);
	void ShowMenuWnd(int x, int y);
	void OnSetting(int nSel);
	void OnBtnActive();
	void OnBtnAbout();
	void UpdateArtList();
	void ShowSettingWnd(int nSel, const string& strName);
	void ShowArtDetailsWnd(int nArtId);

protected:
	CUser m_user;
	vector<CTeacher> m_vecTeacher;		// 推荐讲师列表
	vector<CTeacher> m_vecCareTeacher;	//关注讲师列表
	vector<CTeacher> m_vecCeleTeacher;	//名人榜单
	vector<CExchange> m_vecExchange;	// 财经列表
	vector<CArticle> m_vecArt;	// 文章列表
	int m_nSelectItemIndex;		// 选中列表项索引
	CDuiString m_strMenu;		// 菜单xml文件
	int m_nType;		// 登录的用户类型 0学员 1讲师

};


