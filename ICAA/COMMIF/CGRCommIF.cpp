#include "stdafx.h"
#include "CGRCommIF.h"
//#include "../ASAE_DEMONDlg.h"

//#include "../../ELDummyASAE/ELDummyASAE.h"
//#include "../../ELDummyASAE/PopDialogEL/ELGrounInterfaceSim.h"
//#include "../MNGR/GRLogMngr.h"

void CALLBACK GRPingTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired);

/**
* @brief CGRCommIF 생성자
*/
CGRCommIF::CGRCommIF() :
	m_hClientSocket(NULL),
	m_hServerSocket(NULL),
	m_hCommObj(CGRCommObj::GetInstance())
{
	m_bConnect = false;
	m_bServer = false;

	m_hTimer = NULL;
	m_hTimerQueue = NULL;

	if(m_hTimerQueue == NULL)
	{	
		m_hTimerQueue = CreateTimerQueue();
		CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, (WAITORTIMERCALLBACK)GRPingTimer, this, PING_START_INTERVAL, PING_INTERVAL, 0);
	}

}

/**
* @brief CGRCommIF 소멸자
*/
CGRCommIF::~CGRCommIF()
{
	BOOL bRtn = DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, NULL);

	if(bRtn)
	{
		m_hTimerQueue = NULL;
	}
	else
	{
		if ( ERROR_IO_PENDING == GetLastError() || m_hTimerQueue == NULL || m_hTimer == NULL )
		{
			m_hTimerQueue = NULL;
		}
		else
		{
			while(DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, NULL) == 0)
			{
				if ( ERROR_IO_PENDING == GetLastError() || m_hTimerQueue == NULL || m_hTimer == NULL )
				{
					m_hTimerQueue = NULL;
					break;
				}
			}			
		}
	}	
}

void CGRCommIF::ReleaseSocket()
{
	m_hCommObj.FinishPacketProcessingThread();
	m_cCommMngr.ReleaseSocket();
}

CGRCommIF::Cleanup::~Cleanup()
{
	//ReleaseInstance();
}


/**
* @brief CGRCommIF의 singleton pattern 함수
* @return Process에서 유일한 CGRCommIF의 객체
*/
CGRCommIF& CGRCommIF::GetInstance()
{
	static CGRCommIF cGRCommIF;
	return cGRCommIF;
}


bool CGRCommIF::SendToDataLink(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLinkID, unsigned char i_ucRevOperID, unsigned char i_ucOperID, const void *i_pvData)
{
	// size 검사
	if(MAX_DATA_SIZE < i_usSize)
	{
		return false;
	}

	int iRet = m_cCommMngr.SendPacketToDataLink(i_uiOpcode, i_usSize, i_ucLinkID, i_ucRevOperID, i_ucOperID, i_pvData);

	switch(iRet)
	{
	case TCP_SEND_ERROR_SEND:
		{
			return false;
		}

	case TCP_SEND_ERROR_SOCKET:
		{
			return false;
		}

	default: // 정상
		{
			return true;
		}
	}

}

bool CGRCommIF::Connect(int i_iPort, const char *i_pacIP, int i_iNo)
{
	int iRet;
	bool bRst = false;
	bool bDirect = false;

	if ( m_bServer == true ) // 수신국 연결
	{
		if ( bDirect == false ) //수신국 연결
		{
			iRet = m_cCommMngr.CreateTCPRadarLinkClient(i_iPort, i_pacIP, i_iNo);
		}	
	}
	else // Client에서 - Server 연결 
	{
		iRet = m_cCommMngr.CreateTCPClient(i_iPort, i_pacIP, i_iNo); //Client
	}

	if ( iRet != TCP_CLIENT_ERROR_SOCKET && iRet != TCP_CLIENT_ERROR_CONNECT )
	{
		m_hCommObj.SetConnectionInfo(i_iNo, true);

		//if ( m_bServer == false )			// POSN
		//{
		//	// POSN에서는 TCP 소켓 연결 성공시 UDP 소켓을 생성
		//	if ( m_cCommMngr.CreateUDPClient(i_iPort+2, "239.255.255.251", i_iNo) != UDP_CLIENT_ERROR_SOCKET )
		//	{
		//		bRst = true;
		//	}
		//}

	}
	else
	{ //EX_DTEC_Else
		m_hCommObj.SetConnectionInfo(i_iNo, false);		
	}	

	return bRst;
}


