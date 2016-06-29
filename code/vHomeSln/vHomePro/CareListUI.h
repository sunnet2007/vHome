#pragma once
#include "duilib.h"
#include "Teacher.h"

class CCareListUI :	public CListUI
{
public:
	CCareListUI(CPaintManagerUI& paint_manager);
	~CCareListUI(void);


	void DoEvent(TEventUI& event);
	bool SelectItem(int iIndex, bool bTakeFocus/*  = false */);

	bool AddItem(const CTeacher& teacher);

private:
	CPaintManagerUI& m_paintManager;
	CDialogBuilder m_dlgBuilder;
};

