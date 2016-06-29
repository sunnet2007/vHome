#ifndef _CALC_FILE_CID_H
#define _CALC_FILE_CID_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class CBlockCID
{
public:
	CBlockCID()
	{
		fileSize = -1;
		blockSn = -1;
		blockOffset = -1;
		blockSize = -1;
	}

	string fileName;
	int fileSize;
	string fileCID;
	int blockSn;
	int blockOffset;
	int blockSize;
	string blockCID;
};

int CalcFileCID(string filePath, string & fileCID, int & fileSize);

int CalcFileCID(string filePath, int offset, int length, string & fileCID, int & fileSize);

int CalcFileCIDMultyPart(string filePath, vector<CBlockCID> & blockCIDList);

int CalcNeededBlockSN(int fileSize, int offset, int length, int & startSN, int & endSN);

//int CalcFileBlockCID(string filePath, int blockSN, CBlockCID & blockCID);

int GetOffsetAndLengthFromCID(string cid, int & fileSize, int & offset, int & length);

#endif
