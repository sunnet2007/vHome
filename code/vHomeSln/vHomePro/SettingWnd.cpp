#include "SettingWnd.h"
#include "TeacherLogic.h"
#include "Article.h"
#include "StringUtil.h"
#include "ExchangeLogic.h"
#include "UserManagerLogic.h"
#include "Common/Utility/base_string.h"
#include "AppConfig.h"

CSettingWnd::CSettingWnd(void)
{
	m_nSel = 0;
	m_strName = "";
	m_strImg = "";
}

CSettingWnd::CSettingWnd(int nSel, const CUser& user)
{
	m_nSel = nSel;
	m_user = user;
	m_strName = "";
	m_strImg = "";
}
CSettingWnd::CSettingWnd(int nSel, const CUser& user, const string& strName)
{
	m_nSel = nSel;
	m_user = user;
	m_strName = strName;
	m_strImg = "";
}

CSettingWnd::~CSettingWnd(void)
{
}

LPCTSTR CSettingWnd::GetWindowClassName() const
{
	return _T("SettingWindow");
}
CDuiString CSettingWnd::GetSkinFile()
{
	return _T("SettingWnd.xml");
}
CDuiString CSettingWnd::GetSkinFolder()
{
	return _T("");
}

// 处理学生界面
void CSettingWnd::HandleStuInterface()
{
	CHorizontalLayoutUI* pOptionLay = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("horLayoutOption")));
	CHorizontalLayoutUI* pTabLay = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("horLayoutTab")));
	if (pOptionLay)
	{
		pOptionLay->SetVisible(false);
	}
	if (pTabLay)
	{
		pTabLay->SetVisible(false);
	}
	CVerticalLayoutUI* pTeacherLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("teacherVerLayout")));
	if (pTeacherLayout)
	{
		pTeacherLayout->SetVisible(false);
	}
	
	CHorizontalLayoutUI* pHorTeacherLay = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("horLayoutTeacher")));
	if (pHorTeacherLay)
	{
		pHorTeacherLay->SetVisible(false);
	}

	CUserManagerLogic userLogic;
	string strMsg;
	int nRes = userLogic.GetUserInfo(m_user, strMsg);
	if (nRes != 0)
	{
		MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
		return;
	}

	CLabelUI* pLabelImg = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelStuIcon")));
	if (pLabelImg)
	{
		//CDuiString strImg;
		//strImg.Format(_T("icon_%d.png"), m_user.GetUid());
		//pLabelImg->SetBkImage(strImg);
		string strImg = m_user.GetIcon();
		wstring wstrImg = s2ws(strImg);
		pLabelImg->SetBkImage(wstrImg.c_str());
	}

	CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelStuName")));
	if(pLabelName)
	{
		wstring strName = s2ws(m_user.GetNicName());
		pLabelName->SetText(strName.c_str());
	}



}

