#pragma once

class CMenuWnd : public WindowImplBaseNew
{
public:
	CMenuWnd( );
	virtual ~CMenuWnd(void);
	
	static enum 
	{
		letfTop = 0,
		rightTop,
		letfBottom,
		rightBottom
	};

	static bool CreateMenu( HWND hParent,LPCTSTR xml,int PopPos=rightBottom  );
	void EnableItem( UINT index,bool bDisable=FALSE );
	void CheckItem ( UINT index,bool bCheck=TRUE );

private:
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitAfterCreate();
	virtual void AdjustPostion();
	virtual void ItemSelectNotify( TNotifyUI& msg );
	virtual void ItemClickNotify( TNotifyUI& msg );
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	HWND m_hWndMsg;
	CListUI *m_pMenuList;
	int m_iPopPos;
};
