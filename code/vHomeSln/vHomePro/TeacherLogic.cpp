#include "TeacherLogic.h"
#include "TeacherDao.h"

CTeacherLogic::CTeacherLogic(void)
{
}


CTeacherLogic::~CTeacherLogic(void)
{
}

// 关注讲师
int CTeacherLogic::AddAttention(int nTid, const string& strToken, string& strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.AddAttention(nTid,strToken,msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 取消关注
int CTeacherLogic::DelAttention(int nTid, const string& strToken, string& strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.DelAttention(nTid, strToken, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 获取关注的讲师
int CTeacherLogic::GetCareTeacher(const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.GetCareTeacher(strToken, vecTeacher, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 获取名师列表(排名前三)
int CTeacherLogic::GetTopTeacher(const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.GetTopTeacher(strToken, vecTeacher, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 获取名师列表(前三除外)
int CTeacherLogic::GetStarTeacher(int nPage,  int nPageNum, const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.GetStarTeacher(nPage, nPageNum, strToken, vecTeacher, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 获取推荐讲师
int CTeacherLogic::GetTeachers(int nPage, int nPageNum,const string& strToken, vector<CTeacher>& vecTeacher, string& strMsg )
{
	GetTopTeacher(strToken, vecTeacher, strMsg);
	GetStarTeacher(nPage, nPageNum, strToken, vecTeacher, strMsg);
	return 0;
}

// 讲师获取自己发布的文章
int CTeacherLogic::GetAllArtOfTeacher(int nPage, int nPageNum, const string& strToken, vector<CArticle>& vecArt, string& strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.GetAllArtOfTeacher(nPage, nPageNum,strToken,vecArt, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 获取发布文章前的相关信息
int CTeacherLogic::GetArtCat(const string& strToken, string& strExchange, vector<CCategory>& vecCtgy, string& strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.GetArtCat(strToken, strExchange, vecCtgy, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 发布文章
int CTeacherLogic::ReleaseArt(const string& strToken, CArticle& art, string strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	string strImgUrl;
	if (art.GetImgThumb() != "")
	{
		teacherDao.UploadArtImg(strToken, art.GetId(), art.GetImgThumb(), strImgUrl, msg);
	}
	art.SetImgThumb(strImgUrl);
	int nRes = teacherDao.ReleaseArt(strToken, art, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 显示讲师信息
int CTeacherLogic::GetTeacherInfo(CTeacher& teacher, vector<CSpecialty>& vecSpecialty,const string strToken, string strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.GetTeacherInfo(teacher, vecSpecialty, strToken, msg);
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

// 修改讲师信息
int CTeacherLogic::ModifyTeacherInfo(const CTeacher& teacher,const string strToken, string strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	if (teacher.GetImgThumb() != "")
	{
		int nRes = teacherDao.UploadTeacherImg(strToken, teacher.GetImgThumb(), msg); // 修改头像
		if (nRes != 0)
		{
			strMsg = msg.GetInfo();
			return -1;
		}
	}

	int nRes = teacherDao.ModifyTeacherInfo(teacher, strToken, msg);	// 修改个人信息
	if (nRes != 0)
	{
		strMsg = msg.GetInfo();
		return -1;
	}
	return 0;
}

int CTeacherLogic::GetTeacherArts(int nPage, int nPageNum , int tId,const string& strToken, vector<CArticle>& vecArt,vector<CCategory>& vecCtgy, string& strMsg ,int nCtgyId/* = 0*/)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	teacherDao.GetTeacherArts(nPage, nPageNum, tId, strToken, vecArt, vecCtgy, msg, nCtgyId);
	return 0;

}

int CTeacherLogic::UploadTeacherImage(const string& strToken, const string& strImg, string& strMsg)
{
	CTeacherDao teacherDao;
	CResMsg msg;
	int nRes = teacherDao.UploadTeacherImg(strToken, strImg, msg);
	return 0;
}