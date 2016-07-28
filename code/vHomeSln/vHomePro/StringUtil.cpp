#include <windows.h>
#include <string>
using namespace std;

//*tchar是TCHAR类型指针，*_char是char类型指针   
void TcharToChar (const TCHAR * tchar, char * _char)  
{  
	int iLength ;  
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);  
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);   
}  

//char转TCHAR   
void CharToTchar (const char * _char, TCHAR * tchar)  
{  
	int iLength ;  

	iLength = MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, NULL, 0) ;  
	MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, tchar, iLength) ;  
} 


string StringFromLPCTSTR(LPCTSTR str) {
#ifdef _UNICODE
	int size_str = WideCharToMultiByte(CP_ACP, 0, str, -1, 0, 0, NULL, NULL);

	char* point_new_array = new char[size_str];

	WideCharToMultiByte(CP_ACP, 0, str, -1, point_new_array, size_str, NULL, NULL);

	std::string return_string(point_new_array);
	delete[] point_new_array;
	point_new_array = NULL;
	return return_string;
#else
	return std::string(str);
#endif
}

std::wstring s2ws(const std::string& s)

{

	int len;

	int slength = (int)s.length() + 1;

	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);

	wchar_t* buf = new wchar_t[len];

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);

	std::wstring r(buf);

	delete[] buf;

	return r.c_str();

}

string nByteToStr(DWORD dwSize, BYTE* pData, DWORD dwByte, DWORD dwSplit)
{
	BYTE* pBuf = (BYTE*)pData; // local pointer to a BYTE in the BYTE array.

	string strRet("");	
	DWORD nLine = 0;	
	DWORD dwLines = 0;
	DWORD dwRest = 0;
	bool bNeedSplit = true;
	char szTemp[20] = {0, };

	DWORD dwBlock = 0;	
	if(0 == dwSplit)
	{
		dwSplit = dwSize;
		bNeedSplit = false;
	}

	dwRest = dwSize % dwSplit;
	dwLines = dwSize / dwSplit;


	DWORD i, j, k, m;
	for(i = 0; i < dwLines; i++)
	{
		DWORD dwRestTemp = dwSplit % dwByte;
		DWORD dwByteBlock = dwSplit / dwByte;

		for(j = 0; j < dwByteBlock; j++)
		{
			for(k = 0; k < dwByte; k++)
			{
				//wsprintf(szTemp, "%02X", pBuf[i * dwSplit + j * dwByte + k]);
				sprintf(szTemp, "%02X", pBuf[i * dwSplit + j * dwByte + k]);
				strRet += szTemp;
			}
			strRet += " ";
		}
		if(dwRestTemp)
		{
			for(m = 0; m < dwRestTemp; m++)
			{
				sprintf(szTemp, "%02X",pBuf[i * dwSplit + j * dwByte + k - 3 + dwRestTemp]);
				//wsprintf(
				//	szTemp, "%02X",
				//	pBuf[i * dwSplit + j * dwByte + k - 3 + dwRestTemp]);
				strRet += szTemp;
			}
		}
		//if(bNeedSplit)
		//	strRet += NEWLINE;
	}

	if(dwRest)
	{
		DWORD dwRestTemp = dwRest % dwByte;
		DWORD dwByteBlock = dwRest / dwByte;
		for(j = 0; j < dwByteBlock; j++)
		{
			for(k = 0; k < dwByte; k++)
			{
				//wsprintf(szTemp, "%02X", pBuf[dwSize - dwRest + k]);
				sprintf(szTemp, "%02X", pBuf[dwSize - dwRest + k]);
				strRet += szTemp;
			}
			strRet += " ";
		}
		if(dwRestTemp)
		{
			for(m = 0; m < dwRestTemp; m++)
			{
				//wsprintf(
				//	szTemp, "%02X",
				//	pBuf[dwSize - dwRest + k - 1 + dwRestTemp]);
				sprintf(szTemp, "%02X", pBuf[dwSize - dwRest + k - 1 + dwRestTemp]);
				strRet += szTemp;
			}
		}
		//if(bNeedSplit)
		//	strRet += NEWLINE;
	}


	return strRet;
}

string LoadMyImage(const string& strDest)
{
	OPENFILENAME opfn;  
	WCHAR strFilename[MAX_PATH];//存放文件名  
	//初始化  
	ZeroMemory(&opfn, sizeof(OPENFILENAME));  
	opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
	//设置过滤  MP3文件\0*.mp3\0
	opfn.lpstrFilter = L"所有文件\0*.*\0图片文件\0*.jpg\0*.png\0";  
	//默认过滤器索引设为1  
	opfn.nFilterIndex = 1;  
	//文件名的字段必须先把第一个字符设为 \0  
	opfn.lpstrFile = strFilename;  
	opfn.lpstrFile[0] = '\0';  
	opfn.nMaxFile = sizeof(strFilename);  
	//设置标志位，检查目录或文件是否存在  
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;  
	//opfn.lpstrInitialDir = NULL;  
	// 显示对话框让用户选择文件  
	if(GetOpenFileName(&opfn))  
	{  
		char strFile[256];
		TcharToChar(strFilename, strFile);
		FILE* fpFile = fopen(strFile, "rb");
		if (fpFile == NULL)
			return "";
		fseek(fpFile, 0, SEEK_END);
		int nFileLen = 0;
		if ((nFileLen = ftell(fpFile)) <1)
		{
			fclose(fpFile);
			return "";
		}
		byte* buf = new byte[nFileLen];
		if (buf == NULL)
		{
			return "";
		}
		fseek(fpFile, 0, SEEK_SET);

		int nCount = fread(buf, nFileLen, 1, fpFile);


		fclose(fpFile);
		string strResult= nByteToStr(nFileLen, buf, 1, 16);

		FILE* outFile = NULL;

		outFile = fopen(strDest.c_str(), "wb");
		if(outFile == NULL)
		{
			DWORD dwError = GetLastError();
			delete buf;
			buf = NULL;
			return "";
		}

		fwrite(buf, nFileLen, 1, outFile);
		fclose(outFile);

		delete buf;
		buf = NULL;
		return strResult;
	}
	return "";
}