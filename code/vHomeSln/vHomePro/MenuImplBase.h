#pragma once
#include "Control/UIList.h"

namespace DuiLib
{
	class CWindowWnd;
	class INotifyUI;
	class IDialogBuilderCallback;

	class UILIB_API CMenuImplBase : public CWindowWnd, public INotifyUI
	{
	public:
		CMenuImplBase(void);
		~CMenuImplBase(void);

		void CreateMenu(HWND hWndMsg, LPCTSTR pstrXml, IDialogBuilderCallback* pCallback=NULL);

		virtual void InitItem();

		virtual void AdjustPostion();

		virtual LPCTSTR GetWindowClassName() const;
		virtual void OnFinalMessage(HWND /*hWnd*/);

		virtual void Notify(TNotifyUI& msg);

		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void Close(UINT nRet  = IDOK );

		void AddListItem(CControlUI* pListElement);

	protected:
		virtual CDuiString GetXmlFile();
		virtual int GetItemCount();
		virtual int GetItemElementHeght();

	protected:
		CPaintManagerUI m_paintManager;
		HWND m_hWndMsg;
		CListUI *m_pMenuList;

		POINT m_ptPos;
		CDuiString m_strXmlFile;

		IDialogBuilderCallback* m_pCallback;
	};
}
