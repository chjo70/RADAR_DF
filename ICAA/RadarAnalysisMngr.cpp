#include "stdafx.h"
#include "RadarAnalysisMngr.h"
#include "ThreadTask/DFMsgDefnt.h"

// 운용-레이더분석 OPCODE
#define OPCODE_OV_BD_REQINIT		0x01020100		// 운용-레이더분석 초기화 요구
#define OPCODE_OV_BD_REQSWVER		0x01020200		// 운용-레이더분석 SW버전 요구
#define OPCODE_OV_BD_REQPBIT		0x01020300		// 운용-레이더분석 초기자체점검 요구
#define OPCODE_OV_BD_REQIBIT		0x01020400		// 운용-레이더분석 운용자자체점검 요구
#define OPCODE_OV_BD_REQCHCALIB		0x01020500		// 운용-레이더분석 채널 보정 요구

#define OPCODE_OV_BD_REQSTARTACQ	0x01021100		// 운용-레이더분석 수집시작 요구
#define OPCODE_OV_BD_SETRESET		0x01021200		// 운용-레이더분석 장비 리셋 요구
#define OPCODE_OV_BD_CONTROLRECVR	0x01021300		// 운용-레이더분석 수신기 제어
#define OPCODE_OV_BD_REQSYSTEMSET	0x01021400		// 운용-레이더분석 레이더방탐 시스템변수 설정 요구
#define OPCODE_OV_BD_REQCHANGEEMIT	0x01021600		// 운용-레이더분석 레이더/위협 변경 요구

#define OPCODE_OV_BD_REQACQTASK		0x01022100		// 운용-레이더분석 수집 과제 요구
#define OPCODE_OV_BD_REQREJFREQSET	0x01022200		// 운용-레이더분석 수신배제주파수 설정 요구
#define OPCODE_OV_BD_SETSYSTEM		0x01022300		// 운용-레이더분석 시스테 설정

#define OPCODE_OV_BD_SENDERROR		0x0102F100		// 운용-레이더분석 에러 전송

// 레이더분석-운용 OPCODE
#define OPCODE_BD_OV_RSTINIT		0x02010100		// 레이더분석-운용 초기화 결과
#define OPCODE_BD_OV_RSTSWVER		0x02010200		// 레이더분석-운용 SW버전 결과
#define OPCODE_BD_OV_RSTPBIT		0x02010300		// 레이더분석-운용 초기자체점검 결과
#define OPCODE_BD_OV_RSTIBIT		0x02010400		// 레이더분석-운용 운용자자체점검 결과
#define OPCODE_BD_OV_STATECHCALIB	0x02010500		// 레이더분석-운용 채널 진행 상태

#define OPCODE_BD_OV_RSTSTARTACQ	0x02011100		// 레이더분석-운용 수집시작 결과
#define OPCODE_BD_OV_RSTRESETEQUIP	0x02011200		// 레이더분석-운용 장비 리셋 결과
#define OPCODE_BD_OV_RSTRECVCONTROL	0x02011300		// 레이더분석-운용 수신기 제어 결과
#define OPCODE_BD_OV_RSTSYSTEMSET	0x02011400		// 레이더분석-운용 레이더방탐 시스템변수 설정 결과
#define OPCODE_BD_OV_RSTCHANGEEMIT	0x02011600		// 레이더분석-운용 레이더/위협 변경 결과
#define OPCODE_BD_OV_STATETASK		0x02011700		// 레이더분석-운용 과제 수행 상태

#define OPCODE_BD_OV_RSTACQTASK		0x02012100		// 레이더분석-운용 수집과제 수신결과
#define OPCODE_BD_OV_SENDLOB		0x02013100		// 레이더분석-운용 LOB 전송
#define OPCODE_BD_OV_SENDBEAM		0x02013300		// 레이더분석-운용 빔 전송

#define OPCODE_BD_OV_SENDERROR		0x0201F100		// 레이더분석-운용 에러 전송

