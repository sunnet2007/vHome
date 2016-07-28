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
			if (pLabelImg)
			{
				//CDuiString strImg;
				//strImg.Format(_T("teacherimage_%d.png"), teacher.GetUid());
				//pLabelImg->SetBkImage(strImg);

				string strImg = teacher.GetImgThumb();
				wstring wstrImg = s2ws(strImg); 
				pLabelImg->SetBkImage(wstrImg.c_str());
			}

			CLabelUI* pLabelName = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(pTeacherUI, _T("labelName")));
			if (pLabelName)
			{
				wstring wstrName = s2ws(teacher.GetName());
				pLabelName->SetText(wstrName.c_str());
			}
			
			CButtonUI* pButton = static_cast<CButtonUI*>(m_paintManager.FindSubControlByName(pTeacherUI, _T("btnCare")));
			if (pButton)
			{
				int nIfAddtention = teacher.GetIfAttention();
				if (nIfAddtention ==1) // 已经关注
					pButton->SetText(_T("取消关注"));
				else 
					pButton->SetText(_T("关注"));

			}


			CDuiString strId;
			strId.Format(_T("%d"), teacher.GetUid());
			pTeacherUI->SetUserData(strId);

			int nStar = teacher.GetStar();
			CContainerUI* pContainer = static_cast<CContainerUI*>(m_paintManager.FindSubControlByName(pTeacherUI, _T("container")));
			for (int i = 0; i < nStar; i++)
			{
				CLabelUI* pLabelStar = new CLabelUI;
				pLabelStar->SetAttribute(_T("float"), _T("true"));
				CDuiString strPos;
				strPos.Format(_T("%d,%d,%d,%d"), 70+13*i, 30, 70+13*(i+1),43);
				pLabelStar->SetAttribute(_T("pos"),strPos);
				pLabelStar->SetBkImage(_T("file='lightstart.png' source='0,3,13,16'"));
				if (pContainer)
				{
					pContainer->AddAt(pLabelStar, 2+i);
				}

			}

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