#include "AnayListUI.h"
#include "StringUtil.h"

CAnayListUI::CAnayListUI(CPaintManagerUI& paint_manager):m_paintManager(paint_manager)
{
}


CAnayListUI::~CAnayListUI(void)
{
}


//bool CAnayListUI::SelectItem(int iIndex, bool bTakeFocus  = false )
//{
//	return CListUI::SelectItem(iIndex);
//}


bool CAnayListUI::AddItem(const CExchange& item)
{
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
	CDuiString strId;
	strId.Format(_T("%d"), item.GetId());
	pListElement->SetUserData(strId);

	// 交易所图标
	CLabelUI* pLabelLogo = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelLogo")));
	if (pLabelLogo != NULL)
	{
		//CDuiString strImg;
		//strImg.Format(_T("eximage_%d.png"), item.GetId());
		string strImg = item.GetImage();
		wstring wstrImg = s2ws(strImg);
		pLabelLogo->SetBkImage(wstrImg.c_str());
	}

	// 交易所名字
	CLabelUI* pLabelName =static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelName")));
	if (pLabelName != NULL)
	{
		wstring wstrName = s2ws(item.GetName());
		pLabelName->SetText(wstrName.c_str());
	}


	// 关注人数
	CLabelUI* pLabelNum = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pListElement, _T("labelNum")));
	if (pLabelNum != NULL)
	{
		int nNum = item.GetNum();
		CDuiString strNum;
		strNum.Format(_T("%d"), nNum);
		pLabelNum->SetText(strNum);
	}

	// 认证
	//CVerticalLayoutUI* pVerLayout = static_cast<CVerticalLayoutUI*>(m_paintManager.FindSubControlByName(pListElement, _T("verLayout"))); 
	//vector<string> vecAuth = item.GetAuth();
 //	int nCount = vecAuth.size();
	//for (int i = 0; i < nCount; i++)
	//{
	//	CLabelUI* pLabelAuth = new CLabelUI;
	//	//pLabelAuth->SetAttribute(_T("name"), _T("xx"));
	//	pLabelAuth->SetAttribute(_T("text"), s2ws(vecAuth[i]).c_str());
	//	pLabelAuth->SetAttribute(_T("bordersize"),		_T("1"));
	//	pLabelAuth->SetAttribute(_T("bordercolor"),		_T("#FF000000"));
	//	pLabelAuth->SetAttribute(_T("bkcolor"),			_T("#FFEDEFF2"));
	//	pLabelAuth->SetAttribute(_T("float"),			_T("true"));
	//	pLabelAuth->SetAttribute(_T("pos"),				_T("74,35,0,0"));

	//	//pLabelAuth->SetAttribute(_T("autocalcwidth"),	_T("true"));
	//	//pLabelAuth->SetAttribute(_T("showhtml"),		_T("true"));

	//	pLabelAuth->SetAutoCalcWidth(true);

	//	pLabelAuth->SetAttribute(_T("width"),			_T("100"));
	//	pLabelAuth->SetAttribute(_T("height"),			_T("15"));
	//	if (pVerLayout)
	//	{
	//		pVerLayout->Add(pLabelAuth);
	//	}
	//}

	Add(pListElement);

	return true;
}