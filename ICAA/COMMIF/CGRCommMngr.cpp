#include "stdafx.h"
#include "CGRCommMngr.h"
#include "CGRCommObj.h"
#include <MSTcpIP.h>

//extern DWORD WINAPI TcpServerThreadFunc(LPVOID arg);
//extern DWORD WINAPI TcpClientThreadFunc(LPVOID arg);
//extern DWORD WINAPI TcpPacketProcessingThreadFunc(LPVOID arg);

/**
* @brief CGRCommMngr 생성자
*/
CGRCommMngr::CGRCommMngr() :
	m_bInitialized(false),
	m_eSocketStatus(SOCKET_NOT_CREATED),
	m_hServerSocket(NULL),
	m_hDataLinkClientSocket(NULL),
	m_hDataLinkServerSocket(NULL)
{
	int i;
	for ( i = NULL; i < MAX_CLIENT_CONNECT_CNT; i++ )
	{
		m_hClientSocket[i] = NULL;
	}
	
	m_iClientSocketCnt = NULL;
	m_hInterCommClientSocket = NULL;

	m_hUDPServerSocket = NULL;
	m_hUDPClientSocket = NULL;

	m_hDFSocket_1 = NULL;
	m_hPDWSocket_2 = NULL;
	m_hRDUnitSocket_3 = NULL;
	m_hVUHFSocket_4 = NULL;
	m_hRadarSocket_1 = NULL; //수신국

	m_bExePacketProcessingThreadFunc = false;	 

	m_usOperatorID = NULL;
	m_usOperatorID_1 = NULL;
	m_usOperatorID_2 = NULL;
	m_usOperatorID_3 = NULL;
	m_usOperatorID_4 = NULL;
	m_isSelfLastIP = false;

	Init();
};

/**
* @brief CGRCommMngr 소멸자
*/
CGRCommMngr::~CGRCommMngr()
{
	if(m_bInitialized)
	{
		WSACleanup();
	}
}

/**
* @brief Winsock 초기화 함수
* @return 실행 결과
*/
bool CGRCommMngr::Init()
{
	// 윈속 API 초기화
	WSADATA stWsaData;

	if(false == m_bInitialized)
	{
		if(0 != WSAStartup(MAKEWORD(2,2),&stWsaData)) 
		{ 
			TRACE("Winsock API Error\n"); 
			m_bInitialized = false;
			return false;
		}
		else
		{
			TRACE("Winsock API init ok\n"); 
			m_bInitialized = true;
			return true;
		}
	}

	if(false == m_bInitialized)
	{
		TRACE("Init error\n");
	}
	else
	{
		// do nothing
	}

	return m_bInitialized;
}

int CGRCommMngr::SendPacketToDataLink(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLinkID, unsigned char i_ucRevOperID, unsigned char i_ucOperID, const void *i_pvData)
{
	int iRet = NULL;
	void *pvPacket = nullptr;
	int iSize = NULL;

	struct StMissionPacket stPacket = StMissionPacket();
	memcpy(&(stPacket.stMissionHeader.stOpcode), &i_uiOpcode, sizeof(unsigned int));
	stPacket.stMissionHeader.usSize = i_usSize;
	stPacket.stMissionHeader.ucLinkID = i_ucLinkID;
	stPacket.stMissionHeader.ucRevOprID = i_ucRevOperID;
	stPacket.stMissionHeader.iOperID = i_ucOperID;
	//stPacket.stMissionHeader.usOperatorID = m_usOperatorID; // 기 저장된 IP를 자동으로 OperatorID로 설정

	// Header를 Big Endian으로 변환
	ChangeEndianness(&(stPacket.stMissionHeader), sizeof(StMissionHeader));

	// Data copy
	memcpy_s(stPacket.pcData, i_usSize, i_pvData, i_usSize);

	pvPacket = &stPacket;
	iSize = (int)(i_usSize + sizeof(StMissionHeader));

	SOCKET hSocket;
	if ( CGRCommObj::GetInstance().GetServerID() == SERVER_ID_LINK_1 )
	{
		hSocket = m_hRadarSocket_1;
	}
	else
	{
		//hSocket = m_hDLSocket_2;
	}

	iRet = CGRCommObj::GetInstance().SendToLink(hSocket, (const char*)pvPacket, iSize, i_uiOpcode);

	return iRet;
}

