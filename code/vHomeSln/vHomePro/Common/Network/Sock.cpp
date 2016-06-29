#if (defined(_WIN32) || defined(_WIN64))
#include "stdafx.h"
#endif
#include <errno.h>
#include "Sock.h"

static int64 GetMilSec()
{
	#if (defined(_WIN32) || defined(_WIN64))

	return clock();

	#else

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);

	return tv.tv_sec*1000+tv.tv_usec/1000;

	#endif
}

bool IsBrokenSock(int err)
{
#if (defined(_WIN32) || defined(_WIN64))
	if(err == 10035)
	{
		return false;
	}
	else
	{
		return true;
	}
#else
	if(err == EINTR || err == EWOULDBLOCK || err == EAGAIN)
	{
		return false;
	}
	else
	{
		return true;
	}
#endif
}

int SetSockNonBlock(int sock)
{
#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
#endif

	return 0;
}

int RecvFromSock(int sock, void * buf, int len, int timeout)
{
	int64 startTime = GetMilSec();
	int recvLen = 0;
	while(recvLen < len)
	{
		int ret = recv(sock, (char *)buf + recvLen, len - recvLen, 0);
		int en = GetLastError();;
		if(ret < 0)
		{
			if(IsBrokenSock(en))
			{
				if(recvLen > 0)
					return recvLen;
				else
					return -1;
			}
			else
			{
				if(timeout >= 0 && GetMilSec() - startTime >= timeout)
					break;

				Sleep(1);
				continue;
			}
		}
		else if(ret == 0)
		{
			if(recvLen > 0)
				return recvLen;
			else
				return -1;
		}

		recvLen += ret;
	}

	return recvLen;
}

int SendToSock(int sock, const void * buf, int len, int timeout)
{
	int64 startTime = GetMilSec();
	int sentLen = 0;
	while(sentLen < len)
	{
		int ret = send(sock, (char *)buf + sentLen, len - sentLen, 0);
		int en = GetLastError();
		if(ret < 0)
		{
			if(IsBrokenSock(en))
			{
				if(sentLen > 0)
					return sentLen;
				else
					return -1;
			}
			else
			{
				if(timeout >= 0 && GetMilSec() - startTime >= timeout)
					break;

				Sleep(1);
				continue;
			}
		}
		else if(ret == 0)
		{
			if(sentLen > 0)
				return sentLen;
			else
				return -1;
		}

		sentLen += ret;
	}

	return sentLen;
}

int CTcpSock::Create()
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

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		return -1;

	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	return sock;
}

