#pragma once

#define MAX_BUF_SIZE_SOCK	10240
#define MAX_BUF_SIZE	( 65536 * 10 )
#define MAX_BUF_SIZE_FOR_CHILD	(65536 * 3)
#define MAX_PACKET_SIZE				65536
#define MAX_DATA_SIZE	(MAX_BUF_SIZE - sizeof(StMissionHeader))
#define MAX_CLIENT		64
#define MAX_OPCODE		1024
#define COMM_TIMEOUT	1000

#define INTER_COMM_CODE (0xffffffff)
#define LINK_RETRY_CODE	(0xfffffffe)

#define TCP_SERVER_ERROR_SOCKET		(-1)
#define TCP_SERVER_ERROR_BIND		(-2)
#define TCP_SERVER_ERROR_LISTEN		(-3)

#define TCP_CLIENT_ERROR_SOCKET		(-1)
#define TCP_CLIENT_ERROR_CONNECT	(-2)

#define UDP_SERVER_ERROR_SOCKET		(-1)
#define UDP_SERVER_ERROR_BIND		(-2)
#define UDP_SERVER_ERROR_LISTEN		(-3)

#define UDP_CLIENT_ERROR_SOCKET		(-1)
#define UDP_CLIENT_ERROR_CONNECT	(-2)

#define TCP_SEND_ERROR_SOCKET		(-1)
#define TCP_SEND_ERROR_SEND			(-2)

#define MAX_CLIENT_CONNECT_CNT	20

#define MAX_SERVER_CONNECTION_CNT	12

#define DATA_LINK_CONNECT_PORT	7777
#define INTER_COMM_CONNECT_PORT	6001
#define DATA_LINK_CONNECT_START_PORT	7700

#define LINK_FRAME_SIZE	65536

#define START_FRAME_INDEX (1)
#define START_LINK_SEQ_INDEX (1)

#define LINK1_ID				(0)		//수신국1
#define LINK2_ID				(1)		//수신국2
#define LINK3_ID				(2)		//수신국3

#define DEVICECODE_TGD					(0xA0)	//수신국 GPS방탐 장비
#define DEVICECODE_TVU					(0x90)	//수신국 V/UHF 장비
#define DEVICECODE_TDF					(0x88)	//수신국 방탐 장비
#define DEVICECODE_TDP					(0x84)	//수신국 PDW발생판 장비
#define DEVICECODE_TRD					(0x82)	//수신국 레이더방탐 
#define DEVICECODE_TAS					(0x81)	//수신국 ADS-B 장비

#define DEVICECODE_BTA					(0x04)	//감시국 TDOA 장비
#define DEVICECODE_BRA					(0x02)	//감시국 레이더 분석
#define DEVICECODE_BOV					(0x01)	//감시국 운용
#define DEVICECODE_BROADCASTING			(0x00)	//공통(BROADCASTING)

// 메시지 우선순위 정의: 메시지 흐름제어시 우선순위
#define CLASS1									(0x01)
#define CLASS2									(0x02)
#define CLASS3									(0x03)
#define CLASS4									(0x04)
#define CLASS5									(0x05)

//수신 장비 식별
#define Equip_Rev_0V							(0x01)	//감시국
#define Equip_Rev_BD							(0x02)  //감시국_레이더분석
#define Equip_Rev_TA							(0x03)	//TDOA
#define Equip_Rev_AS							(0x04)	//수신국_ADS-B
#define Equip_Rev_TF							(0x05)	//수신국_레이더방탐
#define Equip_Rev_DP							(0x06)	//수신국_방탐PDW
#define Equip_Rev_DF							(0x07)	//수신국_방탐
#define Equip_Rev_VU							(0x08)	//수신국_V/UHF
#define Equip_Rev_GD							(0x09)	//GPS방탐기
#define Equip_Rev_SF							(0x0A)	//음영지역검출기

