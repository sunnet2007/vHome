#pragma once
#include "duilib.h"
#include "Article.h"

class CArtListUI :
	public CListUI
{
public:
	CArtListUI(CPaintManagerUI& paint_manager, int nType);
	~CArtListUI(void);

	//bool Add(CControlUI* pControl);
	//bool AddAt(CControlUI* pControl, int iIndex);
	//bool Remove(CControlUI* pControl, bool bDoNotDestroy/* =false */);
	//void RemoveAll();

	//void DoEvent(TEventUI& event);
	//bool SelectItem(int iIndex, bool bTakeFocus/*  = false */);

	bool AddItem(const CArticle& item);
	// 	virtual void Notify(TNotifyUI& msg);

private:
	CPaintManagerUI& m_paintManager;
	CDialogBuilder m_dlgBuilder;
	int m_nType;	// 类型。主界面0 显示信息界面1
};
