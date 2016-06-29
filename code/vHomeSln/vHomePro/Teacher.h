#pragma once
#include <string>
using namespace std;

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
	string	m_strName;		// 讲师名称
	string	m_strDesc;		// 讲师简介
	string	m_strExName;	// 交易所名称
	string	m_strImgThumb;	// 头像
	int		m_nNum;			// 粉丝人数
	int		m_nUid;			// 讲师Uid
	int		m_nStar;		// 讲师星级
	int		m_nIfAttention;	// 是否关注	1已关注 0未关注
	string	m_strSchool;	// 毕业院校
	string	m_strArticle;	// 最新文章
	int		m_nArticleId;	// 最新文章ID
};

