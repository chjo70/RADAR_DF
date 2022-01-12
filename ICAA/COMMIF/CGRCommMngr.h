#pragma once

//#include "stdafx.h"
#include "atlcore.h"
#include "winsock2.h"
#include "CommStruct.h"

/**
* [식별자 : DD-GMU-SFR-0015]
* [추적성 : SRS-GMU-SOFR-015]
*
* @class CGRCommMngr
* @brief LAN 통신관리 클래스
*
* (1)　클래스 설명
*  - 외부와 LAN으로 통신을 수행하는 클래스이다.
* (2) 설계결정사항
*  - 통신 대상 장비는 지상임무연동기, 저장데이터처리기이다.
* (3) 제한 및 예외처리
*  - 정의되지 않은 메시지의 경우에는 false를 반환한다.
*/
class CGRCommMngr
{
public:
	CGRCommMngr();
	~CGRCommMngr();

	bool Init();

	// add service server
	int CreateTCPServer(int i_iPort);
	int CreateTCPRadarLinkClient(int i_iPort, const char *i_pacIP, int i_iClientNo);
	//int CreateTCPInterCommClient(int i_iPort, const char *i_pacIP);
	//int CreateTCPDataLinkServer(int i_iPort);
	int CreateUDPServer(int i_iPort);

	// add service client
	int CreateTCPClient(int i_iPort, const char *i_pacIP, int i_iClientNo);
	int CreateUDPClient(int i_iPort, const char *i_pacIP);

	// send
	int SendPacket(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLinkID, unsigned char i_ucRevOperID, unsigned char i_ucOperID, const void *i_pvData);
	int SendPacketToDataLink(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLinkID, unsigned char i_ucRevOperID, unsigned char i_ucOperID, const void *i_pvData);


	SOCKET GetClientSocket(const char *i_pacIP = NULL);

	void ReleaseSocket();
	unsigned short GetLastIP();

private:
	bool m_bInitialized;
	ESocketStatus m_eSocketStatus;	// 0 : Server, 1 : Client
	SOCKET m_hClientSocket[MAX_CLIENT_CONNECT_CNT];
	
	SOCKADDR_IN m_mul_adr;

	SOCKET m_hUDPServerSocket;
	SOCKET m_hUDPClientSocket;
	SOCKET m_hServerSocket;	
	SOCKET m_hDataLinkClientSocket;
	SOCKET m_hDataLinkServerSocket;
	SOCKET m_hInterCommClientSocket;

	SOCKET m_hDFSocket_1; //레어더 분석
	SOCKET m_hPDWSocket_2; //PDW 발생판
	SOCKET m_hRDUnitSocket_3; //레어더 방탐장비
	SOCKET m_hVUHFSocket_4; //V/UHF방탐장비

	SOCKET m_hRadarSocket_1;

	unsigned short m_usOperatorID;
	bool m_isSelfLastIP;

	struct StMissionPacket m_stPacket;
	struct StMissionPacket m_stInterCommPacket;
	struct StClientSocket m_stClientSocket;	
	int m_iClientSocketCnt;

	bool m_bExePacketProcessingThreadFunc;

	unsigned short m_usOperatorID_1;
	unsigned short m_usOperatorID_2;
	unsigned short m_usOperatorID_3;
	unsigned short m_usOperatorID_4;
};