// 레이더분석-레이더방탐 OPCODE
#define OPCODE_BD_TF_REQINIT		0x02820100		// 레이더분석-레이더방탐 초기화 요구
#define OPCODE_BD_TF_REQSWVER		0x02820200		// 레이더분석-레이더방탐 SW버전 요구
#define OPCODE_BD_TF_REQPBIT		0x02820300		// 레이더분석-레이더방탐 초기자체점검 요구
#define OPCODE_BD_TF_REQIBIT		0x02820400		// 레이더분석-레이더방탐 운용자자체점검 요구
#define OPCODE_BD_TF_REQCHCALIB		0x02820500		// 레이더분석-레이더방탐 채널 보정 요구

#define OPCODE_BD_TF_REQSTARTACQ	0x02821100		// 레이더분석-레이더방탐 수집시작 요구
#define OPCODE_BD_TF_SETRESET		0x02821200		// 레이더분석-레이더방탐 장비 리셋 요구
#define OPCODE_BD_TF_CONTROLRECVR	0x02821300		// 레이더분석-레이더방탐 수신기 제어
#define OPCODE_BD_TF_REQSYSTEMSET	0x02821400		// 레이더분석-레이더방탐 레이더방탐 시스템변수 설정 요구

#define OPCODE_BD_TF_REQACQTASK		0x02822100		// 레이더분석-레이더방탐 수집 과제 요구
#define OPCODE_BD_TF_REQREJFREQSET	0x02822200		// 레이더분석-레이더방탐 수신배제주파수 설정 요구
#define OPCODE_BD_TF_SETSYSTEM		0x02822300		// 레이더분석-레이더방탐 시스테 설정

#define OPCODE_BD_TF_SENDERROR		0x0282F100		// 레이더분석-레이더방탐 에러 전송

// 레이더방탐-레이더분석 OPCODE
#define OPCODE_TF_BD_RSTINIT		0x82020100		// 레이더방탐-레이더분석 초기화 결과
#define OPCODE_TF_BD_RSTSWVER		0x82020200		// 레이더방탐-레이더분석 SW버전 결과
#define OPCODE_TF_BD_RSTPBIT		0x82020300		// 레이더방탐-레이더분석 초기자체점검 결과
#define OPCODE_TF_BD_RSTIBIT		0x82020400		// 레이더방탐-레이더분석 운용자자체점검 결과
#define OPCODE_TF_BD_STATECHCALIB	0x82020500		// 레이더방탐-레이더분석 채널 진행 상태

#define OPCODE_TF_BD_RSTSTARTACQ	0x82021100		// 레이더방탐-레이더분석 수집시작 결과
#define OPCODE_TF_BD_RSTRESETEQUIP	0x82021200		// 레이더방탐-레이더분석 장비 리셋 결과
#define OPCODE_TF_BD_RSTRECVCONTROL	0x82021300		// 레이더방탐-레이더분석 수신기 제어 결과
#define OPCODE_TF_BD_RSTSYSTEMSET	0x82021400		// 레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과
#define OPCODE_TF_BD_STATETASK		0x82021700		// 레이더방탐-레이더분석 과제 수행 상태
#define OPCODE_TF_BD_RSTACQTASK		0x82022100		// 레이더방탐-레이더분석 수집과제 수신결과

#define OPCODE_TF_BD_SENDLOB		0x82023100		// 레이더방탐-레이더분석 LOB 전송
#define OPCODE_TF_BD_SENDPDW		0x82023200		// 레이더방탐-레이더분석 PDW 전송

#define OPCODE_TF_BD_SENDERROR		0x8202F100		// 레이더방탐-레이더분석 에러 전송



CRadarAnalysisMngr* CRadarAnalysisMngr::uniqueInstance=nullptr;

