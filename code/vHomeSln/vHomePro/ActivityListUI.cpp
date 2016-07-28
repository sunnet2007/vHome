#include "ActivityListUI.h"


#include "AnayListUI.h"
#include "StringUtil.h"

CActivityListUI::CActivityListUI(CPaintManagerUI& paint_manager):m_paintManager(paint_manager)
{
}


CActivityListUI::~CActivityListUI(void)
{
}


//bool CActivityListUI::SelectItem(int iIndex, bool bTakeFocus  = false )
//{
//	return true;
//}


bool CActivityListUI::AddItem(const CActivity& item)
{
	TCHAR szBuf[126];
	memset(szBuf, 0, 126);
	CListContainerElementUI* pListElement = NULL;
	if (!m_dlgBuilder.GetMarkup()->IsValid())
	{
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("ActiveList.xml"), (UINT)0, NULL, &m_paintManager));
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

	CLabelUI* pLabelLogo = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelLogo")));
	if (pLabelLogo != NULL)
	{
		//string strBkImage = "image_" + 
		//char cBkImage[512];
		//memset(cBkImage, 0, 512);
		//sprintf_s(cBkImage, 512, "activity_%d.png", item.GetId());

		//TCHAR tcBkImage[512];
		//memset(tcBkImage, 0, 512);
		//CharToTchar(cBkImage, tcBkImage);

		string strImg = item.GetImg();
		wstring wstrImg = s2ws(strImg);
		pLabelLogo->SetBkImage(wstrImg.c_str());
	}

	CLabelUI* pLabelName =static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelName")));
	if (pLabelName != NULL)
	{
		TCHAR tcName[512];
		memset(tcName, 0, 512);
		CharToTchar(item.GetTitle().c_str(), tcName);
		pLabelName->SetText(tcName);
	}

	CLabelUI* pLabelContent = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelContent")));
	if (pLabelContent != NULL)
	{
		TCHAR tcName[512];
		memset(tcName, 0, 512);
		CharToTchar(item.GetContent().c_str(), tcName);
		pLabelContent->SetText(tcName);
	}

	// 关注人数
	CLabelUI* pLabelNum = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelNum")));
	if (pLabelNum != NULL)
	{
		//int nNum = item.GetNum();

		CDuiString strNum;
		strNum.Format(_T("%d"), item.GetNum());

		pLabelNum->SetText(strNum);
	}

	Add(pListElement);
	return true;
}