/**
* @brief 메시지 송신 함수
* @return 실행 결과
*/
bool CGRCommIF::Send(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLinkID, unsigned char i_ucRevOperID, unsigned char i_ucOperID, const void *i_pvData)
{
	// size 검사
	/*if(MAX_DATA_SIZE < i_usSize)
	{
	return false;
	}*/

	bool bRst = false;

	int iRet = m_cCommMngr.SendPacket(i_uiOpcode, i_usSize, i_ucLinkID, i_ucRevOperID, i_ucOperID, i_pvData);
	//TRACE("Send Rst. size = %d\n", iRet);

	if ( iRet != TCP_SEND_ERROR_SEND && iRet != TCP_SEND_ERROR_SOCKET && iRet != 0 )
	{
		bRst = true;
	}

	return bRst;
}

/**
* @brief 클라이언트 연결 함수
* @return 실행 결과
*/
//bool CGRCommIF::Connect(int i_iPort, const char *i_pacIP)
//{
//	if ( m_bServer )
//		m_PingThread.SetLinkIP(i_pacIP);
//	else
//		m_PingThread.SetServerIP(i_pacIP);
//
//	if ( m_bServer && ( i_iPort != DATA_LINK_CONNECT_PORT && i_iPort != INTER_COMM_CONNECT_PORT ) ) // 서버모드로 동작시 클라이언트 접속은 일괄 차단한다. - 지상임무연동기 모드
//		return false;
//	  
//	if ( m_bConnect == true)
//	{
//		for(std::list<int>::iterator it = m_listConnectPort.begin(); it != m_listConnectPort.end(); it++)
//		{
//			if ( (*it) == i_iPort )
//			{
//				// 동일 포트에 대한 추가 연결은 거절한다
//				return true;
//			}
//		}
//	}
//	
//	m_listConnectPort.push_back(i_iPort);
//
//	int iRet;
//	
//	if ( i_iPort == DATA_LINK_CONNECT_PORT )
//		iRet = m_cCommMngr.CreateTCPDataLinkClient(i_iPort, i_pacIP);
//	else if ( i_iPort == INTER_COMM_CONNECT_PORT )
//		iRet = m_cCommMngr.CreateTCPInterCommClient(i_iPort, i_pacIP);
//	else 
//		//iRet = m_cCommMngr.CreateTCPClient(i_iPort, i_pacIP);
//
//	switch (iRet)
//	{
//	case TCP_CLIENT_ERROR_SOCKET:		
//		{
//			if ( i_iPort == DATA_LINK_CONNECT_PORT )
//			{				
//				m_hCommObj.SetConnectionInfo(PING_TIMER_DATA_LINK, false);
//			}
//			else if ( i_iPort == INTER_COMM_CONNECT_PORT )
//			{
//				;
//			}
//			else
//			{
//				m_hCommObj.SetConnectionInfo(PING_TIMER_SERVER, false);
//			}
//
//			return false;
//		}	
//
//	case TCP_CLIENT_ERROR_CONNECT:
//		{			
//			if ( i_iPort == DATA_LINK_CONNECT_PORT )
//			{				
//				m_hCommObj.SetConnectionInfo(PING_TIMER_DATA_LINK, false);
//			}
//			else if ( i_iPort == INTER_COMM_CONNECT_PORT )
//			{
//				;
//			}
//			else
//			{
//				m_hCommObj.SetConnectionInfo(PING_TIMER_SERVER, false);
//			}
//			return false;			
//		}		
//
//	default: // 정상
//		{
//			m_hClientSocket = iRet;
//			m_bConnect = true;
//
//			if ( i_iPort == DATA_LINK_CONNECT_PORT )
//			{
//				m_hCommObj.SetDataLinkClientSocket(m_hClientSocket);
//				m_hCommObj.SetConnectionInfo(PING_TIMER_DATA_LINK, true);
//			}
//			else if ( i_iPort == INTER_COMM_CONNECT_PORT )
//			{
//				m_hCommObj.SetInterCommClientSocket(m_hClientSocket);				
//			}
//			else
//			{
//				m_hCommObj.SetConnectionInfo(PING_TIMER_SERVER, true);
//
//				// TCP 소켓 연결 성공시 UDP 소켓을 생성
//				if ( m_cCommMngr.CreateUDPClient(i_iPort+2, "239.255.255.255") == UDP_CLIENT_ERROR_SOCKET )
//					return false;
//				else
//					return true;
//			}
//		}
//	}
//}

