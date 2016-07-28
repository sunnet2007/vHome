#pragma once
#include <string>
using namespace std;

// ������Ϣ
class CCategory
{
public:
	CCategory(void);
	~CCategory(void);

public:
	void SetId(int nId);
	void SetName(const string& strName);
	void SetImg(const string& strImg);

	int GetId() const;
	string GetName() const;
	string GetImg() const;

private:
	int m_nId;			// ����id
	string m_strName;	// ������
	string m_strImg;	// ����ͼƬ
};


// ������Ϣʵ����
class CArticle
{
public:
	CArticle(void);
	~CArticle(void);

public:
	void SetId(int nId);
	void SetAbout(const string& strAbout);
	void SetHits(int nHits);
	void SetTitle(const string& strTitle);
	void SetContent(const string& strContent);
	void SetImgThumb(const string& strImgThumb);
	void SetCTime(time_t tTime);
	void SetCtgy(const string& strCtgy);
	void SetCtgyId(int nCtgtId);
	void SetStatus(int nStatus);
	void SetReason(const string& strReason);
	void SetExid(int nExid);
	void SetExName(const string& strExName);


	int GetId() const;
	string GetAbout() const;
	int GetHits() const;
	string GetTitle() const;
	string GetContent() const;
	string GetImgThumb() const;
	time_t GetCTime() const;
	string GetCtgy() const;
	int GetCtgyId() const;
	int GetStatus() const;
	string GetReason() const;
	int GetExid() const;
	string GetExName() const;

private:
	int		m_nId;			// ����id
	string	m_strAbout;		// ���¼��
	int		m_nHits;		// ������
	string	m_strTitle;		// ���±���
	string	m_strContent;	// ��������
	string	m_strImgThumb;	// ����ͼƬ
	time_t	m_tCTime;		// ����ʱ��
	string	m_strCtgy;		// ��������
	int		m_nCtgyId;		// ��������id
	int		m_nStatus;		// ״̬ 0δͨ�� 1ͨ��
	string	m_strReason;	// δͨ��ԭ��
	int		m_nExid;		// ������id
	string	m_strExName;	// ����������

};

