#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
using namespace std;

string EncodePass(string pass)
{
	unsigned char key[] = {0x6f, 0x17, 0x24, 0xe2, 0x24, 0x57, 0x85, 0x9b,
			0xae, 0xe1, 0xad, 0x0c, 0xdb, 0x1d, 0xf7, 0x7a,
			0xec, 0x14, 0xd1, 0xe8, 0xeb, 0x7a, 0xb5, 0x39,
			0x3f, 0xec, 0x5c, 0x96, 0x7d, 0x76, 0x2e, 0x0f};

	int keyLen = sizeof(key);
	int passLen = pass.size();
	if(passLen > 30)
	{
		return "";
	}

	srand(time(NULL));
	int k = (passLen + time(NULL)) % keyLen;
	char ret[100] = { 0 };
	sprintf(ret, "%02x%02x", passLen^(k*8), k);

	int i;
	for(i = 0; i < passLen; i++)
	{
		char hex[10] = { 0 };
		sprintf(hex, "%02x", ((unsigned char)key[k])^((unsigned char)pass[i]));
		strcat(ret, hex);
		k = (k + 1) % keyLen;
	}
	
	for(i = passLen; i < 30; i++)
	{
		char hex[10] = { 0 };
		sprintf(hex, "%02x", rand()%256);
		strcat(ret, hex);
	}

	return ret;
}

string  DecodePass(string pass)
{
	unsigned char key[] = {0x6f, 0x17, 0x24, 0xe2, 0x24, 0x57, 0x85, 0x9b,
			0xae, 0xe1, 0xad, 0x0c, 0xdb, 0x1d, 0xf7, 0x7a,
			0xec, 0x14, 0xd1, 0xe8, 0xeb, 0x7a, 0xb5, 0x39,
			0x3f, 0xec, 0x5c, 0x96, 0x7d, 0x76, 0x2e, 0x0f};

	int keyLen = sizeof(key);
	int passLen = pass.size();
	if(passLen != 64)
		return "";

	int k = 0;
	if(sscanf(pass.substr(2, 2).c_str(), "%x", &k) != 1)
		return "";

	int plen = 0;
	if(sscanf(pass.substr(0, 2).c_str(), "%x", &plen) != 1)
		return "";

	plen = plen^(k*8);
	if(plen < 0 || plen > 30)
		return "";

	string ret;
	int i;
	for(i = 2; i < plen+2; i++)
	{
		int p = 0;
		if(sscanf(pass.substr(i*2, 2).c_str(), "%x", &p) != 1)
			return "";

		ret.append(1, (char)(key[k]^p));
		k = (k + 1) % keyLen;
	}

	return ret;
}
