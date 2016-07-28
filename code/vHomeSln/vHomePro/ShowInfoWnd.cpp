#include "ShowInfoWnd.h"
#include "StringUtil.h"
#include "TeacherLogic.h"
#include "ResMsg.h"
#include "ArtListUI.h"
#include "AppConfig.h"
#include "ArtDetailsWnd.h"

CShowInfoWnd::CShowInfoWnd(void)
{

}

CShowInfoWnd::CShowInfoWnd(int nSelectedTab, const CTeacher& teacher, const CUser& user)
{
	m_nSelectedTab = nSelectedTab;
	m_teacher = teacher;
	m_user = user;
}

CShowInfoWnd::CShowInfoWnd(int nSelectedTab)
{
	m_nSelectedTab = nSelectedTab;
}

CShowInfoWnd::~CShowInfoWnd(void)
{
}

LPCTSTR CShowInfoWnd::GetWindowClassName() const
{
	return _T("ShowInfoWindow");
}
CDuiString CShowInfoWnd::GetSkinFile()
{
	return _T("ShowInfoWnd.xml");
}
CDuiString CShowInfoWnd::GetSkinFolder()
{
	return _T("");
}

void CShowInfoWnd::InitTitle()
{
	// 窗口标题
	CLabelUI* pLabelIcon = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelIcon")));
	CLabelUI* pLabelNicName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelNicName"))); 
	CLabelUI* pLabelNum = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelNum"))); 
	CLabelUI* pLabelDesc = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelDesc"))); 
	if (pLabelIcon)
	{
		//CDuiString strImg;
		//strImg.Format(_T("teacherimage_%d.png"), m_teacher.GetUid());
		//pLabelIcon->SetBkImage(strImg);

		string strImg = m_teacher.GetImgThumb();
		wstring wstrImg = s2ws(strImg);
		pLabelIcon->SetBkImage(wstrImg.c_str());
	}
	if (pLabelNicName)
	{
		wstring wstrNicName = s2ws(m_teacher.GetName());
		pLabelNicName->SetText(wstrNicName.c_str());
	}
	if (pLabelNum)
	{
		CDuiString strName;
		strName.Format(_T("%d"), m_teacher.GetNum());
		pLabelNum->SetText(strName);
	}
	if (pLabelDesc)
	{
		if (m_teacher.GetDesc() != "无")
		{
			wstring wstrDesc = s2ws(m_teacher.GetDesc());
			pLabelDesc->SetText(wstrDesc.c_str());
		}
	}

	int nStar = m_teacher.GetStar();
	CHorizontalLayoutUI* pHorLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("horLayout")));
	for (int i = 0; i < nStar; i++)
	{
		CLabelUI* pLabelStar = new CLabelUI;
		pLabelStar->SetAttribute(_T("float"), _T("true"));
		CDuiString strPos;
		strPos.Format(_T("%d,%d,%d,%d"), 260+20*i, 30, 70+20*(i+1),50);
		pLabelStar->SetAttribute(_T("pos"),strPos);
		pLabelStar->SetBkImage(_T("file='lightstart.png' dest='3,0,16,19'"));
		if (pHorLayout)
		{
			pHorLayout->AddAt(pLabelStar, 7+i);
		}
	}


}
void CShowInfoWnd::InitPersonInfo()
{
	CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelName"))); 
	CLabelUI* pLabelSex = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelSex"))); 
	CLabelUI* pLabelQQ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelQq")));
	CLabelUI* pLabelEmail = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelEmail")));
	CLabelUI* pLabelSchool = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelSchool"))); 
	CLabelUI* pLabelGoodat = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelGoodat"))); 
	CRichEditUI* pRichEditDesc = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richDesc")));

	if (pLabelName)
	{
		string strName = m_teacher.GetName();
		wstring wstrName = s2ws(strName); 
		pLabelName->SetText(wstrName.c_str());
	}
	if (pLabelSex)
	{
		string strSex = m_teacher.GetSex();
		wstring wstrSex = s2ws(strSex);

		pLabelSex->SetText(wstrSex.c_str());
	}
	if (pLabelQQ)
	{
		string str = m_teacher.GetQq();
		wstring wstr = s2ws(str); 
		pLabelQQ->SetText(wstr.c_str());
	}
	if (pLabelEmail)
	{
		string str = m_teacher.GetEmail();
		wstring wstr = s2ws(str); 
		pLabelEmail->SetText(wstr.c_str());
	}
	if (pLabelSchool)
	{
		string str = m_teacher.GetSchool();
		wstring wstr = s2ws(str); 
		pLabelSchool->SetText(wstr.c_str());
	}
	vector<CSpecialty> vecGood = m_teacher.GetGoodat();
	int nSize = vecGood.size();
	string strGood = "";
	for (int i= 0; i < nSize; i++)
	{
		strGood += vecGood[i].GetName();
	}
	if (pLabelGoodat)
	{
		wstring wstr = s2ws(strGood);
		pLabelGoodat->SetText(wstr.c_str());
	}
	if (pRichEditDesc)
	{
		string str = m_teacher.GetDesc();
		wstring wstr = s2ws(str); 
		pRichEditDesc->SetText(wstr.c_str());
		pRichEditDesc->SetReadOnly(true);
	}
	CButtonUI* pBtnCare = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnCancelCare")));
	int nIfAddtention = m_teacher.GetIfAttention();	
	if (pBtnCare)
	{
		if (nIfAddtention == 0)
			pBtnCare->SetText(_T("关注"));
		else
			pBtnCare->SetText(_T("取消关注"));
	}


}
void CShowInfoWnd::InitArtList()
{
	CHorizontalLayoutUI* pTabHorLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("tabHorLayout")));
	CTeacherLogic teacherLogic;
	if (m_vecArt.size() > 0)
	{
		m_vecArt.clear();
	}
	if (m_vecCtgy.size() > 0)
	{
		m_vecCtgy.clear();
	}

	string strMsg;
	int nRes = teacherLogic.GetTeacherArts(0,0,m_teacher.GetUid(), m_user.GetToken(), m_vecArt, m_vecCtgy, strMsg);
	if (nRes != 0)
	{
		MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
		return;
	}
	int nSize = m_vecCtgy.size();
	int nArtCount = m_vecArt.size();
	for (int i = 0; i < nSize; i++)
	{
		CCategory cgty = m_vecCtgy[i];
		CDuiString strName;
		strName.Format(_T("%s"),  cgty.GetName().c_str());
		wstring wstrName = s2ws(cgty.GetName());

		string strImg = cgty.GetImg();
		wstring wstrImg = s2ws(strImg);
		//strImg.Format(_T("ctgyimage_%d.png"), cgty.GetId());

		CDuiString strPos;
		strPos.Format(_T("%d,%d,%d,%d"), 10+60*i, 0, 20+60*(i+1),20);

		COptionUI* pOption = new COptionUI();

		pOption->SetAttribute(_T("name"), wstrName.c_str());
		pOption->SetAttribute(_T("float"), _T("true"));
		pOption->SetAttribute(_T("pos"), strPos);
		pOption->SetAttribute(_T("width"), _T("60"));
		pOption->SetAttribute(_T("height"), _T("20"));
		pOption->SetAttribute(_T("bkcolor"), _T("#FFFFFFFF"));
		pOption->SetAttribute(_T("text"), wstrName.c_str());
		pOption->SetAttribute(_T("group"), _T("artCgty"));
		pOption->SetAttribute(_T("selectedbkcolor"), _T("#FFF36A09"));
		pOption->SetToolTip(wstrName.c_str());
		pOption->SetAttribute(_T("font"), _T("2"));
		CDuiString strUserData;
		strUserData.Format(_T("%d"), cgty.GetId());
		pOption->SetUserData(strUserData);
		m_vecOp.push_back(pOption);
		if (pTabHorLayout)
		{
			pTabHorLayout->Add(pOption);
		}
	}
	if (m_vecOp.size() > 0)
	{
		m_vecOp[0]->Selected(true);
	}


	CArtListUI* artList = static_cast<CArtListUI*>(m_PaintManager.FindControl(_T("lstTeacherArt")));
	if (artList->GetCount() > 0)
	{
		artList->RemoveAll();
	}
	for (int i = 0; i < nArtCount; i++)
	{
		artList->AddItem(m_vecArt[i]);
	}
}

