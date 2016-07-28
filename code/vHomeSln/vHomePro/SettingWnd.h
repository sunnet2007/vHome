#pragma once
#include "duilib.h"
#include "User.h"
#include "Teacher.h"

class CSettingWnd : public WindowImplBase
{
public:
	CSettingWnd(void);
	CSettingWnd(int nSel, const CUser& user);
	CSettingWnd(int nSel, const CUser& user, const string& strName);
	~CSettingWnd(void);

public:
	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��
	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

private:
	void OnRelease();
	void OnUpload();
	void OnApply();
	void OnPhoto();

	// ����ѧ������
	void HandleStuInterface();
	// ����ʦ����
	void HandleTeacherInterface();



private:
	CUser m_user;
	int m_nSel;
	string m_strName;
	CTeacher m_teacher;
	string m_strImg;
	string m_strArtImg;
};

