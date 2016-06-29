#include "StdAfx.h"
#include "Buffer.h"


CBuffer::CBuffer(void):_buf(NULL),_len(0)
{
}


CBuffer::~CBuffer(void)
{
	ReleaseBuffer();
}

char *CBuffer::GetBuffer( int len )
{
	ReleaseBuffer();
	_buf = CMemoryManager::Instance()->GetMemory( len );
	_len = len;
	return _buf;
}
void CBuffer::ReleaseBuffer()
{
	if ( _buf )
	{
		CMemoryManager::Instance()->FreeMemory( _buf,false );
		_buf = NULL;
	}
}
char* CBuffer::operator =( CBuffer &buffer)
{
	ReleaseBuffer();
	_buf = buffer._buf;
	_len = buffer._len;
	return _buf;
}