void CShowInfoWnd::OnPrepare(TNotifyUI& msg)
{
	CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
	pTabControl->SelectItem(m_nSelectedTab);
	InitTitle();
	InitPersonInfo();
	InitArtList();
	
	// <Label name="labelOneStart" float="true" pos="260,50,0,0" width="20" height="20" bkimage="file='lightstart.png' dest='3,0,16,19'" />
	//<Label name="labelOneStart" float="true" pos="275,50,0,0" width="20" height="20" bkimage="file='lightstart.png' dest='3,0,16,19'" />
}

CControlUI* CShowInfoWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("ArtList")) == 0) // 讲师文章
	{
		CArtListUI* pArtList = new CArtListUI(m_PaintManager, 1);
		return pArtList;
	}
	return NULL;
}

void CShowInfoWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			this->Close();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnCancelCare"))
		{
			OnCancelCare();
			return;
		}
	}
	else if (msg.sType == _T("selectchanged"))
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
		if (msg.pSender->GetName() == _T("opPerInfo"))
		{
			pTabControl->SelectItem(0);
			return;
		}
		else if (msg.pSender->GetName() == _T("opArticle"))
		{
			pTabControl->SelectItem(1);
			return;
		}
		//CTabLayoutUI* pTabArt = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabArt")));
		int nSize = m_vecOp.size();
		for(int i = 0; i < nSize; i++)
		{
			if (msg.pSender->GetName() == m_vecOp[i]->GetName())
			{
				//pTabArt->SelectItem(i);
				CDuiString str = m_vecOp[i]->GetUserData();
				int nId = atoi(StringFromLPCTSTR(str.GetData()).c_str());
				CTeacherLogic teacherLogic;
				string strMsg;
				if (m_vecArt.size() > 0)
				{
					m_vecArt.clear();
				}
				int nRes = teacherLogic.GetTeacherArts(0,0,m_teacher.GetUid(), m_user.GetToken(), m_vecArt, m_vecCtgy, strMsg, nId);
				if (nRes != 0)
				{
					MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
					return;
				}
				CArtListUI* artList = static_cast<CArtListUI*>(m_PaintManager.FindControl(_T("lstTeacherArt")));
				if (artList->GetCount() > 0)
				{
					artList->RemoveAll();
				}
				int nArtCount = m_vecArt.size();
				for (int i = 0; i < nArtCount; i++)
				{
					artList->AddItem(m_vecArt[i]);
				}
				return;
			}
		}

	}
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
		return;
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMACTIVATE)
	{
		CArtListUI* pArtList = static_cast<CArtListUI*>(m_PaintManager.FindControl(_T("lstTeacherArt")));
		int nIndex = pArtList->GetItemIndex(msg.pSender);
		CListContainerElementUI* pListElement = static_cast<CListContainerElementUI*>(pArtList->GetItemAt(nIndex));
		CDuiString strId = pListElement->GetUserData();
		string id = StringFromLPCTSTR(strId.GetData());
		int nArtId = atoi(id.c_str());
		ShowArtDetailsWnd(nArtId);
		//string strUrl = CAppConfig::NOS_TEACHER_ARTICLEINFO;
		//wstring wstrUrl = s2ws(strUrl);
		//wstrUrl +=  strId.GetData();
		//ShellExecute(NULL,_T("open"), wstrUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
		return;
	}
	__super::Notify(msg);
}