/**
* @brief 패킷 전송 함수
* @return 실행 결과 또는 전송 길이
*/
int CGRCommMngr::SendPacket(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLinkID, unsigned char i_ucRevOperID, unsigned char i_ucOperID, const void *i_pvData)
{
	int iRet = NULL;
	int iSize = NULL;
	void* pvPacket = nullptr;

	if(i_ucRevOperID == Equip_Rev_BD)
	{
		struct StMissionOperPacket stPacket = StMissionOperPacket();

		memcpy(&(stPacket.stMissionHeader.stOpcode), &i_uiOpcode, sizeof(unsigned int));
		stPacket.stMissionHeader.usSize = i_usSize;
		stPacket.stMissionHeader.ucLinkID = i_ucLinkID;
		stPacket.stMissionHeader.ucRevOprID = i_ucRevOperID;
		stPacket.stMissionHeader.iOperID = i_ucOperID;	

		// Data copy
		//memcpy(stPacket.pcData, i_pvData, i_usSize);
		memcpy_s(stPacket.pcData, i_usSize, i_pvData, i_usSize);

		pvPacket = &stPacket;
		iSize = (int)(i_usSize + sizeof(StMissionHeader));

		list<StClientSocket*>::iterator iter;
		StClientSocket *pTempClient = nullptr;

		if(SOCKET_CLIENT == this->m_eSocketStatus)//client
		{
			// TRACE("tcp client send\n");
			// Send to 레이더분석#1
			if ( (int)i_ucRevOperID == Equip_Rev_BD )
			{
				iRet = send(m_hDFSocket_1, (const char *)pvPacket, iSize, 0);
			}
			

			if( SOCKET_ERROR == iRet)
			{
				TRACE("TCP_SEND_ERROR_SEND\n");
				return TCP_SEND_ERROR_SEND;	
			}
		}	
		else
		{
			TRACE("TCP_SEND_ERROR_SOCKET\n");
			return TCP_SEND_ERROR_SOCKET;
		}
	}
	else
	{
		struct StMissionPacket stPacket = StMissionPacket();

		memcpy(&(stPacket.stMissionHeader.stOpcode), &i_uiOpcode, sizeof(unsigned int));
		stPacket.stMissionHeader.usSize = i_usSize;
		stPacket.stMissionHeader.ucLinkID = i_ucLinkID;
		stPacket.stMissionHeader.ucRevOprID = i_ucRevOperID;
		stPacket.stMissionHeader.iOperID = i_ucOperID;
		//stPacket.stMissionHeader.usOperatorID = CGRCommObj::GetInstance()->m_usOperatorID;
		//테스트 코드
		//i_ucOD=LINK_DEST_DF_ANLYSIS;
		// Header를 Big Endian으로 변환

		if(i_ucRevOperID != Equip_Rev_DP /*&& i_ucRevOperID !=  Equip_Rev_VU*/)
		{
			if(i_ucRevOperID ==  Equip_Rev_VU)//v_uhf
			{
				ChangeEndianness(&(stPacket.stMissionHeader), sizeof(StMissionHeader));
			}
			else
			{
				ChangeEndiannessForMissionHeadeer(&(stPacket.stMissionHeader), sizeof(StMissionHeader));
			}
		}

		if(SOCKET_SERVER == this->m_eSocketStatus)
		{
			m_isSelfLastIP =  false;
			if(i_ucLinkID == 1 && i_ucRevOperID == Equip_Rev_BD)
			{
				m_usOperatorID = SYSTEM_CLR_EQUIP1;
			}
			else if(i_ucLinkID == 2 && i_ucRevOperID == Equip_Rev_BD)
			{
				m_usOperatorID = SYSTEM_CLR_EQUIP2;
			}
			else if(i_ucLinkID == 3 && i_ucRevOperID == Equip_Rev_BD)
			{
				m_usOperatorID = SYSTEM_CLR_EQUIP3;
			}
			else if(i_ucRevOperID == Equip_Rev_0V)
			{
				m_usOperatorID = GetLastIP();			
				m_isSelfLastIP = true;
			}
		}

		// Data copy
		//memcpy(stPacket.pcData, i_pvData, i_usSize);
		memcpy_s(stPacket.pcData, i_usSize, i_pvData, i_usSize);

		pvPacket = &stPacket;
		iSize = (int)(i_usSize + sizeof(StMissionHeader));

		list<StClientSocket*>::iterator iter;
		StClientSocket *pTempClient = nullptr;

		if(SOCKET_SERVER == this->m_eSocketStatus)
		{
			for(iter = CGRCommObj::GetInstance().m_pstClientList.begin(); iter != CGRCommObj::GetInstance().m_pstClientList.end(); iter++)
			{
				pTempClient = *iter;

				if ( m_isSelfLastIP == false )
				{
					//if ( pTempClient->usOperatorID == CGRCommObj::GetInstance().m_usOperatorID )
					if(pTempClient->usOperatorID == m_usOperatorID)
					{
						iRet = send(pTempClient->hSocket, (const char *)pvPacket, iSize, NULL);

						break;
					}				
				}
				else if (m_isSelfLastIP == true) //운용POSN 3곳으로 전송
				{
					if(pTempClient->usOperatorID == OPERID_1 ||  pTempClient->usOperatorID == OPERID_2 || pTempClient->usOperatorID == OPERID_3)
					{
						iRet = send(pTempClient->hSocket, (const char *)pvPacket, iSize, NULL);				
					}							
				}
			}	
		}
		else if(SOCKET_CLIENT == this->m_eSocketStatus)//client
		{
			// TRACE("tcp client send\n");
			if((int)i_ucRevOperID == Equip_Rev_VU)
			{
				iRet = send(m_hVUHFSocket_4, (const char *)pvPacket, iSize, 0);
			}
			// Send to PDW발생판 #2
			else if((int)i_ucRevOperID == Equip_Rev_DP)
			{
				iRet = send(m_hPDWSocket_2, (const char *)pvPacket, iSize, 0);
			}		
			else;


			if( SOCKET_ERROR == iRet)
			{
				TRACE("TCP_SEND_ERROR_SEND\n");
				return TCP_SEND_ERROR_SEND;	
			}
		}	
		else
		{
			TRACE("TCP_SEND_ERROR_SOCKET\n");
			return TCP_SEND_ERROR_SOCKET;
		}		
	}	

	return iRet;	// send가 제대로 수행된 경우 보내진 size 반환 
}

