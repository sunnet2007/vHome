#ifndef _UTILIY_H_20130607
#define _UTILIY_H_20130607

#include <iostream>
#include <string>
using namespace std;

class CUtility
{
public:
	static string GetSign(string s);
	static int    GetHeader(string head, string name, string & val);
	static string GetMime(string ext);
};

#endif