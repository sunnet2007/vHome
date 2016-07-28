#pragma once
#include "duilib.h"
#include "AnayListUI.h"
#include "User.h"
#include "Teacher.h"
#include "Article.h"
#include <vector>
using namespace std;

// �����洰��
class CHomeWnd : public WindowImplBase
{
public:
	CHomeWnd(CUser& user);
	~CHomeWnd(void);

public:
	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

	virtual void RemoveControl( CControlUI *pContrl ); // �ƶ��ؼ�
	virtual void InitWindow();// ��ʼ������

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
	vector<CTeacher> m_vecTeacher;		// �Ƽ���ʦ�б�
	vector<CTeacher> m_vecCareTeacher;	//��ע��ʦ�б�
	vector<CTeacher> m_vecCeleTeacher;	//���˰�
	vector<CExchange> m_vecExchange;	// �ƾ��б�
	vector<CArticle> m_vecArt;	// �����б�
	int m_nSelectItemIndex;		// ѡ���б�������
	CDuiString m_strMenu;		// �˵�xml�ļ�
	int m_nType;		// ��¼���û����� 0ѧԱ 1��ʦ

};


