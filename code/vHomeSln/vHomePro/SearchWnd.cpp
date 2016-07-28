#include "SearchWnd.h"
#include "ExTeacherListUI.h"
#include "ExchangeLogic.h"
#include "StringUtil.h"
#include "ResMsg.h"
#include "TeacherLogic.h"

CSearchWnd::CSearchWnd(const CExchange& ex)
{
	m_exchange = ex;
}


CSearchWnd::~CSearchWnd(void)
{
}


LPCTSTR CSearchWnd::GetWindowClassName() const
{
	return _T("SearchWindow");
}
CDuiString CSearchWnd::GetSkinFile()
{
	return _T("SearchWnd.xml");
}
CDuiString CSearchWnd::GetSkinFolder()
{
	return _T("");
}

CControlUI* CSearchWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("ExTeacherList")) == 0) // 财经分析
	{
		CExTeacherListUI* pList= new CExTeacherListUI(m_PaintManager);
		return pList;
	}
	return NULL;
}

void CSearchWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			this->Close();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnMin"))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		}
		else if (msg.pSender->GetName() == _T("btnCare")) // 关注讲师
		{
			CExTeacherListUI* pTeacherUI = static_cast<CExTeacherListUI*>(m_PaintManager.FindControl(_T("lstExTeacher")));
			CControlUI* pBtn = msg.pSender;
			CDuiString strName = pBtn->GetParent()->GetName();
			CContainerUI* pListElement = static_cast<CContainerUI*>(pBtn->GetParent());

			CDuiString strData = pListElement->GetUserData();
			string strId = StringFromLPCTSTR(strData.GetData());
			int nTeacherId = atoi(strId.c_str());
			OnCare(nTeacherId);
		}
		else if (msg.pSender->GetName() == _T("btnSearch")) // 搜索讲师
		{
			OnSearch();
		}
	}
	else if (msg.sType == _T("windowinit"))
	{
		OnPrepare(msg);
		return;
	}
}

void CSearchWnd::OnPrepare(TNotifyUI& msg)
{
	// 显示交易所信息
	ShowExchangeInfo();

	// 显示交易所讲师信息
	ShowAllExTeachers();
}

void CSearchWnd::ShowExchangeInfo()
{
	//CExchangeLogic exLogic;
	//CExchange exchange;
	//exchange.SetId(m_nExid);
	//string strMsg;
	//int nRes = exLogic.GetExchange(exchange, strMsg);
	//if (nRes != 0)
	//{
	//	return;
	//}

	CLabelUI* pLabelIcon = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelIcon")));
	if (pLabelIcon != NULL) // 头像
	{
		//CDuiString strImg;
		//strImg.Format(_T("eximage_%d.png"), m_exchange.GetId());

		//pLabelIcon->SetBkImage(strImg);
		string strImg = m_exchange.GetImage();
		wstring wstrImg = s2ws(strImg);
		pLabelIcon->SetBkImage(wstrImg.c_str());

		
	}

	CLabelUI* pLabelExcName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelExcName")));
	if (pLabelExcName != NULL)
	{
		wstring wstrName = s2ws(m_exchange.GetName());
		pLabelExcName->SetText(wstrName.c_str());
	}

	if (m_exchange.GetAuth().size() <= 0)
	{
		CLabelUI* pLabelAuth = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelAuth")));
		if (pLabelAuth)
		{
			pLabelAuth->SetVisible(false);
		}
	}
}

void CSearchWnd::ShowAllExTeachers()
{
	CExTeacherListUI* pTeacherList = static_cast<CExTeacherListUI*>(m_PaintManager.FindControl(_T("lstExTeacher")));
	if (pTeacherList != NULL)
	{
		if (pTeacherList->GetCount() > 0)
		{
			pTeacherList->RemoveAll();
		}
		if (m_vecTeacher.size() > 0)
		{
			m_vecTeacher.clear();
		}

		CExchangeLogic exLogic;

		string strMsg;
		string strToken = m_user.GetToken(); // 后期加入
		int nRes = exLogic.GetAllTeacherOfExc(m_vecTeacher, m_exchange.GetId(), strToken, strMsg);
		if (nRes != 0)
		{
			MessageBox(*this, _T("登录超时"), _T("V家金服"), 0);
			return;
		}
		int nSize = m_vecTeacher.size();
		for (int i = 0; i < nSize; i++)
		{
			CTeacher teacher = m_vecTeacher[i];
			pTeacherList->AddItem(teacher);
		}
	}
}

// 关注讲师
void CSearchWnd::OnCare(int nUid)
{
	CTeacherLogic teacherLogic;
	string strMsg;
	int nRes= teacherLogic.AddAttention(nUid, m_user.GetToken(), strMsg);
	if (nRes != 0)
	{
		wstring wstr = s2ws(strMsg);
		MessageBox(NULL, wstr.c_str(), _T("V家金服"),0);
		return;
	}
	MessageBox(NULL, _T("关注成功"), _T("V家金服"),0);
	return;
}

void CSearchWnd::OnSearch()
{
	CEditUI* pEditSearch = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editSearch")));
	if (pEditSearch)
	{
		CExTeacherListUI* pTeacherList = static_cast<CExTeacherListUI*>(m_PaintManager.FindControl(_T("lstExTeacher")));
		if (pTeacherList)
		{
			if (pTeacherList->GetCount() > 0)
			{
				pTeacherList->RemoveAll();
			}
		}
		CDuiString strKeyword = pEditSearch->GetText();
		if (strKeyword.GetLength() <= 0)
		{
			return;
		}
		wstring wstrKey = strKeyword;

		int nSize = m_vecTeacher.size();
		for (int i = 0; i < nSize; i++)
		{
			CTeacher teacher = m_vecTeacher[i];
			string strName = teacher.GetName();			
			wstring wstrName = s2ws(strName); 
			size_t nPos = wstrName.find_first_of(wstrKey, 0);
			if (nPos != string::npos)
			{
				pTeacherList->AddItem(teacher);
			}
		}
	}
}

//void CSearchWnd::SetExid(int nExid)
//{
//	m_nExid = nExid;
//}
//int CSearchWnd::GetExid()
//{
//	return m_nExid;
//}

void CSearchWnd::SetUser(const CUser& user)
{
	m_user = user;
}
CUser CSearchWnd::GetUser()
{
	return m_user;
}