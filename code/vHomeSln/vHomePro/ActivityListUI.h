#pragma once
#include "duilib.h"
#include "Activity.h"

class CActivityListUI : public CListUI
{
public:
	CActivityListUI(CPaintManagerUI& paint_manager);
	~CActivityListUI(void);

	//bool Add(CControlUI* pControl);
	//bool AddAt(CControlUI* pControl, int iIndex);
	//bool Remove(CControlUI* pControl, bool bDoNotDestroy/* =false */);
	//void RemoveAll();

	//void DoEvent(TEventUI& event);
	//bool SelectItem(int iIndex, bool bTakeFocus/*  = false */);

	bool AddItem(const CActivity& item);
	// 	virtual void Notify(TNotifyUI& msg);

private:
	CPaintManagerUI& m_paintManager;
	CDialogBuilder m_dlgBuilder;
};