// 处理讲师界面
void CSettingWnd::HandleTeacherInterface()
{

	string strMsg;
	CTeacherLogic teacherLogic;

	CLabelUI* pLabelImg = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelIcon")));
	CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelName")));
	CLabelUI* pLabelNum = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelNum")));
	CLabelUI* pLabelDesc = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelDesc")));
	if (pLabelImg)
	{
		string strImg = m_user.GetIcon();
		wstring wstrImg = s2ws(strImg);
		pLabelImg->SetBkImage(wstrImg.c_str());
	}
	if (pLabelName)
	{
		wstring wstr = s2ws(m_user.GetNicName());
		pLabelName->SetText(wstr.c_str());
	}	
		
	CVerticalLayoutUI* pStuLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("stuVerLayout")));
	if (pStuLayout)
	{
		pStuLayout->SetVisible(false);
	}

	vector<CCategory> vecCtgy;
	string strExchange;
	int nRes = teacherLogic.GetArtCat(m_user.GetToken(), strExchange, vecCtgy, strMsg);
	if (nRes != 0)
	{
		wstring wstr = s2ws(strMsg);
		MessageBox(NULL, wstr.c_str(), _T("V家金服"), 0);
		return;
	}
	CComboUI* pComboClass = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comboClass")));
	if (pComboClass)
	{
		int nSize = vecCtgy.size();
		for (int i = 0; i < nSize; i++)
		{
			CCategory ctgy = vecCtgy[i];
			CListLabelElementUI* pElement = new CListLabelElementUI;
			CDuiString strId;
			strId.Format(_T("%d"), ctgy.GetId());
			pElement->SetUserData(strId.GetData());
			pElement->SetText(s2ws(ctgy.GetName()).c_str());
			pComboClass->Add(pElement);
		}
		pComboClass->SelectItem(0);
	}

	vector<CSpecialty> vecSpl;
	//CTeacher teacher;
	nRes = teacherLogic.GetTeacherInfo(m_teacher, vecSpl, m_user.GetToken(), strMsg);
	if (nRes != 0)
	{
		wstring wstr = s2ws(strMsg);
		MessageBox(NULL, wstr.c_str(), _T("V家金服"), 0);
	}

	CComboUI* pComboGoodat = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comboGoodat")));
	if (pComboGoodat)
	{	
		int nSize = vecSpl.size();
		for (int i = 0; i < nSize; i++)
		{
			CSpecialty spl = vecSpl[i];
			CListLabelElementUI* pExElement = new CListLabelElementUI;
			CDuiString strId;
			strId.Format(_T("%d"), spl.GetId());
			pExElement->SetUserData(strId.GetData());
			pExElement->SetText(s2ws(spl.GetName()).c_str());
			pComboGoodat->Add(pExElement);
		}
	}
	string strExName;

	CExchangeLogic exLogic;
	vector<CExchange> vecEx;
	nRes = exLogic.GetExchanges(vecEx, strMsg);
	if (nRes != 0)
	{
		wstring wstr = s2ws(strMsg);
		MessageBox(NULL, wstr.c_str(), _T("V家金服"), 0);
		return;
	}

	CComboUI* pComboExchange = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comboExchange")));
	if (pComboExchange)
	{	
		int nSize = vecEx.size();
		for (int i = 0; i < nSize; i++)
		{
			CExchange ex = vecEx[i];
			CListLabelElementUI* pExElement = new CListLabelElementUI;
			CDuiString strId;
			strId.Format(_T("%d"), ex.GetId());
			pExElement->SetUserData(strId.GetData());
			pExElement->SetText(s2ws(ex.GetName()).c_str());
			pComboExchange->Add(pExElement);
			if (m_teacher.GetExid() == ex.GetId())
			{
				strExName = ex.GetName();
			}
		}

		pComboExchange->SelectItem(0);
	}
	
	CEditUI* pEditExchange = static_cast<CEditUI*>( m_PaintManager.FindControl(_T("editExchange")));
	if (pEditExchange)
	{
		wstring wstr = s2ws(strExName);
		pEditExchange->SetText(wstr.c_str());
	}

	if (pLabelDesc)
	{
		wstring wstr = s2ws(m_teacher.GetDesc());
		pLabelDesc->SetText(wstr.c_str());
	}
}

