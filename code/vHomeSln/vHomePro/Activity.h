#pragma once
#include <string>
#include <time.h>
using namespace std;

// 活动信息实体类
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
	int			m_nId;			// 活动ID
	string		m_strTitle;		// 活动标题
	string		m_strContent;	// 获取内容
	int			m_nNum;			// 参加人数
	time_t		m_tStart;		// 开始时间
	time_t		m_tEnd;			// 结束时间
	int			m_nInNum;		// 活动限制人数
	int			m_nMyStatus;	// 登录状态
	string		m_strImg;		// 活动图片
};

