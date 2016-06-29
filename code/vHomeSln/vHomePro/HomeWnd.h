#pragma once
#include "duilib.h"
#include "AnayListUI.h"
#include "User.h"
#include "Teacher.h"
#include <vector>
using namespace std;

// �����洰��
class CHomeWnd : public WindowImplBase
{
public:
	CHomeWnd(void);
	~CHomeWnd(void);

public:
	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

	virtual void RemoveControl( CControlUI *pContrl ); // �ƶ��ؼ�
	virtual void InitWindow();// ��ʼ������

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


