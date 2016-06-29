#pragma once
#include "duilib.h"

// ע���û�����
class CRegisterWnd : public WindowImplBase
{
public:
	CRegisterWnd(void);
	~CRegisterWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

	virtual void Notify(TNotifyUI& msg); // ��Ϣ����


protected:
	void OnSend(); // ������֤��
	void OnRegister(); // ע��
};

