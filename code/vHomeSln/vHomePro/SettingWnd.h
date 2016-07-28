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
	virtual LPCTSTR GetWindowClassName() const;  // 获取窗口名
	virtual CDuiString GetSkinFile();	// 获取布局文件
	virtual CDuiString GetSkinFolder(); // 获取布局文件路径
	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

private:
	void OnRelease();
	void OnUpload();
	void OnApply();
	void OnPhoto();

	// 处理学生界面
	void HandleStuInterface();
	// 处理讲师界面
	void HandleTeacherInterface();



private:
	CUser m_user;
	int m_nSel;
	string m_strName;
	CTeacher m_teacher;
	string m_strImg;
	string m_strArtImg;
};