void CSettingWnd::OnPrepare(TNotifyUI& msg)
{
	CEditUI* pEditNicName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editNicName")));
	CEditUI* pEditName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editName")));
	CEditUI* pEditQQ = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editQQ")));
	CEditUI* pEditEmail = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editEmail")));
	CComboUI* pComboSex = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("ComboSex")));

	CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
	pTabControl->SelectItem(m_nSel);
	if (m_user.GetType() == 0)	// 学员
	{
		 // 处理学生界面
		HandleStuInterface();
		if (pEditNicName)
		{
			wstring wstr = s2ws(m_user.GetNicName());
			pEditNicName->SetText(wstr.c_str());
		}
		if (pEditName)
		{
			wstring wstr = s2ws(m_user.GetName());
			pEditName->SetText(wstr.c_str());
		}
		if (pEditQQ)
		{
			wstring wstr = s2ws(m_user.GetQq());
			pEditQQ->SetText(wstr.c_str());
		}
		if (pEditEmail)
		{
			wstring wstr = s2ws(m_user.GetEmail());
			pEditEmail->SetText(wstr.c_str());
		}
		if (pComboSex)
		{
			if (m_user.GetSex() == "女")
			{
				pComboSex->SelectItem(1);
			}
			else
			{
				pComboSex->SelectItem(0);
			}
		}
	}
	else
	{
		// 处理讲师界面
		HandleTeacherInterface();
		if (pEditNicName)
		{
			wstring wstr = s2ws(m_teacher.GetNicName());
			pEditNicName->SetText(wstr.c_str());
		}
		if (pEditName)
		{
			wstring wstr = s2ws(m_teacher.GetName());
			pEditName->SetText(wstr.c_str());
		}
		if (pEditQQ)
		{
			wstring wstr = s2ws(m_teacher.GetQq());
			pEditQQ->SetText(wstr.c_str());
		}
		if (pEditEmail)
		{
			wstring wstr = s2ws(m_teacher.GetEmail());
			pEditEmail->SetText(wstr.c_str());
		}
		if (pComboSex)
		{
			if (m_teacher.GetSex() == "女")
			{
				pComboSex->SelectItem(1);
			}
			else
			{
				pComboSex->SelectItem(0);
			}
		}
		CEditUI* pEditSchool = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editSchool")));
		CEditUI* pEditExchange = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editExchange")));
		CEditUI* pEditGoodat = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editGoodat")));
		CEditUI* pEditDesc = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editDesc")));
		if (pEditSchool)
		{
			wstring wstr = s2ws(m_teacher.GetSchool());
			pEditSchool->SetText(wstr.c_str());
		}
		if (pEditExchange)
		{
			wstring wstr = s2ws(m_teacher.GetExName());
			pEditExchange->SetText(wstr.c_str());
		}
		if (pEditGoodat)
		{
			vector<CSpecialty> vecGoodat = m_teacher.GetGoodat();
			int nSize = vecGoodat.size();
			wstring wstr = _T("");
			string str = "";
			CDuiString strId = _T("");
			for (int i = 0; i < nSize; i++)
			{
				//str  += vecGoodat[i].GetName();
				if (str == "")
				{
					str = vecGoodat[i].GetName();
				}
				else
				{
					str = (str+ "," + vecGoodat[i].GetName());
				}
				if (strId == _T(""))
				{
					strId.Format(_T("%d"), vecGoodat[i].GetId());
				}
				else
				{
					strId.Format(_T("%s,%d"), strId.GetData(), vecGoodat[i].GetId());
				}
			}
			wstr = s2ws(str);
			pEditGoodat->SetText(wstr.c_str());
			pEditGoodat->SetUserData(strId);
		}
		if (pEditDesc)
		{
			wstring wstr = s2ws(m_teacher.GetDesc());
			pEditDesc->SetText(wstr.c_str());
		}	
	}

	CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
	if (pTab)
	{
		pTab->SelectItem(m_nSel);
	}

	COptionUI* pOption = NULL;
	if (m_nSel == 0)
	{
		pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opEdit")));
	}
	else if (m_nSel == 1)
	{
		pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opSecurity")));
	}
	else
	{
		pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("opNew")));
	}
	pOption->Selected(true);
		
		

	return;
}

void CSettingWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			Close();
			return;
		}
		else if(msg.pSender->GetName() == _T("btnRelease"))
		{
			 OnRelease();
			 return;
		}
		else if(msg.pSender->GetName() == _T("btnUpload"))
		{
			OnUpload();
			return;
		}
		else if(msg.pSender->GetName() == _T("btnApply"))
		{
			OnApply();
			return;
		}
		else if(msg.pSender->GetName() == _T("btnPhoto"))
		{
			 OnPhoto();
			 return;
		}
	}
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
		return;
	}
	else if (msg.sType == _T("selectchanged"))
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
		if (msg.pSender->GetName() == _T("opEdit"))
		{
			pTabControl->SelectItem(0);
			return;
		}
		else if (msg.pSender->GetName() == _T("opSecurity"))
		{
			pTabControl->SelectItem(1);
			return;
		}
		else if (msg.pSender->GetName() == _T("opNew"))
		{
			pTabControl->SelectItem(2);
			return;
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		CEditUI* pEditGoodat = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editGoodat")));
		CComboUI* pComboGoodat = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comboGoodat")));

		if( msg.pSender->GetName() == _T("comboGoodat") )
		{
			if (pEditGoodat)
			{
				CDuiString str = pEditGoodat->GetText();
				if (str == _T(""))
				{
					str = pComboGoodat->GetText();
				}
				else
				{
					str = (str+_T(",") + pComboGoodat->GetText());
				}
				CDuiString strUserData = pEditGoodat->GetUserData();
				int nIndex = pComboGoodat->GetCurSel();
				CListLabelElementUI* pCurSelItem = static_cast<CListLabelElementUI*>( pComboGoodat->GetItemAt(nIndex));
				CDuiString strCur = pCurSelItem->GetUserData();
				if (strUserData == _T(""))
				{
					strUserData = strCur;
				}
				else
				{
					strUserData.Format(_T("%s,%s"), strUserData.GetData(), strCur.GetData());
				}
				pEditGoodat->SetText(str);
				pEditGoodat->SetUserData(strUserData);
			}

		}
	}
	return __super::Notify(msg);
}

