#pragma once

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#pragma warning(disable : 4786)
#pragma comment(lib,"libmysql.lib")
#endif

#include "..\mysql\include\mysql.h"
#include "DatabaseHelper.h"



class CMysqlHelper:public CDatabaseHelper
{
public:
	CMysqlHelper();
	~CMysqlHelper();

public:
	int Open( DbInfo &db_info );
	int Close();                
	int Query(const string &sql,CSqlResult &result);
	int Execute(const string & sql,CSqlResult &result);

private:
	int connect();
private:
	MYSQL	m_mysql;
	DbInfo  db_info;
	int		m_connected;
};

