#if (defined(_WIN32) || defined(_WIN64))

/*+++ 
HDID.CPP 
Written by Lu Lin 
http://lu0.126.com 
2000.11.3 
---*/

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include <ObjBase.h>
#include "md5.h"
# pragma comment(lib, "wbemuuid.lib")

#include <windows.h> 
#include <iostream> 
#include <stdio.h>
#include <time.h>
using namespace std;

#define DFP_GET_VERSION 0x00074080 
#define DFP_SEND_DRIVE_COMMAND 0x0007c084 
#define DFP_RECEIVE_DRIVE_DATA 0x0007c088 

#pragma pack(1) 
typedef struct _GETVERSIONOUTPARAMS { 
	BYTE bVersion;  // Binary driver version. 
	BYTE bRevision;  // Binary driver revision. 
	BYTE bReserved;  // Not used. 
	BYTE bIDEDeviceMap; // Bit map of IDE devices. 
	DWORD fCapabilities; // Bit mask of driver capabilities. 
	DWORD dwReserved[4]; // For future use. 
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS; 

#if 1 //(_MSC_VER <= 1400)

typedef struct _IDEREGS_2010 { 
	BYTE bFeaturesReg;  // Used for specifying SMART "commands". 
	BYTE bSectorCountReg; // IDE sector count register 
	BYTE bSectorNumberReg; // IDE sector number register 
	BYTE bCylLowReg;   // IDE low order cylinder value 
	BYTE bCylHighReg;  // IDE high order cylinder value 
	BYTE bDriveHeadReg;  // IDE drive/head register 
	BYTE bCommandReg;  // Actual IDE command. 
	BYTE bReserved;   // reserved for future use.  Must be zero. 
} IDEREGS_2010, *PIDEREGS_2010, *LPIDEREGS_2010; 

typedef struct _SENDCMDINPARAMS_2010 { 
	DWORD cBufferSize;  // Buffer size in bytes 
	IDEREGS_2010 irDriveRegs;  // Structure with drive register values. 
	BYTE bDriveNumber;  // Physical drive number to send 
	// command to (0,1,2,3). 
	BYTE bReserved[3];  // Reserved for future expansion. 
	DWORD dwReserved[4];  // For future use. 
	//BYTE  bBuffer[1];   // Input buffer. 
} SENDCMDINPARAMS_2010, *PSENDCMDINPARAMS_2010, *LPSENDCMDINPARAMS_2010; 

typedef struct _DRIVERSTATUS_2010 { 
	BYTE bDriverError;  // Error code from driver, 
	// or 0 if no error. 
	BYTE bIDEStatus;   // Contents of IDE Error register. 
	// Only valid when bDriverError 
	// is SMART_IDE_ERROR. 
	BYTE bReserved[2];  // Reserved for future expansion. 
	DWORD dwReserved[2];  // Reserved for future expansion. 
} DRIVERSTATUS_2010, *PDRIVERSTATUS_2010, *LPDRIVERSTATUS_2010; 

typedef struct _SENDCMDOUTPARAMS_2010 { 
	DWORD    cBufferSize;  // Size of bBuffer in bytes 
	DRIVERSTATUS DriverStatus;  // Driver status structure. 
	BYTE   bBuffer[512];   // Buffer of arbitrary length 
	// in which to store the data read from the drive. 
} SENDCMDOUTPARAMS_2010, *PSENDCMDOUTPARAMS_2010, *LPSENDCMDOUTPARAMS_2010; 

#endif

typedef struct _IDSECTOR { 
	USHORT wGenConfig; 
	USHORT wNumCyls; 
	USHORT wReserved; 
	USHORT wNumHeads; 
	USHORT wBytesPerTrack; 
	USHORT wBytesPerSector; 
	USHORT wSectorsPerTrack; 
	USHORT wVendorUnique[3]; 
	CHAR sSerialNumber[20]; 
	USHORT wBufferType; 
	USHORT wBufferSize; 
	USHORT wECCSize; 
	CHAR sFirmwareRev[8]; 
	CHAR sModelNumber[40]; 
	USHORT wMoreVendorUnique; 
	USHORT wDoubleWordIO; 
	USHORT wCapabilities; 
	USHORT wReserved1; 
	USHORT wPIOTiming; 
	USHORT wDMATiming; 
	USHORT wBS; 
	USHORT wNumCurrentCyls; 
	USHORT wNumCurrentHeads; 
	USHORT wNumCurrentSectorsPerTrack; 
	ULONG ulCurrentSectorCapacity; 
	USHORT wMultSectorStuff; 
	ULONG ulTotalAddressableSectors; 
	USHORT wSingleWordDMA; 
	USHORT wMultiWordDMA; 
	BYTE bReserved[128]; 
} IDSECTOR, *PIDSECTOR; 

/*+++ 
Global vars 
---*/ 
//GETVERSIONOUTPARAMS vers; 
//SENDCMDINPARAMS_2010 in; 
//SENDCMDOUTPARAMS_2010 out; 
//HANDLE h; 
//DWORD i; 
//BYTE j;

void CopyRight(){ 
	cerr<<endl<<"HDD identifier v1.0 for WIN95/98/Me/NT/2000. written by Lu Lin"<<endl; 
	cerr<<"For more information, please visit Inside Programming: http://lu0.126.com"<<endl; 
	cerr<<"2000.11.3"<<endl<<endl; 
} 
VOID ChangeByteOrder(PCHAR szString, USHORT uscStrSize) 
{ 

	int i; 
	CHAR temp; 

	for (i = 0; i < uscStrSize; i+=2) 
	{ 
		temp = szString[i]; 
		szString[i] = szString[i+1]; 
		szString[i+1] = temp; 
	} 
} 

void DetectIDE(BYTE bIDEDeviceMap){ 
	if (bIDEDeviceMap&1){ 
		if (bIDEDeviceMap&16){ 
			cout<<"ATAPI device is attached to primary controller, drive 0."<<endl; 
		}else{ 
			cout<<"IDE device is attached to primary controller, drive 0."<<endl; 
		} 
	} 
	if (bIDEDeviceMap&2){ 
		if (bIDEDeviceMap&32){ 
			cout<<"ATAPI device is attached to primary controller, drive 1."<<endl; 
		}else{ 
			cout<<"IDE device is attached to primary controller, drive 1."<<endl; 
		} 
	} 
	if (bIDEDeviceMap&4){ 
		if (bIDEDeviceMap&64){ 
			cout<<"ATAPI device is attached to secondary controller, drive 0."<<endl; 
		}else{ 
			cout<<"IDE device is attached to secondary controller, drive 0."<<endl; 
		} 
	} 
	if (bIDEDeviceMap&8){ 
		if (bIDEDeviceMap&128){ 
			cout<<"ATAPI device is attached to secondary controller, drive 1."<<endl; 
		}else{ 
			cout<<"IDE device is attached to secondary controller, drive 1."<<endl; 
		} 
	} 
} 

int hdid9x(char * sn){
	if(sn == NULL)
		return -1;

	GETVERSIONOUTPARAMS vers;
	SENDCMDINPARAMS_2010 in;
	SENDCMDOUTPARAMS_2010 out;
	HANDLE h;
	DWORD i;

	ZeroMemory(&vers,sizeof(vers)); 
	//We start in 95/98/Me 
	h=CreateFileA("\\\\.\\Smartvsd",0,0,0,CREATE_NEW,0,0); 
	if (!h){ 
		cout<<"open smartvsd.vxd failed"<<endl; 
		//exit(0);
		return -1;
	} 

	if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0)){ 
		cout<<"DeviceIoControl failed:DFP_GET_VERSION"<<endl; 
		CloseHandle(h); 
		return -1; 
	} 
	//If IDE identify command not supported, fails 
	if (!(vers.fCapabilities&1)){ 
		cout<<"Error: IDE identify command not supported."; 
		CloseHandle(h); 
		return -1; 
	} 
	//Display IDE drive number detected 
	DetectIDE(vers.bIDEDeviceMap); 
	//Identify the IDE drives 
	int j;
	for (j=0;j<4;j++){ 
		PIDSECTOR phdinfo; 
		char s[41]; 

		ZeroMemory(&in,sizeof(in)); 
		ZeroMemory(&out,sizeof(out)); 
		if (j&1){ 
			in.irDriveRegs.bDriveHeadReg=0xb0; 
		}else{ 
			in.irDriveRegs.bDriveHeadReg=0xa0; 
		} 
		if (vers.fCapabilities&(16>>j)){ 
			//We don't detect a ATAPI device. 
			cout<<"Drive "<<(int)(j+1)<<" is a ATAPI device, we don't detect it"<<endl; 
			continue; 
		}else{ 
			in.irDriveRegs.bCommandReg=0xec; 
		} 
		in.bDriveNumber=j; 
		in.irDriveRegs.bSectorCountReg=1; 
		in.irDriveRegs.bSectorNumberReg=1; 
		in.cBufferSize=512; 
		if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0)){ 
			cout<<"DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA"<<endl; 
			CloseHandle(h); 
			return -1; 
		} 
		phdinfo=(PIDSECTOR)out.bBuffer; 
		memcpy(s,phdinfo->sModelNumber,40); 
		s[40]=0; 
		ChangeByteOrder(s,40); 
		//cout<<"Module Number:"<<s<<endl; 
		memcpy(s,phdinfo->sFirmwareRev,8); 
		s[8]=0; 
		ChangeByteOrder(s,8); 
		//cout<<"\tFirmware rev:"<<s<<endl; 
		memcpy(s,phdinfo->sSerialNumber,20); 
		s[20]=0; 
		ChangeByteOrder(s,20); 
		//cout<<"Serial Number:"<<s<<endl; 
		strcpy(sn, s);
		//cout<<"\tCapacity:"<<phdinfo->ulTotalAddressableSectors/2/1024<<"M"<<endl<<endl; 
	} 

	//Close handle before quit 
	CloseHandle(h); 
	//CopyRight(); 

	return 0;
} 