/**
* @brief Opcode 등록 함수
* @return 실행 결과
*/
bool CGRCommIF::RegisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr)
{
	bool bRet = m_hCommObj.RegisterOpcode(i_uiOpcode, i_pcMngr);
	return bRet;
}

/**
* @brief Opcode 등록 함수
* @return 실행 결과
*/
bool CGRCommIF::UnregisterOpcode(unsigned int i_uiOpcode, CCommonMngr *i_pcMngr)
{
	bool bRet = m_hCommObj.UnregisterOpcode(i_uiOpcode, i_pcMngr);
	return bRet;
}

/**
* @brief 소켓 재연결위함 임시
* @return 실행 결과
*/
bool CGRCommIF::SetReConnect(bool i_bReConnect)
{
	m_bConnect = i_bReConnect;
	return true;
}

//bool CGRCommIF::CreateUDPDemon()
//{	
//	m_cCommMngr.CreateUDPServer(41001);
//	m_cCommMngr.CreateUDPClient(41002, "239.255.255.255");
//
//	
//	CGRCommObj &cGRCommObj = CGRCommObj::GetInstance();
//	cGRCommObj.StartTcpPacketProcessingThreadFunc(NULL);
//
//	//cGRCommObj.SetDlgPointer(i_pDlg);
//
//	return true;
//}

bool CGRCommIF::SendUDPData(int i_iSize, const void *i_pvData)
{
	CGRCommObj &cGRCommObj = CGRCommObj::GetInstance();

	SOCKADDR_IN addrUdpSock;	
	memset(&addrUdpSock, NULL, sizeof(SOCKADDR_IN));
	memcpy(&addrUdpSock, &cGRCommObj.GetUdpSocketAddr(), sizeof(SOCKADDR_IN));

	SOCKET hUdpSocket;
	hUdpSocket = NULL;
	hUdpSocket = cGRCommObj.GetUdpSocket();	

	sendto(hUdpSocket, (const char *)i_pvData, i_iSize, 0, (SOCKADDR*)&addrUdpSock, sizeof(addrUdpSock));	

	return true;
}

