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
	// δ��ֹ�ļ�δ�رմ���һ�ļ���������ж��ļ�ָ���Ƿ��־һ���ļ�
	if( m_pFile != NULL )
	{
		// �ر���ǰ�򿪵��ļ�
		if( !Close( ) )
		{
			return false;
		}
	}

	CBaseString strOpenMode = _T("");

	// �жϴ��ļ��Ķ�д��ʽ
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

	// ���ļ�
	if( _tfopen_s( &m_pFile, lpszFileName, strOpenMode.c_str( ) ) != 0 )
	{
	    return false;
	}

	// �ж��ļ����Ƿ�ɹ�
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
	// �ж��ļ�ָ���Ƿ�Ϊ��
	if( !m_bOpen || m_pFile == NULL )
	{
		return false;
	}

	size_t nReadLen = fread( pBuf, sizeof( char ), nLen, m_pFile ); // ��ȡnLen���ַ�

	// �����ȡ�ĳ���С��ָ���ĳ��ȣ����ж϶�ȡ����ʱ�Ƿ������������û�д�����������ļ�ĩβ
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
	// lcy note 2008_10_10:����ļ�û�а취д��ָ���Ĵ�С��
	// ����δ����п���ȫд��ʱ�����ļ���ָ����С����ʱ�Ƿ���Ҫ�����ļ��Ĵ�СȻ��ִ��д����

	int nWriteLen = fwrite( pBuf, sizeof( char ), nLen, m_pFile ); // д��nLen���ַ�

	// ���д��ĳ���С��ָ���ĳ��ȣ������д����ʧ��
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
	// �ж��ļ�ָ���Ƿ�Ϊ��
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
	// �ж��ļ��Ƿ��Ѿ��ر�
	if( !m_bOpen || m_pFile == NULL )
	{
		return false;
	}

	// �����ļ���С
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

	__int64 nCurPos = _ftelli64( m_pFile );//��ȡ��ǰλ��
// 	���ļ�ָ�붨λ��ĩβ
 	if( _fseeki64( m_pFile, 0, SEEK_END ) != 0 )
	{
 		return 0;
 	}
 	__int64 nFileSize = _ftelli64( m_pFile ); // ��ȡ�ļ�ָ���λ�ã��Ӷ�ȡ���ļ���С
	
	_fseeki64( m_pFile, nCurPos, SEEK_SET );//�ָ���ǰλ��
 	return nFileSize;
}

bool CBaseFile::IsEof()
{
	return feof(m_pFile)>0;
}