/**
* @brief TCP Server Socket 생성 함수
* @return 실행 결과
*/
int CGRCommMngr::CreateTCPServer(int i_iPort)
{
	int	sizeOfLanBuf;
	struct linger	LINGER;

	SOCKADDR_IN stServerAddress;
	int iOptValue = 1;
	DWORD dwThreadID = 0;
	HANDLE hThreadhandle = NULL;

	m_hServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == m_hServerSocket)
	{
		TRACE("TCP_SERVER_ERROR_SOCKET\n"); 
		return TCP_SERVER_ERROR_SOCKET;
	}

	LINGER.l_onoff = TRUE;
	LINGER.l_linger = 0;
	setsockopt(m_hServerSocket, SOL_SOCKET, SO_LINGER, (char *)&LINGER, sizeof(LINGER));
	
	sizeOfLanBuf = 10240;
	setsockopt(m_hServerSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
	setsockopt(m_hServerSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
	setsockopt(m_hServerSocket, SOL_SOCKET, SO_RCVBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));	
	
	/*
	TIMEVAL stTimeout;
	stTimeout.tv_sec = 0;
	stTimeout.tv_usec = 100000; // 0.1 second

	setsockopt(m_hServerSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&stTimeout, sizeof(stTimeout));
	*/

	memset(&stServerAddress, 0, sizeof(stServerAddress));
	stServerAddress.sin_family = AF_INET;
	stServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	stServerAddress.sin_port = htons(i_iPort);

	int iTime = 10;
	setsockopt(m_hServerSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTime, sizeof(iTime));
	setsockopt(m_hServerSocket,SOL_SOCKET,SO_REUSEADDR,(char *)&iOptValue,sizeof(iOptValue));

	if(SOCKET_ERROR == bind(m_hServerSocket, (SOCKADDR*)&stServerAddress, sizeof(stServerAddress)))
	{
		closesocket(m_hServerSocket);
		TRACE("TCP_SERVER_ERROR_BIND\n"); 
		return TCP_SERVER_ERROR_BIND;
	}

	if(SOCKET_ERROR == listen(m_hServerSocket, MAX_CLIENT))
	{
		closesocket(m_hServerSocket);
		TRACE("TCP_SERVER_ERROR_LISTEN\n"); 
		return TCP_SERVER_ERROR_LISTEN;
	}

	CGRCommObj::GetInstance().StartTcpServerThreadFunc(&m_hServerSocket);
	
	if ( m_bExePacketProcessingThreadFunc == false )
	{
		//CreateUDPServer(i_iPort+2);
		CGRCommObj::GetInstance().StartTcpPacketProcessingThreadFunc(NULL);	//udp  
		CGRCommObj::GetInstance().StartRadarLinkPacketProcessingThreadFunc(NULL); //수신국# or Radar 데몬 수신된것
		CGRCommObj::GetInstance().StartTcpPacketProcessingThreadFuncForSend(NULL); //client 수신된것 서버로 전송

		m_bExePacketProcessingThreadFunc = true;
		
	}

	m_eSocketStatus = SOCKET_SERVER;

	return m_hServerSocket;
}

