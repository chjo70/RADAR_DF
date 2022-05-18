#include "stdafx.h"
#include "ICAAMngr.h"
#include "MsgQueueThread.h"
#include "ThreadTask/DFMsgDefnt.h"

CICAAMngr* CICAAMngr::uniqueInstance=nullptr;

CICAAMngr::CICAAMngr()
:m_hCommIF_ICAAMngr(m_hCommIF)
{
	// ini파일에서 Server/Client 관련 정보 얻어오기
	char readBuf[100] = {0};
	char *envini_path = (".\\config.ini");
	CString	cstrServerClient = CString("");
	GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("MODE"), NULL, readBuf, _countof(readBuf), envini_path);
	cstrServerClient.Format(("%s"), readBuf);	// SERVER or CLIENT
	CString	strPort = CString("");

	if(strcmp(cstrServerClient, "SERVER") == 0)
	{
		int iPort = 0;
		TRACE("서버 선택\n");
		m_hCommIF.SetServerID(SERVER_ID_LINK_1);
		GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("PORT1"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.CreateServer(iPort);
	}
	else if(strcmp(cstrServerClient, "ADSBD") == 0)
	{
		int iPort = 0;
		TRACE("서버 선택\n");
		m_hCommIF.SetServerID(SERVER_ID_LINK_1);
		GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("PORT1"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.CreateServer(iPort);

		// 이 부분을 ADSB 부분으로 수정해야 함
		//RADAR 방탐기 OR RADAR 방탐 데몬
		GetPrivateProfileString(("ADSBD"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_1(iPort);
		GetPrivateProfileString(("ADSBD"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_1(readBuf);	
	}
	else if(strcmp(cstrServerClient, "CLIENT") == 0)
	{
		int iPort = 0;

		// 레이더분석 연결 /////////////////////////////////////////////////////////////////////////////////
		TRACE("클라이언트 선택\n");
		GetPrivateProfileString(("CLIENT1"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_1(iPort);
		GetPrivateProfileString(("CLIENT1"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_1(readBuf);


		// PDW발생판 연결 /////////////////////////////////////////////////////////////////////////////////
		GetPrivateProfileString(("CLIENT2"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_2(iPort);
		GetPrivateProfileString(("CLIENT2"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_2(readBuf);


		// 레이더방탐 장비 (제어명령) /////////////////////////////////////////////////////////////////////////////////
		GetPrivateProfileString(("CLIENT3"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_3(iPort);
		GetPrivateProfileString(("CLIENT3"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_3(readBuf);

		// V/UHF 장비 (제어명령) /////////////////////////////////////////////////////////////////////////////////
		GetPrivateProfileString(("CLIENT4"), ("PORT"), NULL, readBuf, _countof(readBuf), envini_path);
		strPort.Format(("%s"), readBuf);
		iPort = _ttoi(strPort);
		m_hCommIF.SetHeartbeatPort_4(iPort);
		GetPrivateProfileString(("CLIENT4"), ("IP"), NULL, readBuf, _countof(readBuf), envini_path);
		m_hCommIF.SetHeartbeatIP_4(readBuf);
		// 운용PC (Only 스펙트럼 전송) /////////////////////////////////////////////////////////////////////////////////
	}
	else
	{

	}
}

CICAAMngr::~CICAAMngr()
{
	
}

CICAAMngr* CICAAMngr::GetInstance()
{
	if (uniqueInstance == nullptr)
	{
		uniqueInstance = new CICAAMngr();		
	}

	return uniqueInstance;
}

void CICAAMngr::Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData)
{
	TRACE("[Log] Message is received.\n");

	// 수신단에서 Msg 데이터 형성
	
	// 쓰레드 처리를 위해 Msg Queue에 집어넣기
}

void CICAAMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE별로 경우를 나열하여 처리하는 로직 구현 필요
	// 하단에 구현

}