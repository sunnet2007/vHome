#include "StdAfx.h"
#include "NetAddr.h"
#include "../Utility/StringFuns.h"
#include "../Utility/Log.h"

CNetAddr::CNetAddr()
{
}
CNetAddr::CNetAddr(int ip,int port)
{
	_ip = ip;
	_port = port;
}
CNetAddr::CNetAddr(const char*ip,int port)
{
	setIp( ip );
	_port = port;
}


CNetAddr::~CNetAddr(void)
{
}
void  CNetAddr::from64(int64 addr)
{
	*this = *(CNetAddr*)&addr;
}
int64 CNetAddr::to64()
{
	int64 i = 0;
	unsigned char*p = (unsigned char*)&i;
	memcpy(p,&_ip,4);
	memcpy(p+4,&_port,4);

	return i;
}
int CNetAddr::ip()
{
	return _ip;
}
int CNetAddr::port()
{
	return _port;
}
int CNetAddr::setIp( const char*ip)
{
	if ( NULL == ip )
	{
		return -1;
	}
	int ret = 0;
#if 0
	vector<string> fields;
	StringSplit(ip,fields,'.');
	if (fields.size() != 4)
	{
		LOG.Log("%s is not a valiable ip", ip);
		return -1;
	}

	unsigned char *d = (unsigned char*)&_ip;
	for (int i = 0; i < 4; i++)
	{
		if ("*" == fields[i])
		{
			d[i] = -1;
		}
		else
		{
			d[i] = atoi(fields[i].c_str());
			if (0 == d[i] && "0" != fields[i])
			{
				ret = -1;
				break;
			}
		}
	}
#else
	_ip = inet_addr( ip ); 
	if(_ip == INADDR_NONE)
	{
		struct hostent *hp;
		if ((hp = gethostbyname(ip)) == NULL)
		{
			ret = -1;
		}
		else
		{
			_ip = ((struct in_addr *) hp->h_addr)->s_addr;
		}
	}
#endif
	return ret;
}
std::string CNetAddr::getIp()
{
	char tmp[32]={0};
#if 0
	unsigned char *bytes = (unsigned char *) &_ip;
	sprintf(tmp, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
#else
	in_addr inAddr;
	inAddr.s_addr = _ip;
	sprintf(tmp, "%s",inet_ntoa( inAddr ));
#endif
	return tmp;
}
std::string CNetAddr::toAddrString()
{
	char tmp[32]={0};
#if 0
	unsigned char *bytes = (unsigned char *) &_ip;
	sprintf(tmp, "%d.%d.%d.%d:%d", bytes[0], bytes[1], bytes[2], bytes[3],_port);
#else
	in_addr inAddr;
	inAddr.s_addr = _ip;
	sprintf(tmp, "%s:%d",inet_ntoa( inAddr ),_port);
#endif
	return tmp;
}

int CNetAddr::distance( CNetAddr &addr )
{
	unsigned int ip1 = _ip;
	unsigned int ip2 = addr.ip();

	unsigned int mask = 0xff;
	unsigned int n1 = 0;
	unsigned int n2 = 0;
	for (int i = 0; i < 4; i++)
	{
		n1 <<= 8;
		n2 <<= 8;
		n1 |= ip1 & mask;
		n2 |= ip2 & mask;
		ip1 >>= 8;
		ip2 >>= 8;
	}
	int result = 0;
	if (n1 > n2)
	{
		result = n1 - n2;
	}
	else
	{
		result = n2 - n1;
	}
	return result;
}
void CNetAddr::setSockAddr( sockaddr_in &addr )
{
	_ip = addr.sin_addr.s_addr;
	_port = ntohs(addr.sin_port);
}
sockaddr_in CNetAddr::makeSockAddr()
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = _ip;
	addr.sin_port = htons( _port );
	return addr;
}