/**
* @brief TCP Server Socket 생성 함수
* @return 실행 결과
*/
//int CGRCommMngr::CreateTCPDataLinkServer(int i_iPort)
//{
//	int	sizeOfLanBuf;
//	struct linger	LINGER;
//
//	SOCKADDR_IN stServerAddress;
//	int iOptValue = 1;
//	DWORD dwThreadID = 0;
//	HANDLE hThreadhandle = NULL;
//
//	m_hDataLinkServerSocket = socket(PF_INET, SOCK_STREAM, 0);
//	if(INVALID_SOCKET == m_hDataLinkServerSocket)
//	{
//		TRACE("TCP_SERVER_ERROR_SOCKET\n"); 
//		return TCP_SERVER_ERROR_SOCKET;
//	}
//
//	LINGER.l_onoff = TRUE;
//	LINGER.l_linger = 0;
//	setsockopt(m_hDataLinkServerSocket, SOL_SOCKET, SO_LINGER, (char *)&LINGER, sizeof(LINGER));
//	sizeOfLanBuf = 10240;
//	setsockopt(m_hDataLinkServerSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
//	setsockopt(m_hDataLinkServerSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
//	setsockopt(m_hDataLinkServerSocket, SOL_SOCKET, SO_RCVBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));	
//
//	memset(&stServerAddress, 0, sizeof(stServerAddress));
//	stServerAddress.sin_family = AF_INET;
//	stServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
//	stServerAddress.sin_port = htons(i_iPort);
//
//	setsockopt(m_hDataLinkServerSocket,SOL_SOCKET,SO_REUSEADDR,(char *)&iOptValue,sizeof(iOptValue));
//
//	if(SOCKET_ERROR == bind(m_hDataLinkServerSocket, (SOCKADDR*)&stServerAddress, sizeof(stServerAddress)))
//	{
//		closesocket(m_hDataLinkServerSocket);
//		TRACE("TCP_SERVER_ERROR_BIND\n"); 
//		return TCP_SERVER_ERROR_BIND;
//	}
//
//	if(SOCKET_ERROR == listen(m_hDataLinkServerSocket, MAX_CLIENT))
//	{
//		closesocket(m_hDataLinkServerSocket);
//		TRACE("TCP_SERVER_ERROR_LISTEN\n"); 
//		return TCP_SERVER_ERROR_LISTEN;
//	}
//
//	CGRCommObj::GetInstance().StartTcpServerThreadFunc(&m_hDataLinkServerSocket);
//	if ( m_bExePacketProcessingThreadFunc == false )	
//	{
//		CreateUDPServer(i_iPort+2);
//		CGRCommObj::GetInstance().StartTcpPacketProcessingThreadFunc(NULL);	
//		m_bExePacketProcessingThreadFunc = true;		
//	}
//
//	m_eSocketStatus = SOCKET_SERVER;
//
//	return m_hDataLinkServerSocket;
//}

