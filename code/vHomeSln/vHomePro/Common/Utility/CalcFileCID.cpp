#include "CalcFileCID.h"
#include "sha1.h"

int CalcFileCID(string filePath, string & fileCID, int & fileSize)
{
	fileCID = "";
	fileSize = -1;

	FILE * fp;
	fp = fopen(filePath.c_str(), "rb");
	if(fp == NULL)
		return -1;

	unsigned char data_block[60*1024] = { 0 };
	int data_len = sizeof(data_block);
	unsigned char out[100] = { 0 };

	fseek(fp, 0L, SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);
	if(fileSize > 60*1024)
	{
		fread(data_block, 20*1024, 1, fp);
		fseek(fp, fileSize/3, SEEK_SET);
		fread(data_block+20*1024, 20*1024, 1, fp);
		fseek(fp, -20*1024, SEEK_END);
		fread(data_block+40*1024, 20*1024, 1, fp);
		data_len = 60*1024;
	}
	else
	{
		fread(data_block, fileSize, 1, fp);
		data_len = fileSize;
	}
	fclose(fp);

	ctx_sha1 hash;
	hash.update(data_block, data_len);
	hash.finish(out);

	char tmp[100] = { 0 };
	int j;
	for(j = 0; j < 20; j++)
	{
		sprintf(tmp, "%02X", (unsigned char)out[j]);
		fileCID += tmp;
	}

	sprintf(tmp, "%010X%010X%010X", fileSize, 0, fileSize);
	fileCID += tmp;

	return 0;
}

int CalcFileCID(string filePath, int offset, int length, string & fileCID, int & fileSize)
{
	fileCID = "";
	fileSize = -1;

	FILE * fp;
	fp = fopen(filePath.c_str(), "rb");
	if(fp == NULL)
		return -1;

	unsigned char data_block[60*1024] = { 0 };
	int data_len = sizeof(data_block);
	unsigned char out[100] = { 0 };

	fseek(fp, 0L, SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);

	if(offset < 0 || offset >= fileSize)
	{
		fclose(fp);
		return -1;
	}

	if(length < 0)
		length = fileSize-offset;

	if(length > 60*1024)
	{
		fseek(fp, offset, SEEK_SET);
		fread(data_block, 20*1024, 1, fp);
		fseek(fp, offset+length/3, SEEK_SET);
		fread(data_block+20*1024, 20*1024, 1, fp);
		fseek(fp, offset+length-20*1024, SEEK_SET);
		fread(data_block+40*1024, 20*1024, 1, fp);
		data_len = 60*1024;
	}
	else
	{
		fseek(fp, offset, SEEK_SET);
		fread(data_block, length, 1, fp);
		data_len = length;
	}
	fclose(fp);

	ctx_sha1 hash;
	hash.update(data_block, data_len);
	hash.finish(out);

	char tmp[100] = { 0 };
	int j;
	for(j = 0; j < 20; j++)
	{
		sprintf(tmp, "%02X", (unsigned char)out[j]);
		fileCID += tmp;
	}

	sprintf(tmp, "%010X%010X%010X", fileSize, offset, length);
	fileCID += tmp;

	return 0;
}

