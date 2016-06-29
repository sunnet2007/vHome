#pragma once
#include <Windows.h>

#define BASE_ERROR               ( -1 ) // �ļ���βλ�� 010 000

// �����ļ��򿪷�ʽ�ĺ�
#define BASE_READ           ( 0x00000001 ) // ֻ�������û���ҵ�����û�д򿪣��򿪽���ʧ��
#define BASE_WRITE          ( 0x00000002 ) // ��ֻд�ķ�ʽ�򿪣����ͬ���ļ����ڣ�ԭ�������ݶ�ʧ
#define BASE_APP            ( 0x00000004 )
#define BASE_CREATE         ( 0x00000008 ) // �Զ����Ʒ�ʽ���ļ�
#define BASE_BINARY         ( 0x00000010 ) // ���ı���ʽ���ļ�
#define BASE_TEXT           ( 0x00000020 )


class  CBaseFile
{
public:
	CBaseFile( void );
	~CBaseFile( void );
	/*
	���ܣ����ļ�
	������lpszFileName ---�ļ���ָ��
			nMode        ---�ļ��򿪷�ʽ
	���أ��򿪳ɹ�����true��ʧ�ܷ���false
	��ע��
	*/
	virtual bool Open( LPCTSTR lpszFileName, int nMode );
	/*
	���ܣ��ر��ļ�
	������
	���أ��رճɹ�����true�����򷵻�false 
	��ע��
	*/
	virtual bool Close( void );
	/*
	���ܣ���ȡ����
	������pBuf ---������
			nLen ---�������Ĵ�С
	���أ���ȡ���ֽ���
	��ע��
	*/
 	virtual int Read( void * pBuf, int nLen );
	/*
	���ܣ����ļ���д����
	������pBuf ---������
			nLen ---�������Ĵ�С
	���أ�ʵ��д�뻺�������ֽ���,д������������󷵻�-1
	��ע��
	*/
	virtual int Write( void * pBuf, int nLen );
	/*
	���ܣ������ļ�ָ��
	������nPosFlag ---λ�ñ��
			nOffset  ---����ļ���ʼλ�õ�ƫ����
	���أ����óɹ�����true�����򷵻�false
	��ע��
	*/
	virtual bool Seek( int nPosFlag, __int64 nOffset );
	/*
	���ܣ��ж��ļ��Ƿ��Ѿ���
	������
	���أ��򿪷���true���رշ���false
	��ע��
	*/
	virtual bool IsOpen( void );
	/*
	���ܣ���ȡ�ļ���С
	������
	���أ��ļ���ʵ�ʴ�С
	��ע��
	*/
	virtual __int64 GetFileSize( void );
	/*
	���ܣ������ļ���С
	������nFileSize ---�ļ���С
	���أ����óɹ�����true��ʧ�ܷ���false
	��ע��
	*/
	virtual bool SetFileSize( __int64 nFileSize );
	
	virtual bool IsEof();

protected:
	bool m_bOpen;  // ��־�ļ��Ƿ��
	FILE *m_pFile; // �ļ�ָ��
	
};
