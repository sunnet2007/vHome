#include "ActivityInfoWnd.h"
#include "ActivityLogic.h"
#include "StringUtil.h"
#include "AppConfig.h"

CActivityInfoWnd::CActivityInfoWnd(void)
{
	m_nId = 0;
}

CActivityInfoWnd::CActivityInfoWnd(int nId, const CUser& user)
{
	m_nId = nId;
	m_user = user;
}
CActivityInfoWnd::~CActivityInfoWnd(void)
{
}


LPCTSTR CActivityInfoWnd::GetWindowClassName() const
{
	return _T("ActivityInfoWnd");
}
CDuiString CActivityInfoWnd::GetSkinFile()
{
	return _T("ActivityInfoWnd.xml");
}
CDuiString CActivityInfoWnd::GetSkinFolder()
{
	return _T("");
}

void CActivityInfoWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			Close();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnApply"))	// 报名活动
		{
			//MessageBox(NULL, _T("btnApply"), _T(""), 0);
			OnBtnApply();
			return;
		}
	}
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
		return;
	}
	__super::Notify(msg);
}

void CActivityInfoWnd::OnPrepare(TNotifyUI& msg)
{
	CActivityLogic activityLogic;
	CActivity activity;
	activity.SetId(m_nId);
	string strMsg;
	int nRes = activityLogic.GetActivityInfo(m_user.GetToken(), activity, strMsg); // 获取活动信息
	if (nRes != 0)
	{
		MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
		return;
	}
	char cid[32];
	memset(cid, 0, 32);
	itoa(m_nId, cid, 10);
	string strId(cid);
	string strContent = CAppConfig::NOS_ACTIVITY_CONTENT + strId;
	wstring wstrContent = s2ws(strContent);
	CWebBrowserUI* pActiveXUI = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("webContent")));
	if (pActiveXUI)
	{
		pActiveXUI->NavigateUrl(wstrContent.c_str());
	}


	CLabelUI* pLabelImg = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelImg")));
	if (pLabelImg)
	{
		//CDuiString strImg;
		//strImg.Format(_T("activity_%d.png"), activity.GetId());
		string strImg = activity.GetImg();
		wstring wstrImg = s2ws(strImg);
		pLabelImg->SetBkImage(wstrImg.c_str());
	}
	CLabelUI* pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelName")));
	if (pLabelName)
	{
		string strTitle = activity.GetTitle();
		wstring wstrTitle = s2ws(strTitle);
		pLabelName->SetText(wstrTitle.c_str());
	}
	CLabelUI* pLabelStart = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelStart")));
	if (pLabelStart)
	{
		time_t tStart = activity.GetStart();
		tm tLocal;
		localtime_s(&tLocal, &tStart);
		char cTime[64];
		memset(cTime, 0, 64);
		strftime(cTime, 64, "%Y.%m.%d",&tLocal);
		string strStart(cTime);
		wstring wstrStart = s2ws(strStart);
		pLabelStart->SetText(wstrStart.c_str());
	}
	CLabelUI* pLabelEnd = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelEnd")));
	if (pLabelEnd)
	{
		time_t tEnd = activity.GetEnd();
		tm tLocal;
		localtime_s(&tLocal, &tEnd);
		char cTime[64];
		memset(cTime, 0, 64);
		strftime(cTime, 64, "%Y.%m.%d",&tLocal);
		string strEnd(cTime);
		wstring wstrEnd = s2ws(strEnd);
		pLabelEnd->SetText(wstrEnd.c_str());
	}

	CLabelUI* pLabelNum = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelNum")));
	if (pLabelNum)
	{
		CDuiString strNum;
		strNum.Format(_T("%d"), activity.GetNum());
		pLabelNum->SetText(strNum.GetData());
	}

	CButtonUI* pBtnApply = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnApply")));
	if (pBtnApply)
	{
		if (activity.GetMyStatus() == 1)
		{
			pBtnApply->SetVisible(false);
		}
	}
	return;
}

// 报名活动
void CActivityInfoWnd::OnBtnApply()
{
	CActivityLogic activityLogic;
	string strMsg;
	int nRes = activityLogic.ApplyActivity(m_user.GetToken(), m_nId, strMsg);
	if (nRes != 0)
	{
		MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
		return;
	}
	MessageBox(*this, _T("报名成功"), _T("V家金服"), 0);
}