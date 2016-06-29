#include "Base64.h"

string CBase64::Encode(const string & data)
{
	const unsigned char * pData = (unsigned char *)data.data();
	int dataLen = data.size();
	
    //编码表
    const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //返回值
    string strEncode;
    unsigned char Tmp[4]={0};
    int LineLength=0;
    for(int i=0;i<(int)(dataLen / 3);i++)
    {
        Tmp[1] = *pData++;
        Tmp[2] = *pData++;
        Tmp[3] = *pData++;
        strEncode+= EncodeTable[Tmp[1] >> 2];
        strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
        strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
        strEncode+= EncodeTable[Tmp[3] & 0x3F];
        if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
    }
    //对剩余数据进行编码
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

string CBase64::Decode(const string & data)
{
	const char * pData = data.c_str();
	int dataLen = strlen(data.c_str());
	
    //解码表
    const char DecodeTable[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			62, // '+'
			0, 0, 0,
			63, // '/'
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
			0, 0, 0, 0, 0, 0, 0,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
			0, 0, 0, 0, 0, 0,
			26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
			39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
    };
    //返回值
    string strDecode;
    int nValue;
    int i= 0;
    while (i < dataLen)
    {
        if (*pData != '\r' && *pData!='\n')
        {
            nValue = DecodeTable[*pData++] << 18;
            nValue += DecodeTable[*pData++] << 12;
            strDecode+=(nValue & 0x00FF0000) >> 16;
			if (*pData != '=')
            {
                nValue += DecodeTable[*pData++] << 6;
                strDecode+=(nValue & 0x0000FF00) >> 8;
                if (*pData != '=')
                {
                    nValue += DecodeTable[*pData++];
                    strDecode+=nValue & 0x000000FF;
                }
            }
            i += 4;
        }
        else// 回车换行,跳过
        {
            pData++;
            i++;
        }
	}

	return strDecode;
}
