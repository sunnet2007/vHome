#pragma once
#include "duilib.h"
#include "Exchange.h"

class CAnayListUI :
	public CListUI
{
public:
	CAnayListUI(CPaintManagerUI& paint_manager);
	~CAnayListUI(void);

	//bool SelectItem(int iIndex, bool bTakeFocus/*  = false */);
	bool AddItem(const CExchange& item);

private:
	CPaintManagerUI& m_paintManager;
	CDialogBuilder m_dlgBuilder;
};