void CSettingWnd::OnPhoto()
{
	string strDest = CAppConfig::UI_FILE_PATH + "\\art.png";
	m_strArtImg = LoadMyImage(strDest);
}

// 发表文章
void CSettingWnd::OnRelease()
{
	// 文章标题
	CEditUI* pEditTitle = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editTitle")));
	string strTitle;
	if (pEditTitle)
	{
		//strTitle = StringFromLPCTSTR(pEditTitle->GetText().GetData());
		CBaseStringU uStr = pEditTitle->GetText();
		strTitle = uStr.GetStr();
		
	}

	// 文章内容
	string strContent;
	CRichEditUI* pRichEditContent = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("editContent")));
	if (pRichEditContent)
	{
		//strContent = StringFromLPCTSTR(pRichEditContent->GetText().GetData());
		CBaseStringU uStr = pRichEditContent->GetText();
		strContent = uStr.GetStr();
	}

	// 交易所
	string strExid;
	CComboUI* pComboEx = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comboExchange")));
	if (pComboEx)
	{
		int nIndex = pComboEx->GetCurSel();
		CListLabelElementUI* pElement = static_cast<CListLabelElementUI*>(pComboEx->GetItemAt(nIndex));
		strExid = StringFromLPCTSTR(pElement->GetUserData().GetData());
	}

	// 所属类型
	string strCtgyId;
	CComboUI* pComboCtgy = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comboClass")));
	if (pComboCtgy)
	{
		int nIndex = pComboCtgy->GetCurSel();
		CListLabelElementUI* pElement = static_cast<CListLabelElementUI*>(pComboCtgy->GetItemAt(nIndex));
		//pComboCtgy->GetItemIndex()
		strCtgyId = StringFromLPCTSTR(pElement->GetUserData().GetData());
	}

	CArticle art;
	art.SetTitle(strTitle);
	art.SetContent(strContent);
	art.SetImgThumb(m_strArtImg);
	art.SetExid(atoi(strExid.c_str()));
	art.SetCtgyId(atoi(strCtgyId.c_str()));

	CTeacherLogic teacherLogic;
	string strMsg;
	int nRes = teacherLogic.ReleaseArt(m_user.GetToken(), art, strMsg);
	if (nRes != 0)
	{
		wstring wstr = s2ws(strMsg);
		MessageBox(NULL, wstr.c_str(), _T("V家金服"), 0);
		return;
	}
	MessageBox(NULL, _T("发布成功"), _T("V家金服"), 0);
}

// 上传照片
void CSettingWnd::OnUpload()
{
	//char fileName[256];
	//ZeroMemory(fileName, 256);
	//m_user.GetIcon()
	//sprintf(fileName, "//icon_%d.png", m_user.GetUid());
	string strDest = CAppConfig::UI_FILE_PATH + "\\"+ m_user.GetIcon();

	m_strImg = LoadMyImage(strDest);

	CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelImg")));
	if (pLabel)
	{
		string strFileName = m_user.GetIcon();
		wstring szFileName = s2ws(strFileName);
		pLabel->SetBkImage(szFileName.c_str());
	}

}