int CTcpSock::Close(int sock)
{
	if(sock != -1)
	{
		try
		{
			closesocket(sock);
		}
		catch (...)
		{
			int err = GetLastError();
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int CTcpSock::Connect(int sock, string host, int port, int timeout)
{
	int optval;
	socklen_t optlen = sizeof(int);  
	if(getsockopt(sock, SOL_SOCKET, SO_ERROR,(char*)&optval, &optlen) != 0)
	{
		return -1;
	}

	if(timeout <= 0)
	{
		timeout = 0;
	}

	string ip;
	if(ntohl(inet_addr(host.c_str())) != 0xffffffff
		&& ntohl(inet_addr(host.c_str())) > ntohl(inet_addr("1.0.0.0")))
	{
		ip = host;
	}
	else
	{
		struct hostent * phe;
		phe = gethostbyname(host.c_str()); 
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

	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	servAddr.sin_port = htons(port);

    #if (defined(_WIN32) || defined(_WIN64))
    unsigned long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
    #else
    fcntl(sock, F_SETFL, O_NONBLOCK);
    #endif

	connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr));

	struct timeval tm;
	tm.tv_sec  = 0;
	tm.tv_usec = timeout*1000;

	fd_set set;
	FD_ZERO(&set);
	FD_SET(sock, &set);

	if(select(sock+1, NULL, &set, NULL, &tm) > 0)
	{
		if(FD_ISSET(sock, &set))
		{
			return 0;
		}
	}

	return -1;
}

int CTcpSock::Send(int sock, const void * data, int length)
{
	if(sock <= 0 || data == NULL || length < 0)
		return -1;

	int sentLen = 0;
	while(sentLen < length)
	{
		int optval; 
		socklen_t optlen = sizeof(int);
		if(getsockopt(sock, SOL_SOCKET, SO_ERROR,(char*)&optval, &optlen) != 0)
		{
			return -1;
		}

		fd_set set;
		FD_ZERO(&set);
		FD_SET(sock, &set);

		struct timeval tm;
		tm.tv_sec  = 0;
		tm.tv_usec = 10*1000;

		if(select(sock+1, NULL, &set, NULL, &tm) > 0)
		{
			int len;
			len = send(sock, ((const char *)data)+sentLen, length-sentLen, 0);
			if(len <= 0)
			{
				break;
			}
			sentLen += len;
		}
	}

	return sentLen;
}

int CTcpSock::Send(int sock, const string & data)
{
	return Send(sock, data.data(), data.size());
}

int CTcpSock::Recv(int sock, void * data, int * length, int timeout)
{
	if(sock <= 0 || length == NULL)
		return -1;

	if(*length <= 0)
	{
		*length = 0;
		return 0;
	}

	int selectTimeout;
	if(timeout > 0)
	{
		selectTimeout = timeout;
	}
	else
	{
		selectTimeout = 10;
	}

	int ret = 0;
	int recvd_len = 0;
	while(1)
	{
		int optval; 
		socklen_t optlen = sizeof(int);
		if(getsockopt(sock, SOL_SOCKET, SO_ERROR,(char*)&optval, &optlen) != 0)
		{
			ret = -1;
			break;
		}

		fd_set set;
		FD_ZERO(&set);
		FD_SET(sock, &set);

		struct timeval tm;
		tm.tv_sec  = 0;
		tm.tv_usec = selectTimeout*1000;

		if(select(sock+1, &set, NULL, NULL, &tm) > 0)
		{
			int len = *length - recvd_len;
			len = recv(sock, ((char *)data)+recvd_len, len, 0);
			if(len < 0) //Ê§°Ü
			{
				ret = -1;
				break;
			}
			else if(len == 0) //¶Ï¿ª
			{
				ret = -2;
				break;
			}

			recvd_len += len;
			if(recvd_len == *length)
			{
				ret = 0;
				break;
			}
		}
		else
		{
			if(timeout > 0) //³¬Ê±
			{
				ret = -3;
				break;
			}
		}
	}

	*length = recvd_len;

	return ret;
}

int CTcpSock::Listen(int sock, const string ip, int port)
{
	if(sock <= 0)
		return -1;
	int optval; 
	socklen_t optlen = sizeof(int);
	if(getsockopt(sock, SOL_SOCKET, SO_ERROR,(char*)&optval, &optlen) != 0)
	{
		return -1;
	}
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	if(ip.size() == 0)
		serv_addr.sin_addr.s_addr = INADDR_ANY;
	else
		serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	serv_addr.sin_port = htons(port);

    
	if( bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
	{
		return -1;
	}
	
	if(listen(sock, 1000) == -1)
	{
		return -1;
	}

	optval = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, optlen) == -1)
	{
		closesocket(sock);
		return -1;
	}

	return 0;
}

int CTcpSock::Listen(int sock, int port)
{
	return Listen(sock, "", port);
}
int CTcpSock::Accept(int sock, string & ip, int & port)
{
	if(sock <= 0)
		return -1;

	int client_sock = -1;
	struct sockaddr_in client_addr;
	memset(&client_addr,0,sizeof(client_addr));
	socklen_t addrlen = sizeof(client_addr);

	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 0;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	int flags = fcntl(sock, F_GETFL, 0);
	flags &= ~O_NONBLOCK;
	fcntl(sock, F_SETFL, flags);
	#endif

	client_sock = accept(sock, (sockaddr*)&client_addr, &addrlen);
	if(client_sock == -1)
	{
		return -1;
	}

	ip = inet_ntoa(client_addr.sin_addr);
	port = ntohs(client_addr.sin_port);

	return client_sock;
}
int CTcpSock::Accept(int sock,CNetAddr &addr)
{
	if(sock <= 0)
		return -1;

	int client_sock = -1;
	struct sockaddr_in client_addr;
	memset(&client_addr,0,sizeof(client_addr));
	socklen_t addrlen = sizeof(client_addr);

#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 0;
	ioctlsocket(sock, FIONBIO, &mode);
#else
	int flags = fcntl(sock, F_GETFL, 0);
	flags &= ~O_NONBLOCK;
	fcntl(sock, F_SETFL, flags);
#endif

	client_sock = accept(sock, (sockaddr*)&client_addr, &addrlen);
	if(client_sock == -1)
	{
		return -1;
	}

	addr.setSockAddr( client_addr );

	return client_sock;
}
int CTcpSock::Accept(int sock)
{
	string ip;
	int port;
	return Accept(sock, ip, port);
}

int CTcpSock::CanRead(int sock)
{
	fd_set set;
	FD_ZERO(&set);
	FD_SET(sock, &set);

	struct timeval tm;
	tm.tv_sec  = 0;
	tm.tv_usec = 0;

	if(select(sock+1, &set, NULL, NULL, &tm) > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CTcpSock::CanWrite(int sock)
{
	fd_set set;
	FD_ZERO(&set);
	FD_SET(sock, &set);

	struct timeval tm;
	tm.tv_sec  = 0;
	tm.tv_usec = 0;

	if(select(sock+1, NULL, &set, NULL, &tm) > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
int CUdpSock::Create()
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

int CUdpSock::Close(int sock)
{
	return closesocket(sock);
}

int CUdpSock::Bind(int sock, int port)
{
	sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr.s_addr = INADDR_ANY;

	return bind(sock, (sockaddr*)&clientAddr, sizeof(clientAddr));
}

int CUdpSock::Bind(int sock, const string & ip, int port)
{
	sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr.s_addr = inet_addr(ip.c_str());

	return bind(sock, (sockaddr*)&clientAddr, sizeof(clientAddr));
}

int CUdpSock::SendTo(int sock, const void * buf, int len, const string & descAddr, int descPort)
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

int CUdpSock::SendTo(int sock, CUDPMsg & msg)
{
	return SendTo(sock, msg.data.data(), msg.data.size(), msg.IP, msg.port);
}

int CUdpSock::RecvFrom(int sock, void * buf, int len, string & srcIP, int & srcPort, int timeout)
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

	int startTime = GetMilSec();
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
				if(timeout >= 0 && GetMilSec() - startTime >= timeout)
				{
					break;
				}
				else
				{
					#if (defined(_WIN32) || defined(_WIN64))
					Sleep(1);
					#else
					usleep(1000);
					#endif
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

int CUdpSock::RecvFrom(int sock, void * buf, int len, int timeout)
{
	string srcIP;
	int srcPort;
	return RecvFrom(sock, buf, len, srcIP, srcPort, timeout);
}

int CUdpSock::RecvFrom(int sock, CUDPMsg & msg, int timeout)
{
	msg.Clear();

	string IP;
	int port;
	char buf[64*1024] = { 0 };
	int len = RecvFrom(sock, buf, sizeof(buf), IP, port, timeout);
	if(len <= 0)
	{
		return -1;
	}

	msg.sock = sock;
	msg.IP = IP;
	msg.port = port;
	msg.data.assign(buf, len);

	return 0;
}

int CUdpSock::CanRead(int sock)
{
	fd_set set;
	FD_ZERO(&set);
	FD_SET(sock, &set);

	struct timeval tm;
	tm.tv_sec  = 0;
	tm.tv_usec = 0;

	if(select(sock+1, &set, NULL, NULL, &tm) > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CUdpSock::CanWrite(int sock)
{
	fd_set set;
	FD_ZERO(&set);
	FD_SET(sock, &set);

	struct timeval tm;
	tm.tv_sec  = 0;
	tm.tv_usec = 0;

	if(select(sock+1, NULL, &set, NULL, &tm) > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
*/
