#include "MysqlHelper.h"
#include <sstream>


CMysqlHelper::CMysqlHelper()
{
	m_connected = 0;
}

CMysqlHelper::~CMysqlHelper()
{
	Close();
}

int CMysqlHelper::Open(DbInfo &dbInfo)
{
	db_info = dbInfo;

	return connect();
}

int CMysqlHelper::connect()
{
	if ( m_connected )
	{
		Close();
	}

	mysql_init(&m_mysql);

	if(mysql_real_connect(&m_mysql, db_info.host.c_str(), db_info.user.c_str(), db_info.password.c_str(), db_info.db_name.c_str(), db_info.port, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
	{
		mysql_close(&m_mysql);
		return -1;
	}

	mysql_set_character_set(&m_mysql, db_info.charset.c_str());

	m_connected = 1;

	return 0;
}

int CMysqlHelper::Close()
{
	if(m_connected)
	{
		mysql_close(&m_mysql);
		m_connected = 0;
	}
	return 0;
}

int CMysqlHelper::Query(const string & sql,CSqlResult &result)
{
	MYSQL_RES * mysqlRes = NULL;
	MYSQL_ROW row = NULL;
	int ret;
	int i, j;
	vector<string> rowVec;

	if(!m_connected)
	{
		result.errNo = -1;
		result.errMsg = "Not connected";
		return -1;
	}
	
	ret = mysql_real_query(&m_mysql, sql.c_str(), sql.size());
	result.errNo = mysql_errno(&m_mysql);
	if(ret != 0)
	{
		if(result.errNo == 2006 || result.errNo == 2013 || result.errNo == 2002 || result.errNo == 2003 || result.errNo == 1053)
		{
			if(connect() != 0)
			{
				result.errNo = -1;
				result.errMsg = "Reconnected failed";
				return -1;
			}
		}
		else
		{
			result.errNo = mysql_errno(&m_mysql);
			result.errMsg = mysql_error(&m_mysql);
			return -1;
		}

		if(mysql_real_query(&m_mysql, sql.c_str(), sql.size()) != 0)
		{
			result.errNo = mysql_errno(&m_mysql);
			result.errMsg = mysql_error(&m_mysql);
			return -1;
		}
	}

	//如果用户执行了多条SQL，则只返回第一条SQL的结果集
	mysqlRes = mysql_store_result(&m_mysql);
	if(mysqlRes != NULL)
	{
		result.recordNum = (int)mysql_num_rows(mysqlRes);
		result.fieldNum = mysql_num_fields(mysqlRes);

		for(i = 0; i < result.recordNum; i++)
		{
			rowVec.clear();
			row = mysql_fetch_row(mysqlRes);
			for(j = 0; j < result.fieldNum; j++)
			{
				if(row[j] != NULL)
				{
					rowVec.push_back(row[j]);
				}
				else
				{
					rowVec.push_back("");
				}
			}
			result.recordList.push_back(rowVec);
		}
		mysql_free_result(mysqlRes);
	}

	//如有其它语句，则其它语句的结果集丢弃
	while(mysql_next_result(&m_mysql) == 0)
	{
		mysqlRes = mysql_store_result(&m_mysql);
		if(mysqlRes != NULL)
		{
			mysql_free_result(mysqlRes);
		}
	}

	return 0;
}

int CMysqlHelper::Execute(const string &sql,CSqlResult& result)
{
	MYSQL_RES * mysqlRes = NULL;
	int ret;

	if(!m_connected)
	{
		result.errNo = -1;
		result.errMsg = "Not connected";
		return -1;
	}

	ret = mysql_real_query(&m_mysql, sql.c_str(), sql.size());
	result.errNo = mysql_errno(&m_mysql);
	if(ret != 0)
	{
		if(result.errNo == 2006 || result.errNo == 2013 || result.errNo == 2002 || result.errNo == 2003 || result.errNo == 1053)
		{
			if(connect() != 0)
			{
				result.errNo = -1;
				result.errMsg = "Reconnected failed";
				return -1;
			}
		}
		else
		{
			result.errNo = mysql_errno(&m_mysql);
			result.errMsg = mysql_error(&m_mysql);
			return -1;
		}

		if(mysql_real_query(&m_mysql, sql.c_str(), sql.size()) != 0)
		{
			result.errNo = mysql_errno(&m_mysql);
			result.errMsg = mysql_error(&m_mysql);
			return -1;
		}
	}
	result.recordNum = (int)mysql_affected_rows( &m_mysql );//影响的行数
	/* process each statement result */
	do 
	{
		mysqlRes = mysql_store_result(&m_mysql);
		if(mysqlRes != NULL)
		{
			mysql_free_result(mysqlRes);
		}
		else 
		{

		}
	} while (mysql_next_result(&m_mysql) == 0);

	return 0;
}
