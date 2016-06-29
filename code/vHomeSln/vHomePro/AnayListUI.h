#pragma once
#include "duilib.h"
#include "Exchange.h"

class CAnayListUI :
	public CListUI
{
public:
	CAnayListUI(CPaintManagerUI& paint_manager);
	~CAnayListUI(void);

	//bool Add(CControlUI* pControl);
	//bool AddAt(CControlUI* pControl, int iIndex);
	//bool Remove(CControlUI* pControl, bool bDoNotDestroy/* =false */);
	//void RemoveAll();

	void DoEvent(TEventUI& event);
	bool SelectItem(int iIndex, bool bTakeFocus/*  = false */);

	bool AddItem(const CExchange& item);
	virtual void Notify(TNotifyUI& msg);

private:
	CPaintManagerUI& m_paintManager;
	CDialogBuilder m_dlgBuilder;
};

