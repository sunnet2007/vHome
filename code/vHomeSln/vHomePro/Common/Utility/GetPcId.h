#if (defined(_WIN32) || defined(_WIN64))

#ifndef _GET_PCID_H
#define _GET_PCID_H

#include <iostream>
#include <string>
using namespace std;

int GetHardwareSNumber(char * pszNumber);

string GetPcID(const string & hardwareSN);

#endif

#endif