/**
* @brief TCP Client Socket 생성 함수
* @return 실행 결과
*/
int CGRCommMngr::CreateTCPClient(int i_iPort, const char *i_pacIP, int i_iClientNo)
{
	int iRet = NULL;
	int	sizeOfLanBuf = NULL;
	struct linger	LINGER_CreateTCPGMIClient = linger();
	memset(&LINGER_CreateTCPGMIClient, NULL, sizeof(LINGER_CreateTCPGMIClient));

	SOCKADDR_IN stServerAddress = SOCKADDR_IN();
	memset(&stServerAddress, NULL, sizeof(SOCKADDR_IN));

	DWORD dwThreadID = 0;
	HANDLE hThreadHandle = NULL;
	SOCKET hClientSocket = NULL;
	
	hClientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if( INVALID_SOCKET == hClientSocket )
	{ //EX_DTEC_TryCatchException
		closesocket(hClientSocket);
		iRet = TCP_CLIENT_ERROR_SOCKET;
	}
	else
	{
		LINGER_CreateTCPGMIClient.l_onoff = TRUE;
		LINGER_CreateTCPGMIClient.l_linger = 0;
		setsockopt(hClientSocket, SOL_SOCKET, SO_LINGER, (char *)&LINGER_CreateTCPGMIClient, sizeof(LINGER_CreateTCPGMIClient));
		sizeOfLanBuf = MAX_BUF_SIZE_SOCK;
		setsockopt( hClientSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf) );
		setsockopt( hClientSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf) );
		setsockopt( hClientSocket, SOL_SOCKET, SO_RCVBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf) );

		tcp_keepalive ka_s = {0}, ka_r = {0};
		DWORD dwBytes = 0;
		ka_s.onoff = 1;
		ka_s.keepalivetime = 2000;
		ka_s.keepaliveinterval = 1000;
		WSAIoctl(hClientSocket, SIO_KEEPALIVE_VALS, &ka_s, sizeof(ka_s), &ka_r, sizeof(ka_r), &dwBytes, NULL, NULL);

		memset(&stServerAddress, 0, sizeof(stServerAddress));
		stServerAddress.sin_family = AF_INET;
		stServerAddress.sin_addr.s_addr = inet_addr(i_pacIP);
		stServerAddress.sin_port = htons(i_iPort);

		if(SOCKET_ERROR == connect(hClientSocket, (SOCKADDR*)&stServerAddress, sizeof(stServerAddress)))
		{ //EX_DTEC_TryCatchException
			closesocket(hClientSocket);
			iRet = TCP_CLIENT_ERROR_CONNECT;
			
			//// 모의기 연동을 위한 임시코드
			//if( i_iPort == 6000 || i_iPort == 6600 )
			//{
			//	m_bMessageSimMode = false;
			//}			
		}
		else
		{
			iRet = (int)hClientSocket;
			if ( i_iClientNo == CLIENT_NO_1 )
			{
				m_hDFSocket_1 = hClientSocket;
				CGRCommObj::GetInstance().StartTcpClientDF_ThreadFunc(&m_hDFSocket_1);
				m_usOperatorID_1 = GetLastIP();
			}
			else if(i_iClientNo == CLIENT_NO_2)
			{
				m_hPDWSocket_2 = hClientSocket;
				CGRCommObj::GetInstance().StartTcpClientPDW_ThreadFunc(&m_hPDWSocket_2);
				m_usOperatorID_2 = GetLastIP();
			}
			else if(i_iClientNo == CLIENT_NO_3)
			{
				m_hRDUnitSocket_3 = hClientSocket;
				CGRCommObj::GetInstance().StartTcpClientRDUnit_ThreadFunc(&m_hRDUnitSocket_3);
				m_usOperatorID_3 = GetLastIP();

			}
			else if(i_iClientNo == CLIENT_NO_4)
			{
				m_hVUHFSocket_4 = hClientSocket;
				CGRCommObj::GetInstance().StartTcpClientVUHFUnit_ThreadFunc(&m_hVUHFSocket_4);
				m_usOperatorID_4 = GetLastIP();
			}
			else;


			if( m_bExePacketProcessingThreadFunc == false )
			{
				CGRCommObj::GetInstance().StartTcpPacketProcessingThreadFunc(NULL);
				m_bExePacketProcessingThreadFunc = true;
				m_eSocketStatus = SOCKET_CLIENT;
				m_usOperatorID = GetLastIP();
			}			
		}				
	}		

	return iRet;
}

