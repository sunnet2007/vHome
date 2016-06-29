#pragma once
#include <string>
using namespace std;

// ��ʦ��Ϣʵ����
class CTeacher
{
public:
	CTeacher(void);
	~CTeacher(void);

public:
	void SetName(const string& strName);
	void SetDesc(const string& strDesc);
	void SetExName(const string& strExName);
	void SetImgThumb(const string& strImgThumb);
	void SetNum(int nNum);
	void SetUid(int nUid);
	void SetStar(int nStar);
	void SetIfAttention(int nIfAttention);
	void SetSchool(string strSchool);
	void SetArticle(string strArticle);
	void SetArticleId(int nArticleId);

	string GetName() const;
	string GetDesc() const;
	string GetExName() const;
	string GetImgThumb() const;
	int GetNum() const;
	int GetUid() const;
	int GetStar() const;
	int GetIfAttention() const;
	string GetSchool() const;
	string GetArticle() const;
	int GetArticleId() const;


protected:
	string	m_strName;		// ��ʦ����
	string	m_strDesc;		// ��ʦ���
	string	m_strExName;	// ����������
	string	m_strImgThumb;	// ͷ��
	int		m_nNum;			// ��˿����
	int		m_nUid;			// ��ʦUid
	int		m_nStar;		// ��ʦ�Ǽ�
	int		m_nIfAttention;	// �Ƿ��ע	1�ѹ�ע 0δ��ע
	string	m_strSchool;	// ��ҵԺУ
	string	m_strArticle;	// ��������
	int		m_nArticleId;	// ��������ID
};

