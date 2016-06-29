#include "ExTeacherListUI.h"
#include "StringUtil.h"

CExTeacherListUI::CExTeacherListUI(CPaintManagerUI& paint_manager):m_paintManager(paint_manager)
{
}


CExTeacherListUI::~CExTeacherListUI(void)
{
}

void CExTeacherListUI::AddItem(const CTeacher& teacher)
{
	SIZE size;
	size.cx = 180;
	size.cy = 80;
	SetItemSize(size);
	CDialogBuilder builder;
	CContainerUI* pTeacherUI = static_cast<CContainerUI*>(builder.Create(_T("ExTeacherList.xml"), (UINT)0));
	if( pTeacherUI != NULL ) {
			if( pTeacherUI == NULL ) pTeacherUI = static_cast<CContainerUI*>(builder.Create());
			if( pTeacherUI != NULL ) {
				this->Add(pTeacherUI);
				CLabelUI* pLabelImg = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pTeacherUI, _T("labelImg")));
				char cBkImage[32];
				memset(cBkImage, 0, 32);
				sprintf_s(cBkImage, 32, "teacherimage_%d.png", teacher.GetUid());

				TCHAR tcBkImage[32];
				memset(tcBkImage, 0, 32);
				CharToTchar(cBkImage, tcBkImage);
				pLabelImg->SetBkImage(tcBkImage);

				CLabelUI* pLabelName = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pTeacherUI, _T("labelName")));
				TCHAR tcName[32];
				memset(tcName, 0, 32);
				CharToTchar(teacher.GetName().c_str(), tcName);
				pLabelName->SetText(tcName);

				// 将ID与列表项绑定
				char cId[32];
				memset(cId, 0,32);
				itoa(teacher.GetUid(), cId, 10);
				TCHAR tcId[32];
				memset(tcId, 0, 32);
				CharToTchar(cId, tcId);
				pTeacherUI->SetUserData(tcId);


				pTeacherUI = NULL;
			}
			else {
				this->RemoveAll();
				return;
			}
	}
}

void CExTeacherListUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_BUTTONUP)
	{
		if (event.pSender->GetName() == _T("btnCare"))
		{
			MessageBox(NULL, _T("关注"), _T(""), 0);
		}
		else if (event.pSender->GetName() == _T("btnTalk"))
		{
			MessageBox(NULL, _T("交谈"), _T(""), 0);
		}
	}
}