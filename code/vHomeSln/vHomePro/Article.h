#pragma once
#include <string>
using namespace std;

// 分类信息
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
	int m_nId;			// 分类id
	string m_strName;	// 分类名
	string m_strImg;	// 分类图片
};


// 文章信息实体类
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
	int		m_nId;			// 文章id
	string	m_strAbout;		// 文章简介
	int		m_nHits;		// 点赞数
	string	m_strTitle;		// 文章标题
	string	m_strContent;	// 文章内容
	string	m_strImgThumb;	// 文章图片
	time_t	m_tCTime;		// 创建时间
	string	m_strCtgy;		// 所属分类
	int		m_nCtgyId;		// 所属分类id
	int		m_nStatus;		// 状态 0未通过 1通过
	string	m_strReason;	// 未通过原因
	int		m_nExid;		// 交易所id
	string	m_strExName;	// 交易所名称

};

