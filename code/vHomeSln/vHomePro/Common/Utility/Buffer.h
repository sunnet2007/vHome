#pragma once
#include "../Pool/MemoryManage.h"


class  CBuffer
{
public:
	CBuffer(void);
	~CBuffer(void);
	char *GetBuffer( int len );
	void ReleaseBuffer();
	char* operator =( CBuffer &buffer);

	const char *c_str(){ return _buf;}
	int length(){ return _len;}
private:
	char* _buf;
	int _len;
};