bool CGRCommIF::ConnectADSBS(int i_iPort, const char *i_pacIP)
{
	int iRet;
	int	sizeOfLanBuf;
	struct linger	LINGER;

	SOCKADDR_IN stServerAddress;
	DWORD dwThreadID = 0;
	HANDLE hThreadHandle = NULL;

	m_hClientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == m_hClientSocket)
	{
		TRACE("TCP_CLIENT_ERROR_SOCKET\n"); 
		return TCP_CLIENT_ERROR_SOCKET;
	}

	LINGER.l_onoff = TRUE;
	LINGER.l_linger = 0;
	setsockopt(m_hClientSocket, SOL_SOCKET, SO_LINGER, (char *)&LINGER, sizeof(LINGER));
	sizeOfLanBuf = 10240;
	setsockopt(m_hClientSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
	setsockopt(m_hClientSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));
	setsockopt(m_hClientSocket, SOL_SOCKET, SO_RCVBUF, (char *)&sizeOfLanBuf, sizeof(sizeOfLanBuf));

	memset(&stServerAddress, 0, sizeof(stServerAddress));
	stServerAddress.sin_family = AF_INET;
	stServerAddress.sin_addr.s_addr = inet_addr(i_pacIP);
	stServerAddress.sin_port = htons(i_iPort);

	if(SOCKET_ERROR == connect(m_hClientSocket, (SOCKADDR*)&stServerAddress, sizeof(stServerAddress)))
	{
		closesocket(m_hClientSocket);
		TRACE("TCP_CLIENT_ERROR_CONNECT\n"); 
		return TCP_CLIENT_ERROR_CONNECT;
	}
	
	CGRCommObj::GetInstance().StartTcpClientADSBThreadFunc(&m_hClientSocket);
	CGRCommObj::GetInstance().StartTcpPacketProcessingADSBThreadFunc(NULL);
	CGRCommObj::GetInstance().SetConnectionInfo(PING_TIMER_SERVER, true);
	
	return true;
}

// - [이형호 송신]
int CGRCommIF::SendNEXSANData(int i_iSize, const void *i_pvData)
{	
	int iRet = NULL;
	if ( m_hClientSocket )
		iRet = send(m_hClientSocket, (const char *)i_pvData, i_iSize, 0);

	return iRet;
}

bool CGRCommIF::CreateServer(int i_iPort)
{
	int iRet = m_cCommMngr.CreateTCPServer(i_iPort);

	switch (iRet)
	{
	case TCP_SERVER_ERROR_SOCKET:		
		{
			return false;
		}	

	case TCP_SERVER_ERROR_BIND:
		{
			return false;
		}		

	case TCP_SERVER_ERROR_LISTEN:
		{
			return false;
		}

	default: // 정상
		{			
			m_hServerSocket = iRet;
			m_bServer = true;
			m_hCommObj.SetSocketMode(SOCKET_SERVER);
			/*if ( i_iPort == DATA_LINK_CONNECT_PORT )
				m_hCommObj.SetDataLinkServerSocket(m_hServerSocket);*/
			return true;
		}
	}
}

//void CGRCommIF::ReturnPingRst(int i_iConnectType, bool i_bRst)
//{
//	char readBuf[100] = {0};
//	char *envini_path = (".\\..\\COMMON\\Config.ini");
//	CString	strPort;
//	int iPort = 0;
//
//	GetPrivateProfileString(("CLIENT"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
//	strPort.Format(("%s"), readBuf);
//	iPort = atoi(strPort);
//
//	GetPrivateProfileString(("CLIENT"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
//
//	if ( i_bRst == true ) // 연결
//	{			
//		if ( m_hCommObj.GetConnectionInfo(i_iConnectType) == false )
//		{
//			m_listConnectPort.clear();
//				
//			TRACE ("[PORT : %d,  재연결시도]\n", iPort);
//			Connect(iPort,readBuf);				
//		}
//	}
//	else // 단락
//	{
//		if ( m_hCommObj.GetConnectionInfo(i_iConnectType) == true )
//		{
//			TRACE ("[PORT : %d,  PING에 의한 연결해제 인지]\n", iPort);
//			m_hCommObj.SetConnectionInfo(i_iConnectType, false);
//		}
//	}
//}

void CALLBACK GRPingTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CGRCommIF* pThis = ((CGRCommIF*)lpParam);

	//pThis->m_PingThread.StartPing(NULL, PING_ID_HEARTBEAT);

	if ( pThis->m_bServer )
		pThis->m_PingThread.StartPing(NULL, PING_ID_HEARTBEAT_SVR);
	else
		pThis->m_PingThread.StartPing(NULL, PING_ID_HEARTBEAT_CLT);
}


void CGRCommIF::SetHeartbeatIP_1(char* i_pcBuf)
{
	memcpy(m_acHeartbeatIP_1, i_pcBuf, MAX_IP_STR_BYTE);
	m_PingThread.SetHeartbeatIP_1(m_acHeartbeatIP_1);
}

