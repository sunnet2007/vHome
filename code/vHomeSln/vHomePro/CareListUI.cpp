#include "CareListUI.h"
#include "StringUtil.h"

CCareListUI::CCareListUI(CPaintManagerUI& paint_manager):m_paintManager(paint_manager)
{
}


CCareListUI::~CCareListUI(void)
{
}



void CCareListUI::DoEvent(TEventUI& event)
{
	//if (event.Type == UIEVENT_RBUTTONDOWN)
	//{
	//	CPoint pt(0,0);
	//	GetCursorPos(&pt);
	//	//CMenuWnd* pMenu = new CMenuWnd;
	//	//pMenu->Create(*this, _T("MenuWnd"),UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, LOWORD(lParam), HIWORD(wParam));
	//	CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("MenuWnd.xml"),pt, &m_paintManager);
	//	pMenu->ShowModal();
	//}


	return __super::DoEvent(event);
}

//bool CCareListUI::SelectItem(int iIndex, bool bTakeFocus  = false )
//{
//	return true;
//}


bool CCareListUI::AddItem(const CTeacher& teacher)
{
	TCHAR szBuf[126];
	memset(szBuf, 0, 126);
	CListContainerElementUI* pListElement = NULL;
	if (!m_dlgBuilder.GetMarkup()->IsValid())
	{
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("CareList.xml"), (UINT)0, NULL, &m_paintManager));
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
	CDuiString strId;
	strId.Format(_T("%d"), teacher.GetUid());
	pListElement->SetUserData(strId);

	// 讲师头像
	CLabelUI* pLabelImg = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelImg")));
	if (pLabelImg != NULL)
	{
		string strImg = teacher.GetImgThumb();
		wstring wstrImg = s2ws(strImg); 
		pLabelImg->SetBkImage(wstrImg.c_str());
	}

	// 讲师名字
	CLabelUI* pLabelName =static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelName")));
	if (pLabelName != NULL)
	{
		wstring wstr = s2ws(teacher.GetName());
		pLabelName->SetText(wstr.c_str());
	}

	// 个性签名
	CLabelUI* pLabelSign = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelSign")));
	if (pLabelSign != NULL)
	{
		wstring wstr = s2ws(teacher.GetDesc());
		pLabelSign->SetText(wstr.c_str());
		//pLabelSign->SetVisible(false);
	}

	CVerticalLayoutUI* pVLayout = static_cast<CVerticalLayoutUI*>(m_paintManager.FindSubControlByName(pListElement, _T("vLayout")));
	if (pVLayout)
	{
		// 讲师星级
		int nStars = teacher.GetStar();
		for (int i = 0; i < nStars; i++)
		{
			CLabelUI* pLabelStar = new CLabelUI;
			pLabelStar->SetAttribute(_T("float"), _T("true"));
			CDuiString strPos;
			strPos.Format(_T("%d,10,0,0"), 160+(i*15));
			pLabelStar->SetAttribute(_T("pos"), strPos);
			pLabelStar->SetAttribute(_T("width"), _T("13"));
			pLabelStar->SetAttribute(_T("height"), _T("13"));
			pLabelStar->SetAttribute(_T("bkimage"), _T("file='lightstart.png' source='0,3,13,16'"));
			pVLayout->Add(pLabelStar);

			//pVLayout->AddAt(pLabelStar, i+2);
		}
	}


	//// 认证 TODO:根据数量动态添加
	//CLabelUI* pLabelTag1 = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelTag1")));
	//if (pLabelTag1 != NULL)
	//{
	//	//TCHAR tcTag1[32];
	//	//memset(tcTag1, 0, 32);
	//	//CharToTchar()
	//	pLabelTag1->SetText(_T("商务厅"));
	//}

	//CLabelUI* pLabelTag2 = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelTag2")));
	//if (pLabelTag2 != NULL)
	//{
	//	pLabelTag2->SetText(_T("金融办"));
	//}

	//// 关注人数
	//CLabelUI* pLabelNum = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelNum")));
	//if (pLabelNum != NULL)
	//{
	//	int nNum = item.GetNum();

	//	char cNum[16];
	//	memset(cNum, 0, 16);
	//	itoa(nNum, cNum, 10);

	//	TCHAR tcNum[16];
	//	memset(tcNum, 0, 16);
	//	CharToTchar(cNum, tcNum);

	//	pLabelNum->SetText(tcNum);
	//}

	Add(pListElement);
	int index = GetItemIndex(pListElement);
	return true;
}
