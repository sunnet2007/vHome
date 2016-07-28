#pragma once
#include <string>
#include <time.h>
using namespace std;

// ���Ϣʵ����
class CActivity
{
public:
	CActivity(void);
	~CActivity(void);

public:
	void SetId(int nId);
	void SetTitle(const string& strTitle);
	void SetContent(const string& strContent);
	void SetNum(int nNum);
	void SetStart(time_t tStart);
	void SetEnd(time_t tEnd);
	void SetInNum(int nInNum);
	void SetMyStatus(int nStatus);
	void SetImg(const string& strImg);

	int GetId() const;
	string GetTitle() const;
	string GetContent() const;
	int GetNum() const;
	time_t GetStart() const;
	time_t GetEnd() const;
	int GetInNum() const;
	int GetMyStatus() const;
	string GetImg() const;

private:
	int			m_nId;			// �ID
	string		m_strTitle;		// �����
	string		m_strContent;	// ��ȡ����
	int			m_nNum;			// �μ�����
	time_t		m_tStart;		// ��ʼʱ��
	time_t		m_tEnd;			// ����ʱ��
	int			m_nInNum;		// ���������
	int			m_nMyStatus;	// ��¼״̬
	string		m_strImg;		// �ͼƬ
};

