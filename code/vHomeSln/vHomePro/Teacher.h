#pragma once
#include <string>
#include <vector>
using namespace std;

// �ó���Ŀ
class CSpecialty
{
public:
	CSpecialty(void);
	~CSpecialty(void);

public:
	void SetId(int nId);
	void SetName(const string& strName);

	int GetId() const;
	string GetName() const;

private:
	int m_nId;
	string m_strName;
};


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
	void SetSchool(const string& strSchool);
	void SetArticle(const string& strArticle);
	void SetArticleId(int nArticleId);
	void SetNicName(const string& strNicName);
	void SetSex(const string& strSex);
	void SetEmail(const string& strEmail);
	void SetQq(const string& strQq);
	void SetExid(int nId);
	void SetGoodat(const vector<CSpecialty> vecGoodat);
	void SetGoodatIds(const string& strGoodatIds);

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
	string GetNicName() const;
	string GetSex() const;
	string GetEmail() const;
	string GetQq() const;
	int GetExid() const;
	vector<CSpecialty> GetGoodat() const;
	string GetGoodIds() const;

protected:
	string	m_strName;		// ��ʦ����
	string	m_strDesc;		// ��ʦ���
	string	m_strExName;	// ����������
	int		m_nExid;		// ������id
	string	m_strImgThumb;	// ͷ��
	int		m_nNum;			// ��˿����
	int		m_nUid;			// ��ʦUid
	int		m_nStar;		// ��ʦ�Ǽ�
	int		m_nIfAttention;	// �Ƿ��ע	1�ѹ�ע 0δ��ע
	string	m_strSchool;	// ��ҵԺУ
	string	m_strArticle;	// ��������
	int		m_nArticleId;	// ��������ID
	string	m_strNicName;	// ��ʦ�ǳ�
	string	m_strSex;			// �Ա�
	string	m_strEmail;		// ����
	string	m_strQq;		// QQ
	vector<CSpecialty> m_vecGoodat;	// �ó�
	string m_strGoodatIds;		// �ó���ID�ַ���
};

