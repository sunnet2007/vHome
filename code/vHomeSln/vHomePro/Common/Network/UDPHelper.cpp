#include "UDPHelper.h"
#include "MsgPack.h"

int CUDPHelper::CreateSock()
{
	#if (defined(_WIN32) || defined(_WIN64))
	
	WORD wV;
	WSADATA ws;
	int er;
	wV = MAKEWORD(1, 1);
	er = WSAStartup(wV, &ws);
	if(er != 0)
	{
		return -1;
	}
	
	if(LOBYTE(ws.wVersion) != 1 || HIBYTE(ws.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	
	#endif

	return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

int CUDPHelper::CloseSock(int sock)
{
	return closesocket(sock);
}

int CUDPHelper::Bind(int sock, int port)
{
	sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr.s_addr = INADDR_ANY;

	return bind(sock, (sockaddr*)&clientAddr, sizeof(clientAddr));
}

int CUDPHelper::Bind(int sock, const string & ip, int port)
{
	sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr.s_addr = inet_addr(ip.c_str());

	return bind(sock, (sockaddr*)&clientAddr, sizeof(clientAddr));
}

int CUDPHelper::SendTo(int sock, const void * buf, int len, const string & descAddr, int descPort)
{
	string ip;
	if(ntohl(inet_addr(descAddr.c_str())) != 0xffffffff	&& ntohl(inet_addr(descAddr.c_str())) > ntohl(inet_addr("1.0.0.0")))
	{
		ip = descAddr;
	}
	else
	{
		struct hostent * phe;
		phe = gethostbyname(descAddr.c_str()); 
		if (phe == NULL)
		{
			return -1;  
		} 
		else
		{
			struct in_addr addr;
			addr.s_addr = *(unsigned long *) phe->h_addr_list[0]; 
			ip = inet_ntoa(addr);
		}
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(descPort);
	servAddr.sin_addr.s_addr = inet_addr(ip.c_str());

	int ret = sendto(sock, (const char *)buf, len, 0, (sockaddr*)&servAddr,sizeof(servAddr));
	
	return ret;
}

int CUDPHelper::SendTo(int sock, CMsgPack args, const string & descAddr, int descPort)
{
	string s;
	args.Encode(s);

	return SendTo(sock, s.data(), s.size(), descAddr, descPort);
}

int CUDPHelper::RecvFrom(int sock, void * buf, int len, string & srcIP, int & srcPort, int timeout)
{
	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	srcIP = "";
	srcPort = -1;

	sockaddr_in clientAddr;
	socklen_t size = sizeof(clientAddr);
	int ret = -1;

	int startTime = clock();
	while(true)
	{
		ret = recvfrom(sock, (char *)buf, len, 0, (sockaddr*)&clientAddr, &size);
		if(ret == -1)
		{
			int tryAgain = 0;

			#if (defined(_WIN32) || defined(_WIN64))
			if(GetLastError() == 10035)
			{
				tryAgain = 1;
			}
			#else
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				tryAgain = 1;
			}
			#endif

			if(tryAgain)
			{
				if(timeout >= 0 && clock() - startTime >= timeout)
				{
					break;
				}
				else
				{
					Sleep(10);
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			srcIP = inet_ntoa(clientAddr.sin_addr);
			srcPort = ntohs(clientAddr.sin_port);
			break;
		}
	}

	return ret;
}

int CUDPHelper::RecvFrom(int sock, void * buf, int len, int timeout)
{
	string srcIP;
	int srcPort;
	return RecvFrom(sock, buf, len, srcIP, srcPort, timeout);
}

int CUDPHelper::RecvFrom(int sock, CMsgPack & args, string & srcIP, int & srcPort, int timeout)
{
	args.Clear();

	char buf[64*1024] = { 0 };
	int len = RecvFrom(sock, buf, sizeof(buf), srcIP, srcPort, timeout);
	if(len <= 0)
	{
		return -1;
	}

	string s;
	s.assign(buf, len);

	return args.Decode(s);
}

int CUDPHelper::RecvFrom(int sock, CMsgPack & args, int timeout)
{
	string srcIP;
	int srcPort;
	return RecvFrom(sock, args, srcIP, srcPort, timeout);
}



int CUDPHelper::RecvFrom(int sock, void * buf, int len, sockaddr_in &addr,int timeout )
{
#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
#endif


	socklen_t size = sizeof(addr);
	int ret = -1;

	int startTime = clock();
	while(true)
	{
		ret = recvfrom(sock, (char *)buf, len, 0, (sockaddr*)&addr, &size);
		if(ret == -1)
		{
			int tryAgain = 0;

#if (defined(_WIN32) || defined(_WIN64))
			if(GetLastError() == 10035)
			{
				tryAgain = 1;
			}
#else
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				tryAgain = 1;
			}
#endif

			if(tryAgain)
			{
				if(timeout >= 0 && clock() - startTime >= timeout)
				{
					break;
				}
				else
				{
					Sleep(10);
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return ret;
}
int CUDPHelper::SendTo(int sock, void * buf, int len, sockaddr_in &addr)
{
	return sendto(sock, (const char *)buf, len, 0, (sockaddr*)&addr,sizeof(addr));
}