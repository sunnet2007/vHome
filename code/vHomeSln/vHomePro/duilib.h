#pragma once
#include <UIlib.h>
using namespace DuiLib;

//#ifdef _DEBUG
//#   ifdef _UNICODE
//#       pragma comment(lib, "DuiLib.lib")
//#   else
//#       pragma comment(lib, "DuiLib_d.lib")
//#   endif
//#else
//#   ifdef _UNICODE
//#       pragma comment(lib, "DuiLib_u.lib")
//#   else
//#       pragma comment(lib, "DuiLib.lib")
//#   endif
//#endif

// 以XML生成界面的窗口基类
class CXMLWnd : public WindowImplBase
{
public:
	explicit CXMLWnd(LPCTSTR pszXMLPath) 
		: m_strXMLPath(pszXMLPath){}

public:
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("XMLWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return m_strXMLPath;
	}

	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}

protected:
	CDuiString m_strXMLPath;
};


//#ifdef _DEBUG
//#pragma comment(lib, "DuiLib.lib")
//#else
//#pragma comment(lib,"DuiLib.lib")
//#endif // _DEBUG


#ifdef DEBUG
#pragma comment(lib, "duilib_d.lib")
#else
#pragma comment(lib,"duilib.lib")
#endif // DEBUG