void CGRCommIF::SetHeartbeatPort_1(int i_iPort)
{
	m_iHeartbeatPort_1 = i_iPort;
}

void CGRCommIF::SetHeartbeatIP_2(char* i_pcBuf)
{
	memcpy(m_acHeartbeatIP_2, i_pcBuf, MAX_IP_STR_BYTE);
	m_PingThread.SetHeartbeatIP_2(m_acHeartbeatIP_2);
}

void CGRCommIF::SetHeartbeatPort_2(int i_iPort)
{
	m_iHeartbeatPort_2 = i_iPort;	
}

void CGRCommIF::SetHeartbeatIP_3(char* i_pcBuf)
{
	memcpy(m_acHeartbeatIP_3, i_pcBuf, MAX_IP_STR_BYTE);
	m_PingThread.SetHeartbeatIP_3(m_acHeartbeatIP_3);
}

void CGRCommIF::SetHeartbeatPort_3(int i_iPort)
{
	m_iHeartbeatPort_3 = i_iPort;
}

void CGRCommIF::SetHeartbeatIP_4(char* i_pcBuf)
{
	memcpy(m_acHeartbeatIP_4, i_pcBuf, MAX_IP_STR_BYTE);
	m_PingThread.SetHeartbeatIP_4(m_acHeartbeatIP_4);
}