//int CGRCommMngr::CreateTCPInterCommClient(int i_iPort, const char *i_pacIP)
//{
//	int	sizeOfLanBuf;
//	struct linger	LINGER;
//
//	SOCKADDR_IN stServerAddress;
//	DWORD dwThreadID = 0;
//	HANDLE hThreadHandle = NULL;
//
//	m_hInterCommClientSocket = socket(PF_INET, SOCK_STREAM, 0);
//	if(INVALID_SOCKET == m_hInterCommClientSocket)
//	{
//		TRACE("TCP_CLIENT_ERROR_SOCKET\n"); 
//		return TCP_CLIENT_ERROR_SOCKET;
//	}
//
//	LINGER.l_onoff = TRUE;
//	LINGER.l_linger = 0;
//	setsockopt(m_hInterCommClientSocket, SOL_SOCKET, SO_LINGER, (char *)&LINGER, sizeof(LINGER));
//	sizeOfLanBuf = 10240;
//	setsockopt(m_hInterCommClientSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
//	setsockopt(m_hInterCommClientSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
//	setsockopt(m_hInterCommClientSocket, SOL_SOCKET, SO_RCVBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
//
//	memset(&stServerAddress, 0, sizeof(stServerAddress));
//	stServerAddress.sin_family = AF_INET;
//	stServerAddress.sin_addr.s_addr = inet_addr(i_pacIP);
//	stServerAddress.sin_port = htons(i_iPort);
//
//	if(SOCKET_ERROR == connect(m_hInterCommClientSocket, (SOCKADDR*)&stServerAddress, sizeof(stServerAddress)))
//	{
//		closesocket(m_hInterCommClientSocket);
//		TRACE("TCP_CLIENT_ERROR_CONNECT\n"); 
//		return TCP_CLIENT_ERROR_CONNECT;
//	}
//	
//	CGRCommObj::GetInstance().StartTcpInterCommClientThreadFunc(&m_hInterCommClientSocket);	
//	if ( m_bExePacketProcessingThreadFunc == false )
//	{
//		CGRCommObj::GetInstance().StartTcpPacketProcessingThreadFunc(NULL);
//		m_bExePacketProcessingThreadFunc = true;		
//	}
//
//	m_eSocketStatus = SOCKET_CLIENT;
//
//	return m_hInterCommClientSocket;	
//}

void CGRCommMngr::ReleaseSocket()
{
	if ( m_hUDPServerSocket != NULL )
	{
		closesocket(m_hUDPServerSocket);
	}

	if ( m_hDFSocket_1 != NULL )
	{
		closesocket(m_hDFSocket_1);
	}

	if ( m_hPDWSocket_2 != NULL )
	{
		closesocket(m_hPDWSocket_2);
	}

	if ( m_hRDUnitSocket_3 != NULL )
	{
		closesocket(m_hRDUnitSocket_3);
	}

	if ( m_hVUHFSocket_4 != NULL )
	{
		closesocket(m_hVUHFSocket_4);
	}

	if ( m_hRadarSocket_1 != NULL )
	{
		closesocket(m_hRadarSocket_1);
	}

	/*if ( m_hUDP1ClientSocket != NULL )
	{
	closesocket(m_hUDP1ClientSocket);
	}

	if ( m_hUDP2ClientSocket != NULL )
	{
	closesocket(m_hUDP2ClientSocket);
	}

	if ( m_hInterCommClientSocket != NULL )
	{
	closesocket(m_hInterCommClientSocket);
	}

	if ( m_hProceedClientSocket != NULL )
	{
	closesocket(m_hProceedClientSocket);
	}

	if ( m_hGMISocket_1 != NULL )
	{
	closesocket(m_hGMISocket_1);
	}

	if ( m_hGMISocket_2 != NULL )
	{
	closesocket(m_hGMISocket_2);
	}*/
}

