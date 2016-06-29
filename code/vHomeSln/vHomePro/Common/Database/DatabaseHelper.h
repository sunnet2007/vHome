#pragma once
#include "SqlResult.h"

class DbInfo
{
public:
	DbInfo():host("127.0.0.1"),port(3306),user("root"),charset("utf8")
	{

	}
	string	host;
	int		port;
	string	user;
	string	password;
	string	db_name;
	string	charset;
};

class CDatabaseHelper
{
public:
	CDatabaseHelper(void){};
	virtual ~CDatabaseHelper(void){};

	virtual int Open( DbInfo &db_info )=0;
	virtual int Close()=0;          
	virtual int Query(const string &sql,CSqlResult &result)=0;
	virtual int Execute(const string & sql,CSqlResult &result)=0;//返回结果中不存储结果集

	//事务
	int Begin()
	{
		CSqlResult sqlResult;
		Execute("START TRANSACTION",sqlResult );
		return sqlResult.errNo;
	}
	int Commit()
	{
		CSqlResult sqlResult;
		Execute("COMMIT ",sqlResult );
		return sqlResult.errNo;
	}
	int Rollback()
	{
		CSqlResult sqlResult;
		Execute("ROLLBACK ",sqlResult );
		return sqlResult.errNo;
	}

};

