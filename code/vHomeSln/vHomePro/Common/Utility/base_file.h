#pragma once
#include <Windows.h>

#define BASE_ERROR               ( -1 ) // 文件结尾位置 010 000

// 定义文件打开方式的宏
#define BASE_READ           ( 0x00000001 ) // 只读，如果没有找到或者没有打开，打开将会失败
#define BASE_WRITE          ( 0x00000002 ) // 以只写的方式打开，如果同名文件存在，原来的数据丢失
#define BASE_APP            ( 0x00000004 )
#define BASE_CREATE         ( 0x00000008 ) // 以二进制方式打开文件
#define BASE_BINARY         ( 0x00000010 ) // 以文本方式打开文件
#define BASE_TEXT           ( 0x00000020 )


class  CBaseFile
{
public:
	CBaseFile( void );
	~CBaseFile( void );
	/*
	功能：打开文件
	参数：lpszFileName ---文件名指针
			nMode        ---文件打开方式
	返回：打开成功返回true，失败返回false
	备注：
	*/
	virtual bool Open( LPCTSTR lpszFileName, int nMode );
	/*
	功能：关闭文件
	参数：
	返回：关闭成功返回true，否则返回false 
	备注：
	*/
	virtual bool Close( void );
	/*
	功能：读取数据
	参数：pBuf ---缓冲区
			nLen ---缓冲区的大小
	返回：读取的字节数
	备注：
	*/
 	virtual int Read( void * pBuf, int nLen );
	/*
	功能：向文件中写数据
	参数：pBuf ---缓冲区
			nLen ---缓冲区的大小
	返回：实际写入缓冲区的字节数,写入操作遇到错误返回-1
	备注：
	*/
	virtual int Write( void * pBuf, int nLen );
	/*
	功能：设置文件指针
	参数：nPosFlag ---位置标记
			nOffset  ---相对文件起始位置的偏移量
	返回：设置成功返回true，否则返回false
	备注：
	*/
	virtual bool Seek( int nPosFlag, __int64 nOffset );
	/*
	功能：判断文件是否已经打开
	参数：
	返回：打开返回true，关闭返回false
	备注：
	*/
	virtual bool IsOpen( void );
	/*
	功能：获取文件大小
	参数：
	返回：文件的实际大小
	备注：
	*/
	virtual __int64 GetFileSize( void );
	/*
	功能：设置文件大小
	参数：nFileSize ---文件大小
	返回：设置成功返回true，失败返回false
	备注：
	*/
	virtual bool SetFileSize( __int64 nFileSize );
	
	virtual bool IsEof();

protected:
	bool m_bOpen;  // 标志文件是否打开
	FILE *m_pFile; // 文件指针
	
};
