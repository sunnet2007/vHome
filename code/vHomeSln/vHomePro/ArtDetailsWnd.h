#pragma once
#include "duilib.h"
#include "User.h"

class CArtDetailsWnd : public WindowImplBase
{
public:
	CArtDetailsWnd(void);
	CArtDetailsWnd(int nArtId, CUser user);
	~CArtDetailsWnd(void);
	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��
	virtual void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);

private:
	int m_nArtId;
	CUser m_user;
};