int hdidnt(char * sn){
	if(sn == NULL)
		return -1;

	GETVERSIONOUTPARAMS vers;
	SENDCMDINPARAMS_2010 in;
	SENDCMDOUTPARAMS_2010 out;
	HANDLE h;
	DWORD i;

	char hd[80]; 
	PIDSECTOR phdinfo; 
	char s[41]; 

	ZeroMemory(&vers,sizeof(vers)); 
	//We start in NT/Win2000 
	int j;
	for (j=0;j<4;j++){ 
		sprintf(hd,"\\\\.\\PhysicalDrive%d",j); 
		h=CreateFileA(hd,GENERIC_READ|GENERIC_WRITE, 
			FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0); 
		if (!h){ 
			continue; 
		} 
		if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0)){ 
			CloseHandle(h); 
			continue; 
		} 
		//If IDE identify command not supported, fails 
		if (!(vers.fCapabilities&1)){ 
			cout<<"Error: IDE identify command not supported."; 
			CloseHandle(h); 
			return -1; 
		} 
		//Identify the IDE drives 
		ZeroMemory(&in,sizeof(in)); 
		ZeroMemory(&out,sizeof(out)); 
		if (j&1){ 
			in.irDriveRegs.bDriveHeadReg=0xb0; 
		}else{ 
			in.irDriveRegs.bDriveHeadReg=0xa0; 
		} 
		if (vers.fCapabilities&(16>>j)){ 
			//We don't detect a ATAPI device. 
			cout<<"Drive "<<(int)(j+1)<<" is a ATAPI device, we don't detect it"<<endl; 
			continue; 
		}else{ 
			in.irDriveRegs.bCommandReg=0xec; 
		} 
		in.bDriveNumber=j; 
		in.irDriveRegs.bSectorCountReg=1; 
		in.irDriveRegs.bSectorNumberReg=1; 
		in.cBufferSize=512; 
		if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0)){ 
			cout<<"DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA"<<endl; 
			CloseHandle(h); 
			return -1; 
		} 
		phdinfo=(PIDSECTOR)out.bBuffer; 
		memcpy(s,phdinfo->sModelNumber,40); 
		s[40]=0; 
		ChangeByteOrder(s,40); 
		//cout<<"Module Number: "<<s<<endl; 
		memcpy(s,phdinfo->sFirmwareRev,8); 
		s[8]=0; 
		ChangeByteOrder(s,8); 
		//cout<<"Firmware rev: "<<s<<endl; 
		memcpy(s,phdinfo->sSerialNumber,20); 
		s[20]=0; 
		ChangeByteOrder(s,20); 
		//cout<<"Serial Number: "<<s<<endl; 
		strcpy(sn, s);
		//cout<<"Capacity: "<<phdinfo->ulTotalAddressableSectors/2/1024<<"M"<<endl<<endl; 
		CloseHandle(h); 
	} 
	//CopyRight(); 

	return 0;
} 

