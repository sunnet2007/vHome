#include "StdAfx.h"
#include "MemoryManage.h"
#include <assert.h>


#define  MEMORY_BLOCK_SIZE 1024 
CMemoryManager * CMemoryManager::m_MemoryManager = NULL;

CMemoryManager * CMemoryManager::Instance()
{
	if ( !m_MemoryManager )
	{
		m_MemoryManager = new CMemoryManager();
	}
	return m_MemoryManager;
}

void CMemoryManager::Release()
{
	if ( m_MemoryManager )
	{
		delete m_MemoryManager;
		m_MemoryManager = NULL;
	}
}

int CMemoryManager::GetRealAllocSize( int iSize )
{
	iSize = ( iSize/MEMORY_BLOCK_SIZE + 1 )*MEMORY_BLOCK_SIZE;
	return iSize;
}

char * CMemoryManager::GetMemory( int iSize )
{
	char * ptr = NULL;
	std::map< int , MemoryList >::iterator itMap;
	iSize = GetRealAllocSize( iSize );

	
	CAutoLock lock( m_lock );
	itMap = m_MemoryBlockManager.find( iSize );
	if ( itMap == m_MemoryBlockManager.end() )
	{
		MemoryList lst;
		lst.m_iBlockSize = iSize;
		m_MemoryBlockManager.insert( std::make_pair( iSize , lst ) );
		itMap = m_MemoryBlockManager.find( iSize );
	}
	if ( itMap == m_MemoryBlockManager.end() )
	{
		return NULL;
	}

	MemoryList &lst = itMap->second;


	if ( !lst.m_listFreeMemory.empty() )
	{
		ptr = lst.m_listFreeMemory.front();
		lst.m_listFreeMemory.erase( lst.m_listFreeMemory.begin() );	
	}
	else
	{
		try
		{
			ptr = new char[ iSize ];
		}
		catch (...)
		{
			assert( FALSE );
		}
	}

	if ( ptr )
	{
		memset( ptr , 0 , iSize );
		m_MapMemoryBlock.insert( std::make_pair( ptr , iSize ) );
	}

	return ptr;
		
}

void CMemoryManager::FreeMemory( char * memory,bool bFreeIfNotfind  )
{
	std::map< int , MemoryList >::iterator itMap;
	std::map< char * , int >::iterator itMapPtr;
	int iSize = 0;

	
	CAutoLock lock( m_lock );
	itMapPtr = m_MapMemoryBlock.find( memory );
	if ( itMapPtr != m_MapMemoryBlock.end() )
	{
		iSize = itMapPtr->second;

		itMap = m_MemoryBlockManager.find( iSize );

		if ( itMap != m_MemoryBlockManager.end() )
		{
			MemoryList & lst = itMap->second;

			if ( lst.m_listFreeMemory.size() > lst.m_iMaxCount )
			{
				delete [] memory;
			}
			else
			{
				ZeroMemory( memory , iSize );
				lst.m_listFreeMemory.push_back( memory );
			}
			
		}
		m_MapMemoryBlock.erase( itMapPtr );
	}
	else
	{
		if ( bFreeIfNotfind )
		{
			if ( memory )
			{
				delete [] memory;
			}	
		}
	}
}

CMemoryManager::CMemoryManager()
{
}

CMemoryManager::~CMemoryManager()
{
	std::map< int , MemoryList >::iterator itMap;
	std::map< int , MemoryList >::iterator itMapBegin;
	std::map< int , MemoryList >::iterator itMapEnd;

	std::map< char * , int >::iterator itMapPtr;
	std::map< char * , int >::iterator itMapPtrBegin;
	std::map< char * , int >::iterator itMapPtrEnd;

	
	CAutoLock lock( m_lock );
	itMapBegin = m_MemoryBlockManager.begin();
	itMapEnd   = m_MemoryBlockManager.end();

	for ( itMap = itMapBegin; itMap != itMapEnd; itMap++ )
	{
		MemoryList &lst = itMap->second;

		int iSize = lst.m_listFreeMemory.size();
		for ( int iIndex = 0; iIndex < iSize; iIndex++ )
		{
			if (  lst.m_listFreeMemory[ iIndex ] )
			{
				delete [] lst.m_listFreeMemory[ iIndex ];
			}			
		}
		lst.m_listFreeMemory.clear();
	}

	itMapPtrBegin = m_MapMemoryBlock.begin();
	itMapPtrEnd   = m_MapMemoryBlock.end();
	for ( itMapPtr = itMapPtrBegin; itMapPtr != itMapPtrEnd; itMapPtr++ )
	{
		if ( itMapPtr->first)
		{
			delete [] itMapPtr->first;
		}		
	}
	m_MemoryBlockManager.clear();
}
