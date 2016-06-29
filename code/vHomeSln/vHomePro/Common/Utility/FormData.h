#ifndef _FORM_DATA_H
#define _FORM_DATA_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

#include "CrossPlatform.h"

#define FORM_DATA_NORMAL       0
#define FORM_DATA_MULTIPART    1

class CFormData
{
public:
	CFormData();

	int GetFormArgsFromEnv();
	int SetArg(const string & argname, string argvalue);
	int GetArg(const string & argname, string & argvalue);
	int SetArg(const string & argname, int64 argvalue);
	int GetArg(const string & argname, int64 & argvalue);
	int GetArg(const string & argname, int & argvalue);
	string GetBoundary();
	int Dump(string & str);
	int Encode(string & str, int type = 0);
	int Decode(const string & args, int type = -1); // type, 0 for simple, 1 from multipart, -1 for auto
	int Decode(const char * buf, int len, int type = -1);
	void Clear();
private:
	string EncodeVal(string arg);

	string m_boundary;
	map<string, string> m_args;
};

#endif