CRadarAnalysisMngr::CRadarAnalysisMngr()
:m_hCommIF_RAMngr(m_hCommIF)
{
	char readBuf[100] = {0};
	char *envini_path = ("..\\ICAA\\config.ini");
	CString	cstrServerClient = CString("");
	GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("MODE"), NULL, readBuf, _countof(readBuf), envini_path);
	cstrServerClient.Format(("%s"), readBuf);	// SERVER or CLIENT
	CString	strPort = CString("");

	if(strcmp(cstrServerClient, "SERVER") == 0)
	{
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQINIT, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQSWVER, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQPBIT, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQIBIT, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQCHCALIB, this);		
				
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQSTARTACQ, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_SETRESET, this);		
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_CONTROLRECVR, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQSYSTEMSET, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQCHANGEEMIT, this);

		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQACQTASK, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_REQREJFREQSET, this);
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_SETSYSTEM, this);
	
		m_hCommIF.RegisterOpcode(OPCODE_OV_BD_SENDERROR, this);
			
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTINIT, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTSWVER, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTPBIT, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTIBIT, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_STATECHCALIB, this);

		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTSTARTACQ, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTRESETEQUIP, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTRECVCONTROL, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_RSTSYSTEMSET, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_STATETASK, this);

		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_SENDLOB, this);
		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_SENDPDW, this);

		m_hCommIF.RegisterOpcode(OPCODE_TF_BD_SENDERROR, this);

		m_OVIP = GetLastIP();
		// 시작시 한번만 호출하면 됩니다.
		RadarAnlAlgotirhm::RadarAnlAlgotirhm::Init();
	}
}

CRadarAnalysisMngr::~CRadarAnalysisMngr()
{
	char readBuf[100] = {0};
	char *envini_path = ("..\\ICAA\\config.ini");
	CString	cstrServerClient = CString("");
	GetPrivateProfileString(("SERVER/CLIENT/ADSBD"), ("MODE"), NULL, readBuf, _countof(readBuf), envini_path);
	cstrServerClient.Format(("%s"), readBuf);	// SERVER or CLIENT
	CString	strPort = CString("");

	if(strcmp(cstrServerClient, "SERVER") == 0)
	{
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQINIT, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQSWVER, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQPBIT, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQIBIT, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQCHCALIB, this);		

		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQSTARTACQ, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_SETRESET, this);		
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_CONTROLRECVR, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQSYSTEMSET, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQCHANGEEMIT, this);

		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQACQTASK, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_REQREJFREQSET, this);
		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_SETSYSTEM, this);

		m_hCommIF.UnregisterOpcode(OPCODE_OV_BD_SENDERROR, this);

		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTINIT, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTSWVER, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTPBIT, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTIBIT, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_STATECHCALIB, this);

		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTSTARTACQ, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTRESETEQUIP, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTRECVCONTROL, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_RSTSYSTEMSET, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_STATETASK, this);

		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_SENDLOB, this);
		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_SENDPDW, this);

		m_hCommIF.UnregisterOpcode(OPCODE_TF_BD_SENDERROR, this);

		// 프로그램 종료시 한번만 호출하면 됩니다.
		RadarAnlAlgotirhm::RadarAnlAlgotirhm::Close();
	}
}

CRadarAnalysisMngr* CRadarAnalysisMngr::GetInstance()
{
	if (uniqueInstance == nullptr)
	{
		uniqueInstance = new CRadarAnalysisMngr();		
	}

	return uniqueInstance;
}

void CRadarAnalysisMngr::Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData)
{
	TRACE("[Log] Message is received.\n");

	// 수신단에서 Msg 데이터 형성
	STMsg stInstanceMsg;

	stInstanceMsg.uiOpcode = i_uiOpcode;
	stInstanceMsg.usMSize = i_usSize;
	stInstanceMsg.ucRevOperID = i_ucRevOperID;
	stInstanceMsg.ucLinkID = i_ucLink;
	stInstanceMsg.ucOperatorID = ucOpertorID;

	memcpy(stInstanceMsg.buf, i_pvData, i_usSize);

	// 쓰레드 처리를 위해 Msg Queue에 집어넣기
	PushMsg(stInstanceMsg); 

	m_cond.LIGSetEvent();
}

void CRadarAnalysisMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE별로 경우를 나열하여 처리하는 로직 구현 필요
	// 하단에 구현
	bool bRtnSend = false;

	switch(i_stMsg.uiOpcode)
	{
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// 운용 To 레이더분석
	case OPCODE_OV_BD_REQINIT:		// 운용-레이더분석 초기화 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 초기화 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQINIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 초기화 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 초기화 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQSWVER:		// 운용-레이더분석 SW버전 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 SW버전 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSWVER, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 SW버전 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 SW버전 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQPBIT:		// 운용-레이더분석 초기자체점검 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 초기자체점검 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQPBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 초기자체점검 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 초기자체점검 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQIBIT:		// 운용-레이더분석 운용자자체점검 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 운용자자체점검 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQIBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 운용자자체점검 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 운용자자체점검 요구\n");
			}
		}
		break;
	
	case OPCODE_OV_BD_REQCHCALIB:		// 운용-레이더분석 채널 보정 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 채널 보정 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQCHCALIB, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 채널 보정 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 채널 보정 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQSTARTACQ:	// 운용-레이더분석 수집시작 요구
		{
			TRACE("[수신]운용-레이더분석 수집시작 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSTARTACQ, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 수집시작 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 수집시작 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_SETRESET:		// 운용-레이더분석 장비 리셋 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 장비 리셋 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETRESET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 장비 리셋 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 장비 리셋 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_CONTROLRECVR:		// 운용-레이더분석 수신기 제어
		{
			TRACE("[수신]레이더분석-레이더방탐 수신기 제어\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_CONTROLRECVR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 수신기 제어\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 수신기 제어\n");
			}
		}
		break;
	
	case OPCODE_OV_BD_REQSYSTEMSET:		// 운용-레이더분석 시스템변수 설정 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 시스템변수 설정 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSYSTEMSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집시작 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 시스템변수 설정 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 시스템변수 설정 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQCHANGEEMIT:	// 운용-레이더분석 레이더/위협 변경 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 레이더/위협 변경 요구\n");

			// 레이더분석-레이더방탐 레이더/위협 변경 요구 관련 ICD 메시지가 없음
		}
		break;

	case OPCODE_OV_BD_REQACQTASK:	// 운용-레이더분석 수집 과제 요구
		{
			TRACE("[수신]운용-레이더분석 수집 과제 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQACQTASK, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집 과제 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 수집 과제 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 수집 과제 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQREJFREQSET:	// 운용-레이더분석 수신배제주파수 설정 요구
		{
			TRACE("[수신]운용-레이더분석 수신배제주파수 설정 요구\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQREJFREQSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 수집 과제 요구

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 수신배제주파수 설정 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 수신배제주파수 설정 요구\n");
			}
		}
		break;

	case OPCODE_OV_BD_SETSYSTEM:	// 운용-레이더분석 시스템 설정
		{
			TRACE("[수신]운용-레이더분석 시스템 설정\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETSYSTEM, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 시스템 설정

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 시스템 설정\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 시스템 설정\n");
			}
		}
		break;

	case OPCODE_OV_BD_SENDERROR:	// 운용-레이더분석 에러 전송
		{
			TRACE("[수신]운용-레이더분석 에러 전송\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SENDERROR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-레이더방탐 시스템 설정

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-레이더방탐 에러 전송\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-레이더방탐 에러 전송\n");
			}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// 레이더방탐 To 레이더분석
	case OPCODE_TF_BD_RSTINIT:	// 레이더방탐-레이더분석 초기화 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 수집시작 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 초기화 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 초기화 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSWVER:	// 레이더방탐-레이더분석 SW버전 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 SW버전 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 SW버전 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 SW버전 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTPBIT:	// 레이더방탐-레이더분석 초기자체점검 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 초기자체점검 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 초기자체점검 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 초기자체점검 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTIBIT:	// 레이더방탐-레이더분석 운용자자체점검 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 운용자자체점검 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTIBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 운용자자체점검 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 운용자자체점검 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_STATECHCALIB:	// 레이더방탐-레이더분석 채널 보정 진행 상태
		{
			TRACE("[수신]레이더방탐-레이더분석 채널 보정 진행 상태\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_STATECHCALIB, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 채널 보정 진행 상태\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 채널 보정 진행 상태\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSTARTACQ:	// 레이더방탐-레이더분석 수집시작 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 수집시작 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSTARTACQ, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 수집시작 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 수집시작 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTRESETEQUIP:	// 레이더방탐-레이더분석 장비 리셋 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 장비 리셋 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTRESETEQUIP, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 장비 리셋 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 장비 리셋 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTRECVCONTROL:	// 레이더방탐-레이더분석 수신기 제어 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 수신기 제어 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTRECVCONTROL, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 수신기 제어 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 수신기 제어 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSYSTEMSET:	// 레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과
		{
			TRACE("[수신]레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSYSTEMSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 레이더방탐 시스템변수 설정 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 레이더방탐 시스템변수 설정 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_STATETASK:	// 레이더방탐-레이더분석 과제 수행 상태
		{
			TRACE("[수신]레이더방탐-레이더분석 과제 수행 상태\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_STATETASK, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 과제 수행 상태\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 과제 수행 상태\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTACQTASK:
		{
			TRACE("[수신]레이더방탐-레이더분석 수집과제 수신 결과\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTACQTASK, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 수집과제 수신 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 수집과제 수신 결과\n");
			}
		}
		break;

	case OPCODE_TF_BD_SENDLOB:	// 레이더방탐-레이더분석 LOB 전송
		{
			TRACE("[수신]레이더방탐-레이더분석 LOB 전송\n");

			//LOB 데이터 전송
			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDLOB, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과
			
			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 LOB 전송\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 LOB 전송\n");
			}

			// 알고리즘 수행 후 BEAM 생성하여 BEAM 정보 전송하는 루틴 추가
			///////////////////////////////////////////////////////////////////////////////
			//조철희 수석님의 LOB데이타에서 빔 추출 알고리즘 호출			

			STR_LOBDATA stLOBData;
			memcpy(& stLOBData, i_stMsg.buf, i_stMsg.usMSize);
			STR_ABTDATA *pABTData, txABTData;
			RadarAnlAlgotirhm::RadarAnlAlgotirhm::Start(& stLOBData);
			RadarAnlAlgotirhm::RadarAnlAlgotirhm::UpdateCEDEOBLibrary();
			RadarAnlAlgotirhm::RadarAnlAlgotirhm::GetABTData(pABTData);

			memcpy(&txABTData, pABTData, sizeof(txABTData));
			///////////////////////////////////////////////////////////////////////////////
			
			//빔 데이터 전송
			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDBEAM, sizeof(txABTData), i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, pABTData);	
		}
		break;

	case OPCODE_TF_BD_SENDPDW:	// 레이더방탐-레이더분석 PDW 전송
		{
			TRACE("[수신]레이더방탐-레이더분석 PDW 전송\n");

			// 레이더분석-운용 PDW 전송은 ICD 메시지 정의 안되어 있음
		}
		break;

	case OPCODE_TF_BD_SENDERROR:	// 레이더방탐-레이더분석 에러 전송
		{
			TRACE("[수신]레이더방탐-레이더분석 에러 전송\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDERROR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// 레이더분석-운용 수집시작 결과

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더분석-운용 에러 전송\n");
			}
			else
			{
				TRACE("[송신 실패]레이더분석-운용 에러 전송\n");
			}
		}
		break;

	default:
		break;
	}
}

unsigned char CRadarAnalysisMngr::GetLastIP()
{
	unsigned char ucLastIP = NULL;

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
		ucLastIP = (unsigned char)iIp;
	}	 
	//////////////////////////////////////////////////////////////////////////////////////////

	return ucLastIP;
}