#include "StdAfx.h"
#include "base_file.h"
#include <io.h>
#include "base_string.h"


CBaseFile::CBaseFile( void )
{
	m_pFile = NULL;
	m_bOpen = false;
}

CBaseFile::~CBaseFile( void )
{
	Close();
}

bool CBaseFile::Open( LPCTSTR lpszFileName, int nMode )
{
	// 未防止文件未关闭打开另一文件，因此先判断文件指针是否标志一个文件
	if( m_pFile != NULL )
	{
		// 关闭先前打开的文件
		if( !Close( ) )
		{
			return false;
		}
	}

	CBaseString strOpenMode = _T("");

	// 判断打开文件的读写方式
	switch( nMode & 0x00000fff )
	{
	case BASE_READ:
		strOpenMode = _T("r");
		break;
	case BASE_WRITE:
	case (BASE_READ|BASE_WRITE):
		strOpenMode = _T("r+");
		break;
	case (BASE_READ|BASE_BINARY):
		strOpenMode = _T("rb");
		break;
	case (BASE_WRITE|BASE_BINARY):
	case (BASE_READ|BASE_WRITE|BASE_BINARY):
		strOpenMode = _T("r+b");
		break;
	case BASE_CREATE:
	case (BASE_WRITE|BASE_CREATE):
		strOpenMode = _T("w");
		break;
	case (BASE_WRITE|BASE_CREATE|BASE_BINARY):
		strOpenMode = _T("wb");
		break;
	case (BASE_READ|BASE_WRITE|BASE_CREATE):
		strOpenMode = _T("w+");
		break;
	case (BASE_READ|BASE_WRITE|BASE_CREATE|BASE_BINARY):
		strOpenMode = _T("w+b");
		break;
	case (BASE_READ|BASE_APP):
	case (BASE_WRITE|BASE_APP):
	case (BASE_READ|BASE_WRITE|BASE_APP):
		strOpenMode = _T("a+");
		break; 
	default:
		return false;
		break;
	}

	// 打开文件
	if( _tfopen_s( &m_pFile, lpszFileName, strOpenMode.c_str( ) ) != 0 )
	{
	    return false;
	}

	// 判断文件打开是否成功
	if(  m_pFile == NULL )
	{
		return false;
	}
	m_bOpen = true;
	return true;
}

bool CBaseFile:: Close( void )
{
	if( m_pFile != NULL )
	{
		if( fclose( m_pFile ) != 0 )
		{
			return false;
		}
		m_pFile = NULL;
	}
	m_bOpen = false;
	return true;
}

int CBaseFile::Read( void * pBuf, int nLen )
{
	// 判断文件指针是否为空
	if( !m_bOpen || m_pFile == NULL )
	{
		return false;
	}

	size_t nReadLen = fread( pBuf, sizeof( char ), nLen, m_pFile ); // 读取nLen个字符

	// 如果读取的长度小于指定的长度，则判断读取数据时是否遇到错误；如果没有错误表明读到文件末尾
	if(  ferror( m_pFile ) != 0 )
	{
		return BASE_ERROR;
	}
	else
	{
		return nReadLen;
	}
}

int CBaseFile::Write ( void * pBuf, int nLen )
{
	if( !m_bOpen || m_pFile == NULL )
	{
		return false;
	}
	// lcy note 2008_10_10:如果文件没有办法写入指定的大小，
	// 则如何处理，有可能全写入时超过文件的指定大小，此时是否需要扩大文件的大小然后执行写操作

	int nWriteLen = fwrite( pBuf, sizeof( char ), nLen, m_pFile ); // 写入nLen个字符

	// 如果写入的长度小于指定的长度，则表明写操作失败
	if(  ferror( m_pFile ) != 0 )
	{
		return BASE_ERROR;
	}
	else
	{
		return nWriteLen;
	}
}

bool CBaseFile::Seek( int nPosFlag, __int64 nOffset )
{	
	// 判断文件指针是否为空
	if( ( !m_bOpen ) || ( m_pFile == NULL ) )
	{
		return false;
	}

	if ( 0 != _fseeki64( m_pFile, nOffset, nPosFlag ) )
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool CBaseFile::IsOpen ( void )
{
	return m_bOpen;
}

bool CBaseFile::SetFileSize( __int64 nFileSize )
{
	// 判断文件是否已经关闭
	if( !m_bOpen || m_pFile == NULL )
	{
		return false;
	}

	// 设置文件大小
	if( _chsize_s( _fileno( m_pFile ), nFileSize ) != 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

__int64 CBaseFile::GetFileSize( void )
{
	if( !m_bOpen || m_pFile == NULL )
	{
		return 0;
	}

	__int64 nCurPos = _ftelli64( m_pFile );//获取当前位置
// 	将文件指针定位到末尾
 	if( _fseeki64( m_pFile, 0, SEEK_END ) != 0 )
	{
 		return 0;
 	}
 	__int64 nFileSize = _ftelli64( m_pFile ); // 获取文件指针的位置，从而取得文件大小
	
	_fseeki64( m_pFile, nCurPos, SEEK_SET );//恢复当前位置
 	return nFileSize;
}

bool CBaseFile::IsEof()
{
	return feof(m_pFile)>0;
}
