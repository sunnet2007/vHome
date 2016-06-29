#include "SearchWnd.h"
#include "ExTeacherListUI.h"
#include "ExchangeLogic.h"
#include "StringUtil.h"
#include "ResMsg.h"
#include "TeacherLogic.h"

CSearchWnd::CSearchWnd(void)
{
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
	if (_tcsicmp(pstrClass, _T("ExTeacherList")) == 0) // �ƾ�����
	{
		CExTeacherListUI* pList= new CExTeacherListUI(m_PaintManager);
		CDuiString strName =  pList->GetName();
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
		if (msg.pSender->GetName() == _T("btnCare")) // ��ע��ʦ
		{
			CExTeacherListUI* pTeacherUI = static_cast<CExTeacherListUI*>(m_PaintManager.FindControl(_T("lstExTeacher")));
			CControlUI* pBtn = msg.pSender;
			CDuiString strName = pBtn->GetParent()->GetName();
			CContainerUI* pListElement = static_cast<CContainerUI*>(pBtn->GetParent());

			CDuiString strData = pListElement->GetUserData();
			string strId = StringFromLPCTSTR(strData.GetData());
			int nTeacherId = atoi(strId.c_str());
			OnCare(nTeacherId);
			//MessageBox(NULL, strData, _T(""), 0);
		}
		else if (msg.pSender->GetName() == _T("btnTalk")) // �뽲ʦ��̸
		{
			MessageBox(NULL, _T("��̸"), _T(""), 0);
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
	// ��ʾ��������Ϣ
	ShowExchangeInfo();

	// ��ʾ��������ʦ��Ϣ
	ShowAllExTeachers();
}

void CSearchWnd::ShowExchangeInfo()
{
	CExchangeLogic exLogic;
	CExchange exchange;
	exchange.SetId(m_nExid);
	CResMsg msg;
	exLogic.GetExchange(exchange, msg);

	CLabelUI* pLabelIcon = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelIcon")));
	if (pLabelIcon != NULL) // ͷ��
	{
		char cBkImage[512];
		memset(cBkImage, 0, 512);
		sprintf_s(cBkImage, 512, "eximage_%d.png", exchange.GetId());

		TCHAR tcBkImage[512];
		memset(tcBkImage, 0, 512);
		CharToTchar(cBkImage, tcBkImage);

		pLabelIcon->SetBkImage(tcBkImage);
	}

	CLabelUI* pLabelExcName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelExcName")));
	if (pLabelExcName != NULL)
	{
		TCHAR tcName[512];
		memset(tcName, 0, 512);
		CharToTchar(exchange.GetName().c_str(), tcName);
		pLabelExcName->SetText(tcName);
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

		CExchangeLogic exLogic;
		vector<CTeacher> vecTeacher;
		CResMsg msg;
		string strToken = m_user.GetToken(); // ���ڼ���
		exLogic.GetAllTeacherOfExc(vecTeacher, m_nExid, strToken, msg);
		int nSize = vecTeacher.size();
		for (int i = 0; i < nSize; i++)
		{
			CTeacher teacher = vecTeacher[i];
			pTeacherList->AddItem(teacher);
		}
	}
}

// ��ע��ʦ
void CSearchWnd::OnCare(int nUid)
{
	CTeacherLogic teacherLogic;
	CResMsg msg;
	int nRes = teacherLogic.AddAttention(nUid, m_user.GetToken(), msg);
	if (nRes != 1)
	{
		MessageBox(NULL, _T("ʧ��"), _T(""),0);
		return;
	}
	MessageBox(NULL, _T("�ɹ�"), _T(""),0);
	return;
}

void CSearchWnd::SetExid(int nExid)
{
	m_nExid = nExid;
}
int CSearchWnd::GetExid()
{
	return m_nExid;
}

void CSearchWnd::SetUser(const CUser& user)
{
	m_user = user;
}
CUser CSearchWnd::GetUser()
{
	return m_user;
}