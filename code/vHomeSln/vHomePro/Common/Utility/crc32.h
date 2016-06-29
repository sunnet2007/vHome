
#pragma once


/***************************************************************************
* Description: GetCRC32Table�������crc������
***************************************************************************/

void  GetCRC32Table();


void  GetKeyTable();

/***************************************************************************
* Description: CreateCrc����������������ݴ���CRC-32У����
* Arguments  : DataBuff     ָ�����ݴ���ָ��
*              BufLen       ���ݴ��ĳ���


* Returns    : �������ݴ���CRC-32У����
***************************************************************************/

unsigned long  GetCrc32( char * pDataBuf, int nBufLen );

void  SetCRCKey( int nKey );

void  EncryptData( char * pDataBuf, int nBufLen );

void  DecryptData( char * pDataBuf, int nBufLen );

