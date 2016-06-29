#include "DatabaseConnectPool.h"
#include <assert.h>
using namespace std; 

CDatabaseConnectPool CDatabaseConnectPool::db_pool;

CDatabaseConnectPool::CDatabaseConnectPool(void)
{
	m_poolSize = 20;
	m_dbInfo.db_name = "data.db";
	m_type = DB_SQLITE;
}


CDatabaseConnectPool::~CDatabaseConnectPool(void)
{
}

bool CDatabaseConnectPool::InitPool( const int poolSize,const DbInfo &dbInfo,int dbType )
{
	m_poolSize = poolSize;
	m_dbInfo = dbInfo;
	m_type = dbType;
	return true;
}
bool CDatabaseConnectPool::DestroyPool(void)
{
	CAutoLock lock( m_lock );

	list<CDatabaseHelper*>::iterator it;
	for ( it = lstUsing.begin();it != lstUsing.end();it++ )
	{
		delete *it;
	}
	for ( it = lstFree.begin();it != lstFree.end();it++ )
	{
		delete *it;
	}
	return true;
}
CDatabaseHelper *CDatabaseConnectPool::Get()
{
	CAutoLock lock( m_lock );

	CDatabaseHelper* pDb = NULL;
	if ( lstFree.size() > 0 )
	{
		pDb = lstFree.front();
		lstFree.pop_front();
	}
	else
	{
		if ( lstUsing.size() >= m_poolSize )
		{
			//³¬¹ý³Ø´óÐ¡
			return pDb;
		}
		switch ( m_type )
		{
		case DB_MYSQL:
			pDb = new CMysqlHelper;
			break;
		case DB_SQLITE:
			pDb = new CSqliteHelper;
			break;
		}
		assert( pDb );

		if ( pDb->Open( m_dbInfo ) != 0  )
		{
			delete pDb;
			pDb = NULL;
			return pDb;
		}
	}
	lstUsing.push_back( pDb );
	return pDb;
}
void CDatabaseConnectPool::Release( CDatabaseHelper *pDbHelper)
{
	CAutoLock lock( m_lock );

	list<CDatabaseHelper*>::iterator it;
	for ( it = lstUsing.begin();it != lstUsing.end();it++ )
	{
		if ( *it == pDbHelper )
		{
			lstUsing.erase( it );
			lstFree.push_back( pDbHelper );
			break;
		}
	}
}