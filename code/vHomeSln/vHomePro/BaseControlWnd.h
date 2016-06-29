#pragma once
#include "duilib.h"
class CBaseControlWnd : public WindowImplBase
{
public:
	CBaseControlWnd(void);
	~CBaseControlWnd(void);

protected:
	virtual CDuiString GetSkinFile() {return _T("base.xml");}
	virtual CDuiString GetSkinFolder() {return _T("skin");}
	virtual LPCTSTR GetWindowClassName() const {return _T("BaseWnd");}
	virtual void Notify(TNotifyUI& msg);

protected:
	CButtonUI* m_btnClose;
	CButtonUI* m_btnMin;
	CButtonUI* m_btnMax;
	CButtonUI* m_btnRestore;
};

