#pragma once
#include <map>
#include <vector>
#include "../Utility/Lock.h"


#define MEMORY_BLOCK_MAX_COUNT 20



typedef struct _MemoryList
{
	std::vector< char * > m_listFreeMemory;
	int m_iBlockSize;
	int m_iMaxCount;
	_MemoryList()
	{
		m_iMaxCount = MEMORY_BLOCK_MAX_COUNT;
		m_iBlockSize = 4;
	}
}MemoryList;


class   CMemoryManager
{
public:
	static CMemoryManager * Instance();
	void Release();
	char * GetMemory( int iSize );
	void FreeMemory( char * memory,bool bFreeIfNotfind = true );
	int GetRealAllocSize( int iSize );
protected:
	CMemoryManager();
	~CMemoryManager();

protected:
	std::map< int , MemoryList > m_MemoryBlockManager;
	std::map< char * , int > m_MapMemoryBlock;
	CLock m_lock;

	static CMemoryManager * m_MemoryManager;
};
