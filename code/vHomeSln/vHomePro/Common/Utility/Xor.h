#ifndef _XOR_ENCRYPT_H_20120331
#define _XOR_ENCRYPT_H_20120331
/*************************************************************
�ļ���xor.h
������ʹ�����ķ�ʽ���ļ����м򵥼��ܣ����ܶ��ļ����н���
���ߣ����
�������ڣ�2012-03-31
**************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include "Md5.h"

int Xor(string fn, string hash);

int Xor(char * data, int len, string hash, int offset = 0);

#endif
