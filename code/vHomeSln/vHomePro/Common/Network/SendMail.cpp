#include "SendMail.h"

string ReadRecv(SOCKET sock)
{
	char recvBuff[1024*8] = { 0 };
	memset(recvBuff,0,sizeof(recvBuff));
	recv(sock,recvBuff,1000,0);
	printf("%s",recvBuff);
	return recvBuff;
}

string Base64Encode(const string & data)
{
	const unsigned char * pData = (unsigned char *)data.data();
	int dataLen = data.size();

	const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	string strEncode;
	unsigned char Tmp[4]={0};
	for(int i=0;i<(int)(dataLen / 3);i++)
	{
		Tmp[1] = *pData++;
		Tmp[2] = *pData++;
		Tmp[3] = *pData++;
		strEncode+= EncodeTable[Tmp[1] >> 2];
		strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode+= EncodeTable[Tmp[3] & 0x3F];
	}

	int Mod=dataLen % 3;
	if(Mod==1)
	{
		Tmp[1] = *pData++;
		strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode+= "==";
	}
	else if(Mod==2)
	{
		Tmp[1] = *pData++;
		Tmp[2] = *pData++;
		strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode+= "=";
	}

	return strEncode;
}

int SendMail(CMailInfo mailInfo)
{
	#if (defined(_WIN32) || defined(_WIN64))
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 1, 1 );
	WSAStartup( wVersionRequested, &wsaData );
	#endif

	string res;

	sockaddr_in serverAddr;
	SOCKET sock;
	hostent * host;
	host = gethostbyname(mailInfo.server.c_str());
	if(host==NULL)
	{
		return -1;
	}

	sock = socket(AF_INET, SOCK_STREAM,0);
	serverAddr.sin_addr = *(struct in_addr*)host->h_addr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(mailInfo.port);//25

	if(connect(sock,(const sockaddr*)&serverAddr,sizeof(sockaddr))==-1)
	{
		closesocket(sock);
		return -1;
	}

	char buffer[100*1024] = { 0 }; 

	snprintf(buffer, sizeof(buffer), "EHLO %s\r\n", mailInfo.from.c_str());//from为char数据。存储发送地址
	send(sock,buffer,strlen(buffer),0);

	ReadRecv(sock);

	snprintf(buffer, sizeof(buffer), "AUTH LOGIN\r\n");
	send(sock,buffer,strlen(buffer),0);

	ReadRecv(sock);

	//USER NAME
	//User name is coded by base64.
	string u = Base64Encode(mailInfo.user);//先将用户帐号经过base64编码
	u += "\r\n";
	send(sock,u.c_str(),u.size(),0);

	ReadRecv(sock);

	//password coded by base64
	string p = Base64Encode(mailInfo.pass);//先将密码经过base64编码
	p += "\r\n";
	send(sock,p.c_str(),p.size(),0);

	ReadRecv(sock);


	snprintf(buffer, sizeof(buffer), "MAIL FROM:<%s>\r\n", mailInfo.from.c_str());
	send(sock,buffer,strlen(buffer),0);

	ReadRecv(sock);

	int i;
	for(i = 0; i < mailInfo.to.size(); i++)
	{
		snprintf(buffer, sizeof(buffer), "RCPT TO:<%s>\r\n", mailInfo.to[i].c_str());
		send(sock,buffer,strlen(buffer),0);
		ReadRecv(sock);
	}

	//memset(buffer,0,sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "DATA\r\n");
	send(sock,buffer,strlen(buffer),0);
	ReadRecv(sock);


	snprintf(buffer, sizeof(buffer), "Subject:%s\r\n\r\n", mailInfo.subject.c_str());
	//DATA head
	send(sock,buffer,strlen(buffer),0);

	snprintf(buffer, sizeof(buffer), "%s\r\n.\r\n", mailInfo.body.c_str());
	//DATA body
	send(sock,buffer,strlen(buffer),0);
	ReadRecv(sock);

	snprintf(buffer, sizeof(buffer), "QUIT\r\n");
	send(sock,buffer,strlen(buffer),0);
	res =ReadRecv(sock);
	closesocket(sock);

	#if (defined(_WIN32) || defined(_WIN64))
	//WSACleanup(); 
	#endif

	if(res.substr(0, 3) == "250" || res.substr(0, 3) == "221")
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


