#pragma once
#include <string>
#include "../Utility/CrossPlatform.h"

class CNetAddr
{
public:
	CNetAddr();
	CNetAddr(int ip,int port);
	CNetAddr(const char*ip,int port);
	~CNetAddr(void);

	void  from64(int64 addr);
	int64 to64();
	int	 ip();
	int  port();
	void setPort( int port ){ _port = port;}
	int  setIp( const char*ip);
	std::string getIp();//ip
	std::string toAddrString();//ip:port
	int  distance( CNetAddr &addr );//计算两个ip的距离
	void setSockAddr( sockaddr_in &addr );
	sockaddr_in makeSockAddr();

	
private:
	int _ip;
	int _port;
};