string GetPcID(const string & hardwareSN)
{
	/*
	if(hardwareSN.size() > 0)
	{
		string s = hardwareSN + "F075D1C538EEEDF99FD2829FA6595FD44E877D6B";
		return md5(s);
	}
	else
	{
		char r[20] = { 0 };
		itoa(time(NULL), r, 10);
		string s = string(r) + "F075D1C538EEEDF99FD2829FA6595FD44E877D6B";
		s = md5(s);
		s[0] = 'R';
		s[1] = 'X';

		return s;
	}
	*/

	string s = hardwareSN + "F075D1C538EEEDF99FD2829FA6595FD44E877D6B";
	return md5(s);
}

int GetHardwareSNumber(char * pszNumber)
{
	if(pszNumber == NULL)
		return -1;

	OSVERSIONINFO VersionInfo; 

	ZeroMemory(&VersionInfo,sizeof(VersionInfo)); 
	VersionInfo.dwOSVersionInfoSize=sizeof(VersionInfo); 
	GetVersionEx(&VersionInfo); 

	int ret = -1;

	switch (VersionInfo.dwPlatformId){ 
	case VER_PLATFORM_WIN32s: 
		//cout<<"Win32s is not supported by this programm."<<endl;
		ret = -1;
		break;
	case VER_PLATFORM_WIN32_WINDOWS: 
		ret = hdid9x(pszNumber); 
		break; 
	case VER_PLATFORM_WIN32_NT: 
		ret = hdidnt(pszNumber);
		break;
	default:
		ret = -1;
	}

	return ret;
} 

#endif
