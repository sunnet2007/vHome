
#pragma once


/***************************************************************************
* Description: GetCRC32Table函数获得crc余数表
***************************************************************************/

void  GetCRC32Table();


void  GetKeyTable();

/***************************************************************************
* Description: CreateCrc函数计算出给定数据串的CRC-32校验码
* Arguments  : DataBuff     指向数据串的指针
*              BufLen       数据串的长度


* Returns    : 给定数据串的CRC-32校验码
***************************************************************************/

unsigned long  GetCrc32( char * pDataBuf, int nBufLen );

void  SetCRCKey( int nKey );

void  EncryptData( char * pDataBuf, int nBufLen );

void  DecryptData( char * pDataBuf, int nBufLen );

