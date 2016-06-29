#pragma  once

#pragma warning(disable : 4786)

#include "sqlite3.h"
#include "DatabaseHelper.h"
#include "../Utility/Lock.h"


class CSqliteHelper:public CDatabaseHelper
{
public:
	CSqliteHelper();
	~CSqliteHelper();

public:
	int Open( DbInfo &db_info );
	int Close();                
	int Query(const string &sql,CSqlResult &result);
	//sqlite ���޸Ĳ����ӵ��ļ����������Ҫ����ִ��
	int Execute(const string & sql,CSqlResult &result);

private:

	sqlite3 * m_db;
	int		m_connected;
	static  CRWLock sql_lock;

};