void CShowInfoWnd::SetTeacher(const CTeacher& teacher)
{
	m_teacher = teacher;
}
CTeacher CShowInfoWnd::GetTeacher()
{
	return m_teacher;
}

// 取消关注
void CShowInfoWnd::OnCancelCare()
{
	CTeacherLogic teacherLogic;
	string strToken;
	string strMsg;

	int nIfAddtention = m_teacher.GetIfAttention();	

	if (nIfAddtention == 0)
	{
		int nRes = teacherLogic.DelAttention(m_teacher.GetUid(), strToken, strMsg);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}
		MessageBox(*this, _T("取消关注成功"), _T("V家金服"), 0);
	}
	else // 关注
	{
		int nRes = teacherLogic.AddAttention(m_teacher.GetUid(), strToken, strMsg);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}
		MessageBox(*this, _T("关注成功"), _T("V家金服"), 0);
	}




}

void CShowInfoWnd::ShowArtDetailsWnd(int nArtId)
{
	CArtDetailsWnd* artDetailWnd = new CArtDetailsWnd(nArtId, m_user);
	artDetailWnd->Create(*this, _T("ArtDetailWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	artDetailWnd->ShowWindow();
	int nScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rc;
	::GetWindowRect(*artDetailWnd, &rc);
	int nWndWidth = rc.right-rc.left;
	int nWndHeight = rc.bottom - rc.top;
	int x = (nScreenWidth-nWndWidth)/2;
	int y = (nScreenHeight - nWndHeight)/2;
	::MoveWindow(*artDetailWnd, x, y, nWndWidth, nWndHeight,TRUE);
}