// 确定修改
void CSettingWnd::OnApply()
{
	CEditUI* pEditNicName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editNicName")));
	string strNicName;
	if (pEditNicName)
	{
		CBaseStringU uStr = pEditNicName->GetText();
		strNicName = uStr.GetStr();
		//strNicName = StringFromLPCTSTR(pEditNicName->GetText().GetData());
	}

	CEditUI* pEditName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editName")));
	string strName;
	if (pEditName)
	{
		CBaseStringU uStr = pEditName->GetText();
		strName = uStr.GetStr();
		//strName = StringFromLPCTSTR(pEditName->GetText().GetData());
	}

	CEditUI* pEditQQ = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editQQ")));
	string strQQ;
	if (pEditQQ)
	{
		//strQQ = StringFromLPCTSTR(pEditQQ->GetText().GetData());
		CBaseStringU uStr = pEditQQ->GetText();
		strQQ = uStr.GetStr();
	}

	CEditUI* pEditEmail = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editEmail")));
	string strEmail;
	if (pEditEmail)
	{
		//strEmail = StringFromLPCTSTR(pEditEmail->GetText().GetData()) ;
		CBaseStringU uStr = pEditEmail->GetText();
		strEmail = uStr.GetStr();
	}

	CComboUI* pComboSex = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("ComboSex")));
	string strSex;
	if (pComboSex)
	{
		int nIndex = pComboSex->GetCurSel();
		CListLabelElementUI* pElement = static_cast<CListLabelElementUI*>(pComboSex->GetItemAt(nIndex));
		if (pElement)
		{
			CBaseStringU uStr = pElement->GetText();
			strSex = uStr.GetStr();
		}
	}
	CRichEditUI* pEditDesc = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("editDesc")));
	string strDesc;
	if (pEditDesc)
	{
		CBaseStringU uStr = pEditDesc->GetText();
		strDesc = uStr.GetStr();
	}

	string strMsg;
	if (m_user.GetType() == 0) // 学生
	{	
		m_user.SetNicName(strNicName);
		m_user.SetName(strName);
		m_user.SetEmail(strEmail);
		m_user.SetQq(strQQ);
		m_user.SetSex(strSex);
		CUserManagerLogic userLogic;
		int nRes = userLogic.ModifyUserInfo(m_user, strMsg);
		if (nRes != 0)
		{
			wstring wstr = s2ws(strMsg);
			MessageBox(NULL, wstr.c_str(), _T("V家金服"), 0);
			return;
		}
		CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelStuName")));
		if(pLabelName)
		{
			//wstring strName = s2ws(m_user.GetNicName());
			pLabelName->SetText(pEditNicName->GetText());
		}

	}
	else // 讲师
	{
		CEditUI* pEditSchool = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editSchool")));
		string strSchool;
		if (pEditSchool)
		{
			CBaseStringU uStr = pEditSchool->GetText();
			strSchool = uStr.GetStr();

			//strSchool = StringFromLPCTSTR(pEditSchool->GetText().GetData());
		}

		CEditUI* pGoodat = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editGoodat")));
		CDuiString strIds;
		if (pGoodat)
		{
			strIds = pGoodat->GetUserData();
		}
		CTeacher teacher;
		teacher.SetNicName(strNicName);
		teacher.SetName(strName);
		teacher.SetImgThumb(m_strImg);
		teacher.SetSex(strSex);
		teacher.SetEmail(strEmail);
		teacher.SetQq(strQQ);
		teacher.SetSchool(strSchool);
		teacher.SetDesc(strDesc);
		//teacher.SetGoodat(nullptr);
		string strGoodatIds = StringFromLPCTSTR(strIds.GetData());
		teacher.SetGoodatIds(strGoodatIds);

		CTeacherLogic teacherLogic;
		int nRes = teacherLogic.ModifyTeacherInfo(teacher, m_user.GetToken(), strMsg);
		if (nRes != 0)
		{
			wstring wstr = s2ws(strMsg);
			MessageBox(NULL, wstr.c_str(), _T("V家金服"), 0);
			return;
		}
		CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelName")));
		if (pLabelName)
		{
			wstring wstr = s2ws(m_teacher.GetNicName());
			pLabelName->SetText(pEditNicName->GetText());
		}	
		
		CLabelUI* pLabelIcon = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelIcon")));
		if (pLabelIcon)
		{
			string strIcon = m_user.GetIcon();
			wstring wstrIcon = s2ws(strIcon);
			pLabelIcon->SetBkImage(wstrIcon.c_str());
		}
		MessageBox(NULL, _T("修改成功"), _T("V家金服"), 0);
	}


}