void CGRCommIF::SetHeartbeatPort_4(int i_iPort)
{
	m_iHeartbeatPort_4 = i_iPort;	
}
void CGRCommIF::ReturnPingRst(int i_iConnectType, bool i_bRst, bool i_bSvr)
{
	bool bUseHeartbeat = false;
	bUseHeartbeat = m_hCommObj.GetInfoToUseHeartbeat();	

	STR_LOGMESSAGE stMsg;

	if ( bUseHeartbeat != false )
	{
		if ( i_bRst == true ) // 연결
		{			
			if ( m_hCommObj.GetConnectionInfo(i_iConnectType) == false )
			{
				if(i_bSvr == true)
				{
					if ( i_iConnectType == CLIENT_NO_1 )
					{
						TRACE ("[PORT : %d,  재연결시도]\n", m_iHeartbeatPort_1);
						Connect(m_iHeartbeatPort_1, m_acHeartbeatIP_1, CLIENT_NO_1);
					}	
				}
				else
				{
					if ( i_iConnectType == CLIENT_NO_1 && m_acHeartbeatIP_1[0] != NULL )
					{
						sprintf( stMsg.szContents, "[IP : %s, PORT : %d,  재연결시도]" , m_acHeartbeatIP_1, m_iHeartbeatPort_1 );
						//TRACE ( "[IP : %s, PORT : %d,  재연결시도]" , m_acHeartbeatIP_1, m_iHeartbeatPort_1 );
						::SendMessage( g_DlgHandle, UWM_USER_MSG, (WPARAM) enSYSTEM, (LPARAM) & stMsg.szContents[0] );
						//Log( "시스템" , "재연결 시도" );
						Connect(m_iHeartbeatPort_1, m_acHeartbeatIP_1, CLIENT_NO_1);
					}
					else if ( i_iConnectType == CLIENT_NO_2 && m_acHeartbeatIP_2[0] != NULL )
					{
						sprintf( stMsg.szContents, "[IP : %s, PORT : %d,  재연결시도]" , m_acHeartbeatIP_2, m_iHeartbeatPort_2 );
						//TRACE ("[IP : %s, PORT : %d,  재연결시도]\n", m_acHeartbeatIP_2, m_iHeartbeatPort_2);
						::SendMessage( g_DlgHandle, UWM_USER_MSG, (WPARAM) enSYSTEM, (LPARAM) & stMsg.szContents[0] );
						Connect(m_iHeartbeatPort_2, m_acHeartbeatIP_2, CLIENT_NO_2);
					}
					else if ( i_iConnectType == CLIENT_NO_3 && m_acHeartbeatIP_3[0] != NULL )
					{
						sprintf( stMsg.szContents, "[IP : %s, PORT : %d,  재연결시도]" , m_acHeartbeatIP_3, m_iHeartbeatPort_3 );
						//TRACE ("[IP : %s, PORT : %d,  재연결시도]\n", m_acHeartbeatIP_3, m_iHeartbeatPort_3);
						::SendMessage( g_DlgHandle, UWM_USER_MSG, (WPARAM) enSYSTEM, (LPARAM) & stMsg.szContents[0] );
						m_hCommObj.SetConnectionInfo(CLIENT_NO_3, true);
						//Connect(m_iHeartbeatPort_3, m_acHeartbeatIP_3, CLIENT_NO_3);
					}
					else if  ( i_iConnectType == CLIENT_NO_4 && m_acHeartbeatIP_4[0] != NULL )
					{
						sprintf( stMsg.szContents, "[IP : %s, PORT : %d,  재연결시도]" , m_acHeartbeatIP_4, m_iHeartbeatPort_4 );
						//TRACE ("[IP : %s, PORT : %d,  재연결시도]\n", m_acHeartbeatIP_4, m_iHeartbeatPort_4);
						::SendMessage( g_DlgHandle, UWM_USER_MSG, (WPARAM) enSYSTEM, (LPARAM) & stMsg.szContents[0] );
						Connect(m_iHeartbeatPort_4, m_acHeartbeatIP_4, CLIENT_NO_4);
					}
					else;
				}						
			}
		}
		else // 단락
		{
			if ( m_hCommObj.GetConnectionInfo(i_iConnectType) == true )
			{	
				if(i_bSvr == true)
				{
					if ( i_iConnectType == CLIENT_NO_1 )
					{
						TRACE ("[PORT : %d,  PING에 의한 연결해제 인지]\n", m_iHeartbeatPort_1);
					}		
				}
				else
				{
					if ( i_iConnectType == CLIENT_NO_1 )
					{
						TRACE ("[PORT : %d,  재연결시도]\n", m_iHeartbeatPort_1);
						Connect(m_iHeartbeatPort_1, m_acHeartbeatIP_1, CLIENT_NO_1);
					}
					else if ( i_iConnectType == CLIENT_NO_2 )
					{
						TRACE ("[PORT : %d,  재연결시도]\n", m_iHeartbeatPort_2);
						Connect(m_iHeartbeatPort_2, m_acHeartbeatIP_2, CLIENT_NO_2);
					}
					else if ( i_iConnectType == CLIENT_NO_3 )
					{
						TRACE ("[PORT : %d,  재연결시도]\n", m_iHeartbeatPort_3);
						m_hCommObj.SetConnectionInfo(CLIENT_NO_3, false);
						//Connect(m_iHeartbeatPort_3, m_acHeartbeatIP_3, CLIENT_NO_3);
					}
					else if  ( i_iConnectType == CLIENT_NO_4 )
					{
						TRACE ("[PORT : %d,  재연결시도]\n", m_iHeartbeatPort_4);
						Connect(m_iHeartbeatPort_4, m_acHeartbeatIP_4, CLIENT_NO_4);
					}
					else;
				}						

				m_hCommObj.SetConnectionInfo(i_iConnectType, false);
			}
		}
	}
}

void CGRCommIF::SetServerID(int i_iServerID)
{
	m_hCommObj.SetServerID(i_iServerID);
}

int CGRCommIF::GetServerID()
{
	return m_hCommObj.GetServerID();
}

bool CGRCommIF::GetPDWCHCorrectConnStatus()
{
	return m_hCommObj.GetPDWCHCorretConnStatus();
}

bool CGRCommIF::GetPDWEquipConnStatus()
{
	return m_hCommObj.GetPDWConnStatus();
}

bool CGRCommIF::GetRadarEquipConnStatus()
{
	return m_hCommObj.GetRadarConnStatus();
}