int CalcFileCIDMultyPart(string filePath, vector<CBlockCID> & blockCIDList)
{
	blockCIDList.clear();

	FILE * fp;
	fp = fopen(filePath.c_str(), "rb");
	if(fp == NULL)
		return -1;

	unsigned char data_block[60*1024] = { 0 };
	int data_len = sizeof(data_block);
	unsigned char out[100] = { 0 };

	fseek(fp, 0L, SEEK_END);
	int fileSize = ftell(fp);
	rewind(fp);

	if(fileSize > 60*1024)
	{
		fread(data_block, 20*1024, 1, fp);
		fseek(fp, fileSize/3, SEEK_SET);
		fread(data_block+20*1024, 20*1024, 1, fp);
		fseek(fp, -20*1024, SEEK_END);
		fread(data_block+40*1024, 20*1024, 1, fp);
		data_len = 60*1024;
	}
	else
	{
		fread(data_block, fileSize, 1, fp);
		data_len = fileSize;
	}

	ctx_sha1 hash;
	hash.update(data_block, data_len);
	hash.finish(out);

	CBlockCID blockCID;
	blockCID.fileName = filePath;
	blockCID.fileSize = fileSize;
	blockCID.blockSn = -1;
	blockCID.blockOffset = 0;
	blockCID.blockSize = fileSize;

	char tmp[100] = { 0 };
	int j;
	for(j = 0; j < 20; j++)
	{
		sprintf(tmp, "%02X", (unsigned char)out[j]);
		blockCID.blockCID += tmp;
	}

	sprintf(tmp, "%010X%010X%010X", fileSize, 0, fileSize);
	blockCID.blockCID += tmp;

	//string fileCID = blockCID.fileCID;
	blockCID.fileCID = blockCID.blockCID;
	blockCIDList.push_back(blockCID);

	int BLOCK = 10*1024*1024;
	int BLOCK_NUM = 10;
	int start, block, blockNum;

	if(fileSize <= BLOCK)
	{
		fclose(fp);
		return 0;
	}

	if(fileSize%BLOCK == 0)
	{
		blockNum = fileSize/BLOCK;
	}
	else
	{
		blockNum = fileSize/BLOCK + 1;
	}

	if(blockNum > BLOCK_NUM)
		blockNum = BLOCK_NUM;

	if(fileSize%blockNum == 0)
	{
		block = fileSize/blockNum;
	}
	else
	{
		block = fileSize/blockNum + 1;
	}

	for(start = 0; start < fileSize; start += block)
	{
		int len;
		if(fileSize - start >= block)
		{
			len = block;
		}
		else
		{
			len = fileSize - start;
		}

		if(len > 60*1024)
		{
			fseek(fp, start, SEEK_SET);
			fread(data_block, 20*1024, 1, fp);
			fseek(fp, start+len/3, SEEK_SET);
			fread(data_block+20*1024, 20*1024, 1, fp);
			fseek(fp, start+len-20*1024, SEEK_SET);
			fread(data_block+40*1024, 20*1024, 1, fp);
			data_len = 60*1024;
		}
		else
		{
			fseek(fp, start, SEEK_SET);
			fread(data_block, len, 1, fp);
			data_len = len;
		}

		ctx_sha1 hash;
		hash.update(data_block, data_len);
		hash.finish(out);

		blockCID.blockSn++;
		blockCID.blockOffset = start;
		blockCID.blockSize = len;
		blockCID.blockCID = "";

		for(j = 0; j < 20; j++)
		{
			sprintf(tmp, "%02X", (unsigned char)out[j]);
			blockCID.blockCID += tmp;
		}

		sprintf(tmp, "%010X%010X%010X", fileSize, start, len);
		blockCID.blockCID += tmp;

		blockCIDList.push_back(blockCID);
	}

	fclose(fp);

	return 0;
}
/*
int CalcFileBlockCID(string filePath, int blockSN, CBlockCID & blockCID)
{
	blockCID = CBlockCID();

	if(blockSN < -1)
		return -1;

	FILE * fp;
	fp = fopen(filePath.c_str(), "rb");
	if(fp == NULL)
		return -1;

	unsigned char data_block[60*1024] = { 0 };
	int data_len = sizeof(data_block);
	unsigned char out[100] = { 0 };

	fseek(fp, 0L, SEEK_END);
	int fileSize = ftell(fp);
	rewind(fp);

	int BLOCK = 10*1024*1024;
	int BLOCK_NUM = 10;
	int start, block, blockNum;
	int j;
	char tmp[100] = { 0 };

	if(blockSN == -1 || fileSize <= BLOCK)
	{
		if(fileSize > 60*1024)
		{
			fread(data_block, 20*1024, 1, fp);
			fseek(fp, fileSize/3, SEEK_SET);
			fread(data_block+20*1024, 20*1024, 1, fp);
			fseek(fp, -20*1024, SEEK_END);
			fread(data_block+40*1024, 20*1024, 1, fp);
			data_len = 60*1024;
		}
		else
		{
			fread(data_block, fileSize, 1, fp);
			data_len = fileSize;
		}

		ctx_sha1 hash;
		hash.update(data_block, data_len);
		hash.finish(out);

		blockCID.fileName = filePath;
		blockCID.fileSize = fileSize;
		blockCID.blockSn = -1;
		blockCID.blockOffset = 0;
		blockCID.blockSize = fileSize;

		for(j = 0; j < 20; j++)
		{
			sprintf(tmp, "%02X", (unsigned char)out[j]);
			blockCID.blockCID += tmp;
		}

		sprintf(tmp, "%010X%010X", 0, fileSize);
		blockCID.blockCID += tmp;

		blockCID.fileCID = blockCID.blockCID;
		fclose(fp);
		return 0;
	}


	if(fileSize%BLOCK == 0)
	{
		blockNum = fileSize/BLOCK;
	}
	else
	{
		blockNum = fileSize/BLOCK + 1;
	}

	if(blockNum > BLOCK_NUM)
		blockNum = BLOCK_NUM;

	if(fileSize%blockNum == 0)
	{
		block = fileSize/blockNum;
	}
	else
	{
		block = fileSize/blockNum + 1;
	}

	start = block*blockSN;
	if(start >= fileSize)
	{
		fclose(fp);
		return -1;
	}

	int len;
	if(fileSize - start >= block)
	{
		len = block;
	}
	else
	{
		len = fileSize - start;
	}

	if(len > 60*1024)
	{
		fseek(fp, start, SEEK_SET);
		fread(data_block, 20*1024, 1, fp);
		fseek(fp, start+len/3, SEEK_SET);
		fread(data_block+20*1024, 20*1024, 1, fp);
		fseek(fp, start+len-20*1024, SEEK_SET);
		fread(data_block+40*1024, 20*1024, 1, fp);
		data_len = 60*1024;
	}
	else
	{
		fseek(fp, start, SEEK_SET);
		fread(data_block, len, 1, fp);
		data_len = len;
	}

	ctx_sha1 hash;
	hash.update(data_block, data_len);
	hash.finish(out);

	blockCID.blockSn = blockSN;
	blockCID.blockOffset = start;
	blockCID.blockSize = len;
	blockCID.blockCID = "";

	for(j = 0; j < 20; j++)
	{
		sprintf(tmp, "%02X", (unsigned char)out[j]);
		blockCID.blockCID += tmp;
	}

	sprintf(tmp, "%010X%010X", start, len);
	blockCID.blockCID += tmp;

	fclose(fp);

	return 0;
}
*/
int CalcNeededBlockSN(int fileSize, int offset, int length, int & startSN, int & endSN)
{
	startSN = -1;
	endSN = -2;

	int BLOCK = 10*1024*1024;
	int BLOCK_NUM = 10;
	int start, block, blockNum;

	if(fileSize <= BLOCK)
	{
		startSN = -1;
		endSN = -1;
		return 0;
	}

	if(fileSize%BLOCK == 0)
	{
		blockNum = fileSize/BLOCK;
	}
	else
	{
		blockNum = fileSize/BLOCK + 1;
	}

	if(blockNum > BLOCK_NUM)
		blockNum = BLOCK_NUM;

	if(fileSize%blockNum == 0)
	{
		block = fileSize/blockNum;
	}
	else
	{
		block = fileSize/blockNum + 1;
	}

	startSN = offset/block;
	endSN = (offset+length-1)/block;

	return 0;
}

int GetOffsetAndLengthFromCID(string cid, int & fileSize, int & offset, int & length)
{
	offset = -1;
	length = -1;

	if(cid.size() != 70)
		return -1;

	string str;
	str = cid.substr(40, 10);
	sscanf(str.c_str(), "%X", &fileSize);

	str = cid.substr(50, 10);
	sscanf(str.c_str(), "%X", &offset);

	str = cid.substr(60, 10);
	sscanf(str.c_str(), "%X", &length);

	return 0;
}