#define SYSTEM_CLR_EQUIP1	14
#define SYSTEM_CLR_EQUIP2	24
#define SYSTEM_CLR_EQUIP3	34

#define OPERID_1			40
#define OPERID_2			46
#define OPERID_3			48

#pragma pack(push,1)
#ifndef SOPCODE
#define SOPCODE
struct SOPCode {
	unsigned char uchClass: 4;
	unsigned char uchEncrpytion: 4;
	unsigned char uchCMDCode;
	unsigned char uchDest;
	unsigned char uchSrc;

	SOPCode() :
	uchSrc(0),
		uchDest(0),
		uchCMDCode(0),
		uchEncrpytion(0),
		uchClass(0)		
	{}
};
#endif
#ifndef STIME
#define STIME
struct STime
{
	//6/4/5/5/6/6
	int nYear:6;
	int nMonth:4;
	int nDay:5;
	int nHour:5;
	int nMin:6;
	int nSec:6;
	STime()
		:nYear(2000),
		nMonth(1),
		nDay(1),
		nHour(0),
		nMin(0),
		nSec(0)
	{}
	UINT GetTime() {
		UINT unTime;
		memcpy(&unTime, this, sizeof(UINT));
		return unTime;
	}
	UINT GetCurTime() {
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		nYear	= stTime.wYear;
		nMonth	= stTime.wMonth;
		nDay	= stTime.wDay;
		nHour	= stTime.wHour;
		nMin	= stTime.wMinute;
		nSec	= stTime.wSecond;
		return GetTime();
	}
};
#endif
#pragma pack(pop)

class CCommonMngr;

enum ESocketStatus
{
	SOCKET_NOT_CREATED = 0,
	SOCKET_SERVER,
	SOCKET_CLIENT,
};

struct StOpcode
{
#ifdef BIG_ENDIAN
	unsigned char ucSource;
	unsigned char ucDestination;
	unsigned char ucCommandCode;
	unsigned char ucEncrytion		:4;
	unsigned char ucClass			:4;
#else
	unsigned char ucClass			:4;
	unsigned char ucEncrytion		:4;
	unsigned char ucCommandCode;
	unsigned char ucDestination;
	unsigned char ucSource;
#endif
};

struct StMissionHeader
{
#ifdef BIG_ENDIAN
	struct StOpcode stOpcode;

	unsigned short usSize;
	unsigned char  ucLinkID				:2;
	unsigned char  ucRevOprID			:6;
	unsigned char   iOperID;
#else
	struct StOpcode stOpcode;

	unsigned char   iOperID;
	unsigned char  ucRevOprID			:6;
	unsigned char  ucLinkID				:2;
	unsigned short usSize;	
#endif
};

struct StMissionPacket
{
	struct StMissionHeader stMissionHeader;
	char pcData[MAX_DATA_SIZE];
};

struct StOperOpcode
{
#ifdef BIG_ENDIAN
	unsigned char ucSource;
	unsigned char ucDestination;
	unsigned char ucCommandCode;
	unsigned char ucEncrytion		:4;
	unsigned char ucClass			:4;
#else
	unsigned char ucClass			:4;
	unsigned char ucEncrytion		:4;
	unsigned char ucCommandCode;
	unsigned char ucDestination;
	unsigned char ucSource;
#endif
};

struct StMissionOperHeader
{
	struct StOperOpcode stOpcode;

	unsigned short usSize;
	unsigned short  iOperID				:8;
	unsigned short  ucRevOprID			:6;
	unsigned short  ucLinkID			:2;	
};

struct StMissionOperPacket
{
	struct StMissionOperHeader stMissionHeader;
	char pcData[MAX_DATA_SIZE];
};


struct StLinkHeaderNFrame
{
#ifdef BIG_ENDIAN
	unsigned int	uiACID				:4;
	unsigned int	uiEncryption		:1;
	unsigned int	uiCompression		:1;
	unsigned int	uiAck				:1;
	unsigned int	uiRetransmission	:1;
	unsigned int	uiFramePartition	:1;
	unsigned int	uiLSize				:23;

