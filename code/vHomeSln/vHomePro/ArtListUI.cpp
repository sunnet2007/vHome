#include "ArtListUI.h"
#include "StringUtil.h"

CArtListUI::CArtListUI(CPaintManagerUI& paint_manager, int nType):m_paintManager(paint_manager)
{
	m_nType = nType;
}


CArtListUI::~CArtListUI(void)
{
}

//bool CArtListUI::SelectItem(int iIndex, bool bTakeFocus  = false )
//{
//	return true;
//}


bool CArtListUI::AddItem(const CArticle& item)
{
	TCHAR szBuf[126];
	memset(szBuf, 0, 126);
	CListContainerElementUI* pListElement = NULL;
	if (!m_dlgBuilder.GetMarkup()->IsValid())
	{
		if (m_nType == 0)
		{
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("ArtList.xml"), (UINT)0, NULL, &m_paintManager));
		}
		else
		{
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("ArtList2.xml"), (UINT)0, NULL, &m_paintManager));
		}
	
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
		CHorizontalLayoutUI* pLayout = static_cast<CHorizontalLayoutUI*>(m_paintManager.FindSubControlByName(pListElement, _T("horLayoutLogo")));
		if (item.GetImgThumb() == "")
		{
			pLayout->SetVisible(false);
		}
		else
		{
			pLayout->SetVisible(true);
			//string strBkImage = "image_" + 
			//char cBkImage[512];
			//memset(cBkImage, 0, 512);
			//sprintf_s(cBkImage, 512, "artimage_%d.png", item.GetId());

			//TCHAR tcBkImage[512];
			//memset(tcBkImage, 0, 512);
			//CharToTchar(cBkImage, tcBkImage);
			string strImg = item.GetImgThumb();
			wstring wstrImg = s2ws(strImg); 
			pLabelLogo->SetBkImage(wstrImg.c_str());

		}

	}
	CLabelUI* pLabelTitle =static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelTitle")));
	if (pLabelTitle != NULL)
	{
		wstring strTitle = s2ws(item.GetTitle());
		pLabelTitle->SetText(strTitle.c_str());
	}

	CRichEditUI* pEditContent = static_cast<CRichEditUI*>(m_paintManager.FindSubControlByName(pListElement, _T("editContent")));
	if (pEditContent != NULL)
	{
		wstring strAbout = s2ws(item.GetAbout());
		pEditContent->SetText(strAbout.c_str());
	}



	Add(pListElement);
	return true;
}
