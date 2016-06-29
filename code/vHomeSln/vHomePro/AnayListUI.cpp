#include "AnayListUI.h"
#include "StringUtil.h"

CAnayListUI::CAnayListUI(CPaintManagerUI& paint_manager):m_paintManager(paint_manager)
{
}


CAnayListUI::~CAnayListUI(void)
{
}

void CAnayListUI::Notify(TNotifyUI& msg)
{
	CDuiString sType = msg.sType;
}


void CAnayListUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_RBUTTONDOWN)
	{
		CPoint pt(0,0);
		GetCursorPos(&pt);
		//CMenuWnd* pMenu = new CMenuWnd;
		//pMenu->Create(*this, _T("MenuWnd"),UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, LOWORD(lParam), HIWORD(wParam));
		CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("MenuWnd.xml"),pt, &m_paintManager);
		pMenu->ShowModal();
	}


	return ;
}

bool CAnayListUI::SelectItem(int iIndex, bool bTakeFocus  = false )
{
	return true;
}


bool CAnayListUI::AddItem(const CExchange& item)
{
	TCHAR szBuf[126];
	memset(szBuf, 0, 126);
	CListContainerElementUI* pListElement = NULL;
	if (!m_dlgBuilder.GetMarkup()->IsValid())
	{
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("AnayList.xml"), (UINT)0, NULL, &m_paintManager));
	}
	else
	{
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &m_paintManager));
	}
	if (pListElement == NULL)
	{
		return false;
	}
	// 将ID与列表项绑定
	char cId[32];
	memset(cId, 0,32);
	itoa(item.GetId(), cId, 10);
	TCHAR tcId[32];
	memset(tcId, 0, 32);
	CharToTchar(cId, tcId);
	pListElement->SetUserData(tcId);

	// 交易所图标
	CLabelUI* pLabelLogo = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelLogo")));
	if (pLabelLogo != NULL)
	{
		//string strBkImage = "image_" + 
		char cBkImage[512];
		memset(cBkImage, 0, 512);
		sprintf_s(cBkImage, 512, "eximage_%d.png", item.GetId());

		TCHAR tcBkImage[512];
		memset(tcBkImage, 0, 512);
		CharToTchar(cBkImage, tcBkImage);
		pLabelLogo->SetBkImage(tcBkImage);
	}

	// 交易所名字
	CLabelUI* pLabelName =static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelName")));
	if (pLabelName != NULL)
	{
		TCHAR tcName[512];
		memset(tcName, 0, 512);
		CharToTchar(item.GetName().c_str(), tcName);
		pLabelName->SetText(tcName);
	}

	// 认证 TODO:根据数量动态添加
	CLabelUI* pLabelTag1 = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelTag1")));
	if (pLabelTag1 != NULL)
	{
		//TCHAR tcTag1[32];
		//memset(tcTag1, 0, 32);
		//CharToTchar()
		pLabelTag1->SetText(_T("商务厅"));
	}

	CLabelUI* pLabelTag2 = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelTag2")));
	if (pLabelTag2 != NULL)
	{
		pLabelTag2->SetText(_T("金融办"));
	}

	// 关注人数
	CLabelUI* pLabelNum = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelNum")));
	if (pLabelNum != NULL)
	{
		int nNum = item.GetNum();

		char cNum[16];
		memset(cNum, 0, 16);
		itoa(nNum, cNum, 10);

		TCHAR tcNum[16];
		memset(tcNum, 0, 16);
		CharToTchar(cNum, tcNum);
		
		pLabelNum->SetText(tcNum);
	}

	Add(pListElement);
	int index = GetItemIndex(pListElement);
	return true;
}