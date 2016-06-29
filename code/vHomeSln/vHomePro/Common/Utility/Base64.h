#ifndef _BASE64_H_20121227
#define _BASE64_H_20121227

#include <string>
using namespace std;

class CBase64
{
public:
    static string Encode(const string & data);
    static string Decode(const string & data);
};


#endif
