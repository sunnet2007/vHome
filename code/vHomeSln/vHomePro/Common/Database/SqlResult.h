#ifndef _SQL_RESULT_H
#define _SQL_RESULT_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

//#include "CommFuns.h"

typedef vector< vector<string> > CRecordList;

class CSqlResult
{
public:
	int           errNo;      //错误状态码。无错误时为0
	string        errMsg;     //错误描述。无错误时为OK
	int           recordNum;  //记录的条数
	int           fieldNum;   //记录的字段数
	CRecordList   recordList; //记录列表

	CSqlResult()
	{
		errNo      = 0;
		errMsg     = "OK";
		recordNum  = 0;
		fieldNum   = 0;
	}
	
	int Encode(string & str)
	{
		string boundary = GenBoundary();

		char tmp[20] = { 0 };
		
		str = boundary;
		sprintf_s(tmp, "%d", errNo);
		str += tmp + boundary;
		str += errMsg + boundary;
		sprintf_s(tmp, "%d", recordNum);
		str += tmp + boundary;
		sprintf_s(tmp, "%d", fieldNum);
		str += tmp + boundary;
		
		int i, j;
		for(i = 0; i < recordNum; i++)
		{
			for(j = 0; j < fieldNum; j++)
			{
				str += recordList[i][j] + boundary;
			}
		}
		
		return 0;
	}
	
	int Decode(string & str)
	{
		Clear();

		if(str.size() <= 12)
			return -1;

		string boundary = str.substr(0, 12);
		
		string::size_type p1 = boundary.size();
		string::size_type p2 = str.find(boundary, p1);
		if(p2 == string::npos)
			return -1;
		
		errNo = atoi(str.substr(p1, p2-p1).c_str());
		
		p1 = p2 + boundary.size();
		p2 = str.find(boundary, p1);
		if(p2 == string::npos)
		{
			Clear();
			return -1;
		}
		/*
		if(p1 == string::npos || p1 == str.size()-boundary.size())
		{
			Clear();
			return -1;
		}
		
		p1 += boundary.size();
		*/
		errMsg = str.substr(p1, p2-p1);
		
		p1 = p2 + boundary.size();
		p2 = str.find(boundary, p1);
		if(p2 == string::npos)
		{
			Clear();
			return -1;
		}
		/*
		if(p1 == string::npos || p1 == str.size()-boundary.size())
		{
			Clear();
			return -1;
		}
		
		p1 += boundary.size();
		*/
		
		recordNum = atoi(str.substr(p1, p2-p1).c_str());

		/*
		p1 = str.find(boundary, p1);
		if(p1 == string::npos || p1 == str.size()-boundary.size())
		{
			Clear();
			return -1;
		}
		
		p1 += boundary.size();
		*/

		p1 = p2 + boundary.size();
		p2 = str.find(boundary, p1);
		if(p2 == string::npos)
		{
			Clear();
			return -1;
		}
		
		fieldNum = atoi(str.substr(p1, p2-p1).c_str());
		
		recordList.clear();
		
		int i, j;
		for(i = 0; i < recordNum; i++)
		{
			vector<string> row;
			for(j = 0; j < fieldNum; j++)
			{
				/*
				p1 = str.find(boundary, p1);
				if(p1 == string::npos || p1 == str.size()-boundary.size())
				{
					Clear();
					return -1;
				}
				
				p1 += boundary.size();
				*/

				p1 = p2 + boundary.size();
				p2 = str.find(boundary, p1);
				if(p2 == string::npos)
				{
					Clear();
					return -1;
				}
				
				row.push_back(str.substr(p1, p2-p1));
			}
			recordList.push_back(row);
		}
		
		return 0;
	}

	void Clear()
	{
		errNo      = 0;
		errMsg     = "OK";
		recordNum  = 0;
		fieldNum   = 0;
		recordList.clear();
	}
	
	void Print(FILE * fp = stdout)
	{
		fprintf(fp, "retn: %d, desc: %s\n", errNo, errMsg.c_str());
		fprintf(fp, "row: %d, field: %d\n", recordNum, fieldNum);

		int i, j;
		for(i = 0; i < recordNum; i++)
		{
			for(j = 0; j < fieldNum; j++)
			{
				fprintf(fp, "%-20s| ", recordList[i][j].c_str());
			}
			fprintf(fp, "\n");
		}
	}

private:
	
	string GenBoundary()
	{
		char boundary[13] = { "|SQL-RESULT|" };

		while(true)
		{
			int found = 0;
			int i, j;
			for(i = 0; i < recordNum; i++)
			{
				for(j = 0; j < fieldNum; j++)
				{
					if(recordList[i][j].find(boundary) != string::npos)
					{
						found = 1;
						break;
					}
				}

				if(found)
					break;
			}
			
			if(!found)
				break;

			for(i = 1; i <= 10; i++)
			{
				boundary[i] = 'A' + rand()%26;
			}
		}

		return boundary;
	}

};

#endif
