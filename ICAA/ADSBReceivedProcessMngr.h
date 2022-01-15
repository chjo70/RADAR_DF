#pragma once

#include <list>
#include <map>

#include "MsgQueueThread.h"
#include "CommIF\CommonMngr.h"
#include "CommIF\CGRCommIF.h"

#include <sstream>
#include <iomanip>
#include <cmath>

//#define PI 3.1415926535897
#define MAX_BUF_ADS_SIZE       66560	//65536	//8192 // 한번에 읽어들일 최대 버프 사이즈

using namespace std;


#pragma pack(1)
typedef struct stFlightADSBData
{
	char			acFlightID[7];
	char			acFlightTitle[9];
	char			acTrackTime[18];
	unsigned int	uiLongitude;
	unsigned int	uiLatitude;
	unsigned int	uiAltitude;
	unsigned short	usVelocity;
	unsigned short	usHeading;

	stFlightADSBData()
	{
		memset(acFlightID, 0, sizeof(acFlightID));
		memset(acFlightTitle, 0, sizeof(acFlightTitle));
		memset(acTrackTime, 0, sizeof(acTrackTime));
		uiLongitude = 0;
		uiLatitude = 0;
		uiAltitude = 0;
		usVelocity = 0;
		usHeading = 0;
	}
}STFlightADSBData;

typedef struct stSendADSBData
{
	unsigned char	ucADSB_ID;
	unsigned char	ucNumOfData;
	STFlightADSBData pstFlightADSBData[2000];

	stSendADSBData()
	{
		ucADSB_ID = 0;
		ucNumOfData = 0;
		memset(&pstFlightADSBData, 0, sizeof(STFlightADSBData)*2000);
	}

}STSendADSBData;


typedef struct stADSBRawPacket
{
	int buf_size;
	char buf[MAX_BUF_SIZE];

	stADSBRawPacket()
	{
		buf_size = 0;
		memset(buf, 0, sizeof(char)*MAX_BUF_SIZE);
	}
}STADSBRawPacket;

typedef struct stADSBDecodedData
{
	string strICAOAddr;
	char acAircarftID[9];
	bool isEven;
	bool isOdd;
	float fLat;
	float fLon;
	float fLatEven;
	float fLatOdd;
	float fLonEven;
	float fLonOdd;
	unsigned int uiAltitude;
	bool bIsGroundSpeed;
	float fVel;
	float fHeading;
	char acTrackTime[18];
	unsigned int uiTout;
	
	stADSBDecodedData()
	{
		strICAOAddr = "";
		memset(acAircarftID, 0, sizeof(acAircarftID));
		isEven = false;
		isOdd = false;
		fLatEven = 0;
		fLatOdd = 0;
		fLonEven = 0;
		fLonOdd = 0;
		uiAltitude = 0;
		fLat = 0;
		fLon = 0;
		fVel = 0;
		fHeading = 0;
		bIsGroundSpeed = false;
		memset(acTrackTime, 0, sizeof(acTrackTime));
		uiTout = 0;
	}
}STADSBDecodedData;


class CADSBReceivedProcessMngr : public CMsgQueueThread, public CCommonMngr
{
public:
	static CADSBReceivedProcessMngr* GetInstance();

	void Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData);
	void ProcessMsg(STMsg& i_stMsg);
	bool SendADSBData();

	CGRCommIF& m_hCommIF_ADSBRPMngr;
	static CADSBReceivedProcessMngr* uniqueInstance;
	
private:
	CADSBReceivedProcessMngr();
	~CADSBReceivedProcessMngr();
	
	bool ConvertCharToBinary(IN char* i_pcInput, IN int i_iSize, OUT char* i_pcOuput);
	bool ConvertBinaryToInt(IN char* i_pcInput, IN int i_iCntOfBits, OUT int* i_piOutput);
	double NL_lat(double i_dbLat);

	bool ParseDF(IN STADSBRawPacket* i_stADSBRawPacket);
	bool ParseICAO(STADSBRawPacket* i_stADSBRawPacket, STADSBDecodedData* i_stADSBDecodedData);
	unsigned char ParseTC(STADSBRawPacket* i_stADSBRawPacket);
	bool ParseAI(STADSBRawPacket* i_stADSBRawPacket, STADSBDecodedData* i_stADSBDecodedData);
	bool ParseAP(STADSBRawPacket* i_stADSBRawPacket, STADSBDecodedData* i_stADSBDecodedData);
	bool ParseAV(STADSBRawPacket* i_stADSBRawPacket, STADSBDecodedData* i_stADSBDecodedData);

	bool PushADSBRawPacket(STADSBRawPacket i_stADSBRawPacket);
	bool ProcessADSBParse();

	bool CalcLat_Lon(bool i_bIsEvenRecently, STADSBDecodedData* i_pstADSBDecodedData);
	bool PutTrackTimeInfo(STADSBDecodedData* i_stADSBDecodedData);
	unsigned char GetLastIP();

public:
	std::map<string, STADSBDecodedData>	m_mapADSBDecodedData;
	unsigned char m_ucSendingTIme;
	unsigned char m_ucTout;
	bool m_bIsAvalibleForSend;
	unsigned char m_ucSendADSBDataInterval;
	unsigned char m_ucCheckTick;
	unsigned char m_ucMyLastIP;

private:
	std::list<STADSBRawPacket>		m_listADSBRawPacket;
	
	CRITICAL_SECTION m_csReceiveAndAssign;

	HANDLE m_hTimer;
	HANDLE m_hTimerQueue;

};