int CGRCommMngr::CreateTCPRadarLinkClient(int i_iPort, const char *i_pacIP, int i_iClientNo)
{
	int iRet = NULL;
	int	sizeOfLanBuf = NULL;
	struct linger	LINGER_CreateTCPDataLinkClient = linger();
	memset(&LINGER_CreateTCPDataLinkClient, NULL, sizeof(LINGER_CreateTCPDataLinkClient));

	SOCKADDR_IN stServerAddress = SOCKADDR_IN();
	memset(&stServerAddress, NULL, sizeof(SOCKADDR_IN));
	DWORD dwThreadID = 0;
	HANDLE hThreadHandle = NULL;
	SOCKET hClientSocket = NULL;

	hClientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == hClientSocket)
	{
		closesocket(hClientSocket);
		iRet = TCP_CLIENT_ERROR_SOCKET;
	}
	else
	{
		LINGER_CreateTCPDataLinkClient.l_onoff = TRUE;
		LINGER_CreateTCPDataLinkClient.l_linger = 0;
		setsockopt(hClientSocket, SOL_SOCKET, SO_LINGER, (char *)&LINGER_CreateTCPDataLinkClient, sizeof(LINGER_CreateTCPDataLinkClient));
		sizeOfLanBuf = MAX_BUF_SIZE_SOCK;
		setsockopt(hClientSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
		setsockopt(hClientSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
		setsockopt(hClientSocket, SOL_SOCKET, SO_RCVBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));

		tcp_keepalive ka_s = {0}, ka_r = {0};
		DWORD dwBytes = 0;
		ka_s.onoff = 1;
		ka_s.keepalivetime = 2000;
		ka_s.keepaliveinterval = 1000;
		WSAIoctl(hClientSocket, SIO_KEEPALIVE_VALS, &ka_s, sizeof(ka_s), &ka_r, sizeof(ka_r), &dwBytes, NULL, NULL);

		memset(&stServerAddress, 0, sizeof(stServerAddress));
		stServerAddress.sin_family = AF_INET;
		stServerAddress.sin_addr.s_addr = inet_addr(i_pacIP);
		stServerAddress.sin_port = htons(i_iPort);

		int iTime = 10;
		setsockopt(hClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTime, sizeof(iTime));

		if(SOCKET_ERROR == connect(hClientSocket, (SOCKADDR*)&stServerAddress, sizeof(stServerAddress)))
		{
			closesocket(hClientSocket);
			iRet = TCP_CLIENT_ERROR_CONNECT;
		}
		else
		{
			// 정상할당
			iRet = (int)hClientSocket;

			if ( i_iClientNo == CLIENT_NO_1 )
			{
				m_hRadarSocket_1 = hClientSocket;
				CGRCommObj::GetInstance().SetRadarLink1ClientSocket(m_hRadarSocket_1);
				CGRCommObj::GetInstance().StartTcpRadarLink_1ClientThreadFunc(&m_hRadarSocket_1);	
			}
			else
			{
				/*m_hDLSocket_2 = hClientSocket;
				CGRCommObj::GetInstance()->SetDataLink2ClientSocket(m_hDLSocket_2);
				CGRCommObj::GetInstance()->StartTcpDataLink_2ClientThreadFunc(&m_hDLSocket_2);*/
			}

			if( !m_bExePacketProcessingThreadFunc )
			{
				CGRCommObj::GetInstance().StartTcpPacketProcessingThreadFunc(NULL);
				m_bExePacketProcessingThreadFunc = true;		
			}

			m_eSocketStatus = SOCKET_SERVER;
		}
	}

	return iRet;
}

int CGRCommMngr::CreateUDPServer(int i_iPort)
{	
	char time_live=64;
	SOCKADDR_IN addrSocket;

	m_hUDPServerSocket=socket(PF_INET, SOCK_DGRAM, 0);
	if(m_hUDPServerSocket == INVALID_SOCKET)
	{
		return UDP_SERVER_ERROR_SOCKET;
	}

	memset(&addrSocket, 0, sizeof(addrSocket));
	addrSocket.sin_family = AF_INET;
	addrSocket.sin_addr.s_addr=inet_addr("239.255.255.255"); //멀티캐스트 IP ex> MULTI_IP_ADDR
	addrSocket.sin_port= htons(i_iPort);

	setsockopt(m_hUDPServerSocket, IPPROTO_IP, IP_MULTICAST_TTL, &time_live, sizeof(time_live));

	// Udp 송신 소켓 및 주소 설정
	CGRCommObj::GetInstance().SetUdpSocket(m_hUDPServerSocket);
	CGRCommObj::GetInstance().SetUdpSocketAddr(addrSocket);

	return m_hUDPServerSocket;
}

