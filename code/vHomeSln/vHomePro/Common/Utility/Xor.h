#ifndef _XOR_ENCRYPT_H_20120331
#define _XOR_ENCRYPT_H_20120331
/*************************************************************
文件：xor.h
描述：使用异或的方式对文件进行简单加密，并能对文件进行解密
作者：寮岩
创建日期：2012-03-31
**************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include "Md5.h"

int Xor(string fn, string hash);

int Xor(char * data, int len, string hash, int offset = 0);

#endif
