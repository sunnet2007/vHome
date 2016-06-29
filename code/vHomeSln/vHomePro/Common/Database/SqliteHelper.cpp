#include "SqliteHelper.h"

CRWLock CSqliteHelper::sql_lock;

CSqliteHelper::CSqliteHelper()
{
	m_db = NULL;
	m_connected = 0;
}

CSqliteHelper::~CSqliteHelper()
{
	Close();
}

int CSqliteHelper::Open( DbInfo &db_info )
{
	int ret = 0;

	Close();

	sqlite3_config(SQLITE_CONFIG_MULTITHREAD);//多线程模式

	if(sqlite3_open(db_info.db_name.c_str(), &m_db) != SQLITE_OK)
	{
		ret = -1;
		m_connected = 0;
	}
	else
	{
		ret = 0;
		m_connected = 1;
	}

	return ret;
}

int CSqliteHelper::Close()
{
	if(m_connected)
	{
		sqlite3_close(m_db);
		m_db = NULL;
		m_connected = 0;
	}

	return 0;
}

int CSqliteHelper::Query(const string &sql,CSqlResult &result)
{
	sqlite3_stmt * stmt = NULL;
	int i;
	vector<string> rowVec;
	int rc;

	if(!m_connected)
	{
		result.errNo = -1;
		result.errMsg = "Not connected";
		return -1;
	}
	
	sql_lock.readLock();//读锁
	rc = sqlite3_prepare(m_db, sql.c_str(), sql.size(), &stmt, NULL);
	if(rc != SQLITE_OK)
	{
		result.errNo = rc;
		result.errMsg = sqlite3_errmsg(m_db);
		sql_lock.readUnLock();
		return -1;
	}

	rc = sqlite3_step(stmt);
	if(rc == SQLITE_ERROR)
	{
		result.errNo = rc;
		result.errMsg = sqlite3_errmsg(m_db);
		sql_lock.readUnLock();
		return -1;
	}

	if(rc == SQLITE_ROW)
	{
		result.fieldNum = sqlite3_column_count(stmt);
		while(rc == SQLITE_ROW)
		{
			rowVec.clear();
			for(i = 0; i < result.fieldNum; i++)
			{
				const char * val = (const char *)sqlite3_column_text(stmt, i);
				if(val != NULL)
				{
					rowVec.push_back(val);
				}
				else
				{
					rowVec.push_back("");
				}
			}
			result.recordList.push_back(rowVec);
			result.recordNum++;

			rc = sqlite3_step(stmt);
		}
	}
	sqlite3_finalize(stmt);

	sql_lock.readUnLock();

	return 0;
}


int CSqliteHelper::Execute(const string &sql,CSqlResult &result)
{
	result.Clear();
	char * errMsg = NULL;
	int ret = -1;

	if(!m_connected)
	{
		result.errNo = -1;
		result.errMsg = "Not connected";
		return ret;
	}

	sql_lock.writeLock();//写锁
	int maxCnt = 3;
	while ( maxCnt-- > 0 )
	{
		int rc = 0;
		rc = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, &errMsg);
		if ( rc == SQLITE_BUSY )
		{
			Sleep(10);
			continue;
		}
		else if ( rc != SQLITE_OK )
		{
			result.errNo = rc;
			result.errMsg = errMsg;
			break;
		}
		ret  = 0;

	} 


	sql_lock.writeUnLock();
	return ret;
}

