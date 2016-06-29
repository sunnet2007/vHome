#pragma once
#include "MysqlHelper.h"
#include "SqliteHelper.h"
#include <list>
#include "../Utility/Lock.h"

class CDatabaseConnectPool
{
public:
	static CDatabaseConnectPool db_pool;

	enum 
	{
		DB_MYSQL = 0,
		DB_SQLITE
	};

	bool InitPool( const int poolSize,const DbInfo &dbInfo,int dbType=DB_SQLITE);
	bool DestroyPool(void);
	CDatabaseHelper *Get();
	void Release( CDatabaseHelper *pDbHelper);

private:
	CDatabaseConnectPool(void);
	~CDatabaseConnectPool(void);

	std::list<CDatabaseHelper*>lstUsing;
	std::list<CDatabaseHelper*>lstFree;
	int		m_poolSize;
	DbInfo	m_dbInfo;
	CLock	m_lock;
	int		m_type;

};

class CAutoDBHelper
{
public:
	CAutoDBHelper()
	{
		pDb = CDatabaseConnectPool::db_pool.Get();
	}
	~CAutoDBHelper()
	{
		CDatabaseConnectPool::db_pool.Release(pDb);
	}
	CSqliteHelper *sqlite()
	{
		return (CSqliteHelper*)pDb;
	}
	CMysqlHelper *mysql()
	{
		return (CMysqlHelper*)pDb;
	}
private:
	CDatabaseHelper *pDb;
};