/*
#define UDP_SERVER_ERROR_SOCKET		(-1)
#define UDP_SERVER_ERROR_BIND		(-2)
#define UDP_SERVER_ERROR_LISTEN		(-3)

#define UDP_CLIENT_ERROR_SOCKET		(-1)
#define UDP_CLIENT_ERROR_CONNECT	(-2)
*/

int CGRCommMngr::CreateUDPClient(int i_iPort, const char *i_pacIP)
{
	int iRet = NULL;
	SOCKADDR_IN adr = SOCKADDR_IN();
	memset(&adr, NULL, sizeof(SOCKADDR_IN));	

	struct ip_mreq joinAdr = ip_mreq();
	memset(&joinAdr, NULL, sizeof(ip_mreq));

	SOCKET hSocket = socket(PF_INET, SOCK_DGRAM, 0);

	if(hSocket == INVALID_SOCKET)
	{ //DTEC_TryCatchException
		closesocket(hSocket);
		iRet = UDP_CLIENT_ERROR_SOCKET;
	}
	else
	{
		memset(&adr, 0, sizeof(adr));
		adr.sin_family = AF_INET;
		adr.sin_addr.s_addr = htonl(INADDR_ANY);
		adr.sin_port = htons(i_iPort);

		if(bind(hSocket, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR)
		{ //EX_DTEC_TryCatchException	
			closesocket(hSocket);
			iRet = UDP_CLIENT_ERROR_SOCKET;
		}
		else
		{
			joinAdr.imr_multiaddr.s_addr = inet_addr(i_pacIP);
			joinAdr.imr_interface.s_addr = htonl(INADDR_ANY);

			if( setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR)
			{ //DTEC_TryCatchException
				closesocket(hSocket);
				iRet = UDP_CLIENT_ERROR_SOCKET;
			}
			else
			{				
				m_hUDPClientSocket = hSocket;
				CGRCommObj::GetInstance().StartUdpClientThreadFunc(&m_hUDPClientSocket);								
			}
		}
	}

	/*SOCKADDR_IN adr;
	struct ip_mreq joinAdr;
	
	m_hUDPClientSocket=socket(PF_INET, SOCK_DGRAM, 0);
	if(m_hUDPClientSocket == INVALID_SOCKET)
	{		
		return UDP_CLIENT_ERROR_SOCKET;
	}

	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = inet_addr(i_pacIP);
	adr.sin_port = htons(i_iPort);

	if(bind(m_hUDPClientSocket, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR)
	{	
		closesocket(m_hUDPClientSocket);
		
		return UDP_CLIENT_ERROR_SOCKET;
	}

	joinAdr.imr_multiaddr.s_addr = inet_addr(i_pacIP);
	joinAdr.imr_interface.s_addr = htonl(INADDR_ANY);
	
	if( setsockopt(m_hUDPClientSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR)
	{		
		return UDP_CLIENT_ERROR_SOCKET;
	}

	CGRCommObj::GetInstance().StartUdpClientThreadFunc(&m_hUDPClientSocket);*/

	return iRet;
}


unsigned short CGRCommMngr::GetLastIP()
{
	unsigned short usLastIP = NULL;

	//////// 자신의 IP 획득 //////////////////////////////////////////////////////////////////
	char name[255] = {0,};
	memset(name, NULL, sizeof(name));

	PHOSTENT hostinfo = PHOSTENT();
	memset(&hostinfo, NULL, sizeof(PHOSTENT));

	CString strIP = _T("");
	if ( gethostname(name, sizeof(name)) == NULL )
	{
		hostinfo = gethostbyname(name);
		if ( hostinfo != NULL )
		{
			strIP = inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[0]);
		}
	}

	int iOffset = NULL;
	int iIp = NULL;
	iOffset = strIP.Find(".", 0);
	iOffset = strIP.Find(".", iOffset+1);
	iOffset = strIP.Find(".", iOffset+1);
	strIP = strIP.Mid(iOffset+1, strIP.GetLength()-iOffset-1);
	iIp	= _ttoi(strIP);

	if ( iIp > 0 && iIp < 65536 )
	{
		usLastIP = (unsigned short)iIp;
	}	 
	//////////////////////////////////////////////////////////////////////////////////////////

	return usLastIP;
}