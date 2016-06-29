#pragma once
#include "duilib.h"
#include "Teacher.h"

class CExTeacherListUI : public CTileLayoutUI
{
public:
	CExTeacherListUI(CPaintManagerUI& paint_manager);
	~CExTeacherListUI(void);

public:
	void AddItem(const CTeacher& teacher);
	void DoEvent(TEventUI& event); 

private:
	CPaintManagerUI& m_paintManager;
	CDialogBuilder m_dlgBuilder;
};