	unsigned int	uiSequenceNumber;

	unsigned short	usTotalFrameCount;
	unsigned short	usFrameNumber;
#else
	unsigned int	uiLSize				:23;
	unsigned int	uiFramePartition	:1;
	unsigned int	uiRetransmission	:1;
	unsigned int	uiAck				:1;
	unsigned int	uiCompression		:1;
	unsigned int	uiEncryption		:1;
	unsigned int	uiACID				:4;

	unsigned int	uiSequenceNumber;

	unsigned short	usFrameNumber;
	unsigned short	usTotalFrameCount;
#endif
};

struct StLinkHeader1Frame
{
#ifdef BIG_ENDIAN
	unsigned int	uiACID				:4;
	unsigned int	uiEncryption		:1;
	unsigned int	uiCompression		:1;
	unsigned int	uiAck				:1;
	unsigned int	uiRetransmission	:1;
	unsigned int	uiFramePartition	:1;
	unsigned int	uiLSize				:23;

	unsigned int	uiSequenceNumber;
#else
	unsigned int	uiLSize				:23;
	unsigned int	uiFramePartition	:1;
	unsigned int	uiRetransmission	:1;
	unsigned int	uiAck				:1;
	unsigned int	uiCompression		:1;
	unsigned int	uiEncryption		:1;
	unsigned int	uiACID				:4;

	unsigned int	uiSequenceNumber;
#endif
};

struct StLinkPacketNFrame
{
	struct StLinkHeaderNFrame stLinkHeader;
	struct StMissionPacket stMissionPacket;
};

struct StLinkPacket1Frame
{
	struct StLinkHeader1Frame stLinkHeader;
	struct StMissionPacket stMissionPacket;
};

struct StMissionPacketContainer
{
	void *pvPacket;
	int iLength;
};

struct StClientSocket
{
	unsigned short  usOperatorID;
	unsigned short  us4thIP;
	SOCKET hSocket;
	bool bDataLink;
};

struct StSendPacketInfo
{
	void *pvPacket;
	int iLength;
};

#define PACKET_TYPE_RECV	1
#define PACKET_TYPE_SEND	2

struct StRecvPacketInfo
{
	void *pvPacket;
	int iLength;	
	int iType;
};

static bool ChangeEndianness(void *i_pvPacket, unsigned int i_uiSize)
{
	unsigned int i = 0;
	unsigned int uiChangeCount = i_uiSize/4;
	unsigned int uiTemp = 0;

	if(0 != (i_uiSize % 4))
	{
		return false;
	}
	else
	{
		for(i=0; i<uiChangeCount; i++)
		{
			*((unsigned int*)i_pvPacket+i) = htonl(*((unsigned int*)i_pvPacket+i));
		}
	}
	return true;
}

static bool ChangeEndiannessForMissionHeadeer(void *i_pvPacket, unsigned int i_uiSize)
{
	unsigned int i = 0;
	unsigned int uiChangeCount = i_uiSize/4;
	unsigned int uiTemp = 0;

	if(0 != (i_uiSize % 4))
	{
		return false;
	}
	else
	{
		StMissionHeader stTempMissionHeader = StMissionHeader();
		memcpy(&stTempMissionHeader, i_pvPacket, i_uiSize);

		unsigned int uiTempEndian = 0;

		memcpy(&uiTempEndian, &stTempMissionHeader.stOpcode, sizeof(unsigned int));
		uiTempEndian = htonl(uiTempEndian);
		memcpy(&stTempMissionHeader.stOpcode, &uiTempEndian, sizeof(unsigned int));

		stTempMissionHeader.usSize = htons(stTempMissionHeader.usSize);

		memcpy(i_pvPacket, &stTempMissionHeader, i_uiSize);
	}
	return true;
}