#pragma once
#include <string>
#include <vector>
using namespace std;

// 擅长项目
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


// 讲师信息实体类
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
	string	m_strName;		// 讲师名称
	string	m_strDesc;		// 讲师简介
	string	m_strExName;	// 交易所名称
	int		m_nExid;		// 交易所id
	string	m_strImgThumb;	// 头像
	int		m_nNum;			// 粉丝人数
	int		m_nUid;			// 讲师Uid
	int		m_nStar;		// 讲师星级
	int		m_nIfAttention;	// 是否关注	1已关注 0未关注
	string	m_strSchool;	// 毕业院校
	string	m_strArticle;	// 最新文章
	int		m_nArticleId;	// 最新文章ID
	string	m_strNicName;	// 讲师昵称
	string	m_strSex;			// 性别
	string	m_strEmail;		// 邮箱
	string	m_strQq;		// QQ
	vector<CSpecialty> m_vecGoodat;	// 擅长
	string m_strGoodatIds;		// 擅长项ID字符串
};

