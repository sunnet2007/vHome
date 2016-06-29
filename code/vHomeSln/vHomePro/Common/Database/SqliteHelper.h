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
	//sqlite 对修改操作加的文件锁，因此需要排它执行
	int Execute(const string & sql,CSqlResult &result);

private:

	sqlite3 * m_db;
	int		m_connected;
	static  CRWLock sql_lock;

};

