#pragma once
#include "duilib.h"

// ע���û�����
class CRegisterWnd : public WindowImplBase
{
public:
	CRegisterWnd(bool bRegister);
	~CRegisterWnd(void);

	virtual LPCTSTR GetWindowClassName() const;  // ��ȡ������
	virtual CDuiString GetSkinFile();	// ��ȡ�����ļ�
	virtual CDuiString GetSkinFolder(); // ��ȡ�����ļ�·��

	virtual void Notify(TNotifyUI& msg); // ��Ϣ����
	void OnPrepare(TNotifyUI& msg);


protected:
	void OnSend(); // ������֤��
	void OnRegister(); // ע��
	bool m_bRegister;	// trueע�� false�һ�����
};

