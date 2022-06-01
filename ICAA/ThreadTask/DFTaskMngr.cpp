#include "stdafx.h"
#include "DFTaskMngr.h"
#include "..\MsgQueueThread.h"
#include "..\ThreadTask/DFMsgDefnt.h"
#include "..\DFData.h"
#include <stdint.h>

#include <stdint.h>







#define ROUNDING(x, dig) (floor((x) *pow(float(10), dig) + 0.5f) /pow(float(10), dig))
#define CSV_BUF				30
#define PH_DIFF				1.40625
//#define PH_DIFF				1.0
#define TASK_ACQTIME		1000000
#define TASK_RQADDCNT		5
#define IQ_PACKET_SIZE    16384

//LMS_ADD_20190704
//_stBigArray* pstBigArray; //LMS_ADD_20171221
_stBigArray* arrpBigArray[ANT_PATH_MAX]; //LMS_ADD_20171221

CDFTaskMngr* CDFTaskMngr::m_pInstance=nullptr;
void CALLBACK GRColectStatusTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void CALLBACK PDWConnStatusTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void CALLBACK ReqTaskRetryTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired);


// 레이더분석-레이더방탐 OPCODE //Receive
#define OPCODE_BD_TF_REQCHCALIB		0x02820500		// 레이더분석-레이더방탐 채널 보정 요구
#define OPCODE_BD_TF_REQCHCALIBCHECK		0x02820600		// 레이더분석-레이더방탐 채널 보정 필요 상태 확인 요구

#define OPCODE_BD_TF_REQSTARTACQ	0x02821100		// 레이더분석-레이더방탐 수집시작 요구
#define OPCODE_BD_TF_SETRESET		0x02821200		// 레이더분석-레이더방탐 장비 리셋 요구
#define OPCODE_BD_TF_CONTROLRECVR	0x02821300		// 레이더분석-레이더방탐 수신기 제어
#define OPCODE_BD_TF_REQSYSTEMSET	0x02821400		// 레이더분석-레이더방탐 레이더방탐 시스템변수 설정 요구
#define OPCODE_BD_TF_REQCHANGEEMIT	0x02821600		// 레이더분석-레이더방탐 레이더/위협 변경 요구

#define OPCODE_BD_TF_REQACQTASK		0x02822100		// 레이더분석-레이더방탐 수집 과제 요구
#define OPCODE_BD_TF_REQREJFREQSET	0x02822200		// 레이더분석-레이더방탐 수신배제주파수 설정 요구
#define OPCODE_BD_TF_SETSYSTEM		0x02822300		// 레이더분석-레이더방탐 시스테 설정
#define OPCODE_BD_TF_REQNEXTTASK	0x02822400		// 레이더분석-레이더방탐 수집 다음 과제 요구
#define OPCODE_BD_TF_REQIQDATA		0x02823500		// 레이더분석-레이더방탐 IQ 시작/중지 요구
#define OPCODE_BD_TF_SENDERROR		0x0282F100		// 레이더분석-레이더방탐 에러 전송


// 레이더방탐-레이더분석 OPCODE //Send
#define OPCODE_TF_BD_RSTINIT		0x82020100		// 레이더방탐-레이더분석 초기화 결과

#define OPCODE_TF_BD_STATECHCALIB	0x82020500		// 레이더방탐-레이더분석 채널 진행 상태(보정)
#define OPCODE_TF_BD_RSTCHCALIBCHECK	0x82020600		// 레이더방탐-레이더분석 채널 보정 필요 상태 결과

#define OPCODE_TF_BD_RSTSTARTACQ	0x82021100		// 레이더방탐-레이더분석 수집시작 결과
#define OPCODE_TF_BD_RSTRESETEQUIP	0x82021200		// 레이더방탐-레이더분석 장비 리셋 결과
#define OPCODE_TF_BD_RSTRECVCONTROL	0x82021300		// 레이더방탐-레이더분석 수신기 제어 결과
#define OPCODE_TF_BD_RSTSYSTEMSET	0x82021400		// 레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과
#define OPCODE_TF_BD_RSTCHANGEEMIT	0x82021600		// 레이더방탐-레이더분석 레이더/위협 변경 결과

#define OPCODE_TF_BD_STATETASK		0x82021700		// 레이더방탐-레이더분석 과제 수행 상태
#define OPCODE_TF_BD_RSTACQTASK		0x82022100		// 레이더방탐-레이더분석 수집과제 수신결과
#define OPCODE_TF_BD_RSTREJFREQSET	0x02822200		// 레이더분석-레이더방탐 수신배제주파수 수신결과

#define OPCODE_TF_BD_SENDLOB		0x82023100		// 레이더방탐-레이더분석 LOB 전송
#define OPCODE_TF_BD_SENDPDW		0x82023200		// 레이더방탐-레이더분석 PDW 전송
#define OPCODE_TF_BD_SENDDESITY		0x82023600		// 레이더방탐-레이더분석 혼잡도 전송

#define OPCODE_TF_BD_SENDERROR		0x8202F100		// 레이더방탐-레이더분석 에러 전송


// 레이더방탐-PDW OPCODE //Send
#define OPCODE_TF_DP_REQCHCALIBCHECK		0x82840600		// 레이더방탐-PDW 채널 보정 필요 상태 확인 요구
#define OPCODE_TF_DP_DONECHCALIBCHECK		0x82840700		// 레이더방탐-PDW 채널 보정 완료 메시지
#define OPCODE_TF_DP_ACQSET			0x82842100		// 레이더방탐-PDW 수집설정
#define OPCODE_TF_DP_SYSSET			0x82841400		// 레이더방탐-PDW 시스템설정

#define OPCODE_TF_DP_REQPDW			0x82843200		// 레이더방탐-PDW PDW 전송 요청
#define OPCODE_TF_DP_REQSPTRUM		0x82842300		// 레이더방탐-PDW 스펙트럼 전송 요청
#define OPCODE_TF_DP_REQSIQ			0x82843500		// 레이더방탐-PDW IQ 전송 요청



// PDW-레이더방탐 OPCODE //Receive
#define OPCODE_DP_TF_RSTCHCALIBCHECK	0x84820600		// 레이더방탐-PDW 채널 보정 필요 상태 결과
#define OPCODE_DP_TF_ACQACK				0x84822100		// PDW-레이더방탐 수집설정 ACK
#define OPCODE_DP_TF_ACQDONESTATUS		0x84823300		// PDW-레이더방탐 채널 보정 수집완료 상태 전송
#define OPCODE_DP_TF_TASKACQDONESTATUS	0x84823400		// PDW-레이더방탐 과제 수집완료 상태 전송
#define OPCODE_DP_TF_RSTPDW				0x84823200		// PDW-레이더방탐 결과
#define OPCODE_DP_TF_RSTIQ				0x84823500		// IQ-레이더방탐 결과

#define AOA_OFFSET_1					27600-7500//27000 //28200
#define AOA_OFFSET_2					28800//28400 //33400
#define AOA_OFFSET_3					5700  //6700
#define AOA_BASE						36000

#define LOCAL_DATA_DIRECTORY			"C:\\Files\\IQDATA"
#define	IQ_EXT							"EIQ"


char g_szAetSignalType[7][3] = { "CW" , "NP" , "CW" , "FM" , "CF", "SH", "AL" };
char g_szAetFreqType[7][3] = { "F_" , "HP" , "RA" , "PA", "UK", "IF" };
char g_szAetPriType[7][3] = { "ST" , "JT", "DW" , "SG" , "PJ", "IP", "UK" } ;

CDFTaskMngr::CDFTaskMngr()
:m_hCommIF_DFTaskMngr(m_hCommIF)
,miAntPathDir(ANT_PATH_N)
{
	// ini파일에서 Server/Client 관련 정보 얻어오기
	//등록정보
	//시스템 설정 정보
	//과제 정보
	//과제 시작정보

	InitializeCriticalSection(&m_csIQFile);

	//m_hCommIF.RegisterOpcode(0x12400000, this); 
	m_LinkInfo = 0;
	m_POSNIP = GetLastIP();
	m_strAntFullFileName = _T("");
	

	//FOR TEST
	//m_LinkInfo = LINK1_ID;
	//m_strAntFullFileName.Format("C:\\IdexFreq\\LIG_MF_Data_no1.txt"); 
	//FOR TEST

	m_LinkInfo = LINK1_ID;
	m_strAntFullFileName.Format("C:\\IdexFreq\\LIG_MF_Data_no1.txt"); 
	m_iAOAOffset = -1240;
	m_stSystemVal.uiCollectorID = LINK1_ID;
	
	/*
	if(m_POSNIP == SYS_CLR_EQUIP1)
	{
		m_LinkInfo = LINK1_ID;
		m_strAntFullFileName.Format("C:\\IdexFreq\\LIG_MF_Data_no1.txt"); 
		m_iAOAOffset = AOA_OFFSET_1;
		m_stSystemVal.uiCollectorID = LINK1_ID;
	}
	else if(m_POSNIP == SYS_CLR_EQUIP2)
	{
		m_LinkInfo = LINK2_ID;
		m_strAntFullFileName.Format("C:\\IdexFreq\\LIG_MF_Data_no2.txt"); 
		m_iAOAOffset = AOA_OFFSET_2;
		m_stSystemVal.uiCollectorID = LINK2_ID;
	}
	else if(m_POSNIP == SYS_CLR_EQUIP3)
	{
		m_LinkInfo = LINK3_ID;
		m_strAntFullFileName.Format("C:\\IdexFreq\\LIG_MF_Data_no3.txt"); 
		m_iAOAOffset = AOA_OFFSET_3;
		m_stSystemVal.uiCollectorID = LINK3_ID;
	}
	else;
	*/

	//시스템 설정
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_SETSYSTEM, this);		

	//수집 다음 과제 요구
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQNEXTTASK, this);		

	//IQ 시작/중지 요구
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQIQDATA, this);	

	//수집과제 요구
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQACQTASK, this);	

	//과제 수집시작 요구	
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQSTARTACQ, this);	
	
	//채널 보정 요구
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQCHCALIB, this);

	//채널 보정 필요 상태 확인 요구
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQCHCALIBCHECK, this);

	//장비 리셋 요구
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_SETRESET, this);

	//수신기 제어
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_CONTROLRECVR, this);

	//레이더방탐 시스템변수 설정
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQSYSTEMSET, this);

	//레이더/위협 변경
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQCHANGEEMIT, this); 

	//수신배제주파수 설정
	m_hCommIF.RegisterOpcode(OPCODE_BD_TF_REQREJFREQSET, this); 

	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////PDW ///////////////////////////////////////////////////////
	//레이더방탐-PDW 채널 보정 필요 상태 결과
	m_hCommIF.RegisterOpcode(OPCODE_DP_TF_RSTCHCALIBCHECK, this); 
	
	//PDW 결과
	m_hCommIF.RegisterOpcode(OPCODE_DP_TF_RSTPDW, this); 
	
	//PDW 채널보정 수집완료 상태
	m_hCommIF.RegisterOpcode(OPCODE_DP_TF_ACQDONESTATUS, this); 

	//PDW 채널보정 과제 수집완료 상태
	m_hCommIF.RegisterOpcode(OPCODE_DP_TF_TASKACQDONESTATUS, this); 

	//IQ 결과
	m_hCommIF.RegisterOpcode(OPCODE_DP_TF_RSTIQ, this); 

	//PDW 수집설정 ACK
	m_hCommIF.RegisterOpcode(OPCODE_DP_TF_ACQACK, this); 


	//안테나 보정 데이터 저장버퍼
	for(int i = 0 ; i < ANT_PATH_MAX ; i++)
	{
		arrpBigArray[i] = (_stBigArray *)malloc(sizeof(struct _stBigArray)); //LMS_ADD_20171221
	}

	m_iMode = 0;
	m_Freq = 0;
	m_strFilePath_Chennel = _T("");
	m_pdwCntZero = 0;
	m_bisRetrySigz = false;
	m_iTaskCnt = 0;
	m_iNextTaskCnt = 0;
	m_iTotCoretFreqDataCnt = 0;
	m_iIncrCoretFreqCnt = 0;
	m_RetryCnt = 0;
	m_RetryAginCnt = 0;
	isRqTaskAcqSigFromPDW = false;
	m_bIsTaskStop = false;

	m_hRetryColectTimerQueue = NULL;
	m_hRetryColectStatTimer = NULL;

	m_hRqTaskRetryTimerQueue = NULL;
	m_hRqTaskRetryTimer = NULL;

	m_hPDWConStatTimer = NULL;
	m_hPDWConTimerQueue = NULL;

	m_strFilePath_Chennel.Format("C:\\IdexFreq\\CHData_Done.csv"); 

	//InitFileFromStartChCorrect();
	/*ofstream fout;
	fout.open(m_strFilePath_Chennel, ios::out | ios::binary | ios::trunc);
	fout.close();*/

	// 시작시 한번만 호출하면 됩니다.
	RadarDirAlgotirhm::RadarDirAlgotirhm::Init();
    m_uiOpInitID=RadarDirAlgotirhm::RadarDirAlgotirhm::GetOpInitID();

	//안테나보정데이터 로딩
	LoadDfCalRomDataPh();
	SetDFCrectFreqList();
	GetAutoFreqChCorrect(); //채널보정주파수 리스트 정보
	m_iTotCoretFreqDataCnt = m_listAutoFreqData.size();

	m_bEndChannelCorrect = false;
	//m_SetReqInitFlag = false;

	m_stSystemVal.uiRxThresholdMode = 0;
	m_stSystemVal.uiCWDecisionPW = 245000;
	m_stSystemVal.uiCWChoppinginterval = 245000;
	m_stSystemVal.uiSignalCheckTime = 1000;
	m_stSystemVal.uiSignalCheckPulseNumber = 1;
	m_stSystemVal.uiFilterType = 0;
	m_stSystemVal.uilsCWFilterApply = 0;
	m_stSystemVal.uilsFreqFilterApply = 0;
	m_stSystemVal.uiFilterMinFreq = 0;
	m_stSystemVal.uiFilterMaxFreq = 50000;
	m_stSystemVal.uilsPAFilterApply = 0;
	m_stSystemVal.iFilterMinPA = 0;
	m_stSystemVal.iFilterMaxPA = 50000;
	m_stSystemVal.uilsPWFilterApply = 0;
	m_stSystemVal.uiFilterMinPW = 0;
	m_stSystemVal.uiFilterMaxPW = 50000;

	m_RadarGain = 0;
	m_TaskUseStat = 0;
	m_nStep = 0;
	m_pIQData = NULL;

	hThread_IQFile = NULL;

	m_hKillIQFileEvent   = CreateEvent(NULL,TRUE,FALSE,NULL); 
	m_hSignalIQFileEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_dwID = 0;
	m_IQFileInit = true;	

	m_uiIQSize = 0;
	m_bChStep = false;

	memset(m_filename, 0, 100);
	memset(m_szDirectory, 0, 100);

	m_bTurnButten = false;

	g_RcvFunc.m_strIP = "127.0.0.1";
	//g_RcvFunc.m_strIP = "30.30.30.241";
	g_RcvFunc.m_iPort = 5050;
	//채널보정 시작
	//PDE발생판 연결상태 확인 후 호출
	//StartCheckPDWConTimer();
	//if(m_hCommIF.GetPDWConnStatus() == true)
	//StartChannelCorrect(MODE_INIT_TYPE);

	m_uiPDWID = 1;
}

CDFTaskMngr::~CDFTaskMngr()
{
	//시스템 설정
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_SETSYSTEM, this);		

	//수집 다음 과제 요구
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQNEXTTASK, this);

	//IQ 시작/중지 요구
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQIQDATA, this);	

	//수집과제 요구
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQACQTASK, this);	

	//과제 수집시작 요구	
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQSTARTACQ, this);	

	//채널 보정 요구
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQCHCALIB, this);

	//채널 보정 필요 상태 확인 요구
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQCHCALIBCHECK, this);

	//장비 리셋 요구
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_SETRESET, this);

	//수신기 제어
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_CONTROLRECVR, this);

	//레이더방탐 시스템변수 설정
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQSYSTEMSET, this);

	//레이더/위협 변경
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQCHANGEEMIT, this); 

	//수신배제주파수 설정
	m_hCommIF.UnregisterOpcode(OPCODE_BD_TF_REQREJFREQSET, this); 


	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////PDW ///////////////////////////////////////////////////////
	//레이더방탐-PDW 채널 보정 필요 상태 결과
	m_hCommIF.UnregisterOpcode(OPCODE_DP_TF_RSTCHCALIBCHECK, this); 
	
	//PDW 결과
	m_hCommIF.UnregisterOpcode(OPCODE_DP_TF_RSTPDW, this); 

	//PDW 수집완료 상태
	m_hCommIF.UnregisterOpcode(OPCODE_DP_TF_ACQDONESTATUS, this); 

	//PDW 채널보정 과제 수집완료 상태
	m_hCommIF.UnregisterOpcode(OPCODE_DP_TF_TASKACQDONESTATUS, this); 

	//IQ 결과
	m_hCommIF.UnregisterOpcode(OPCODE_DP_TF_RSTIQ, this); 

	StopReqTaskRetryTimer();
	StopRetryCollectStatusTimer();
	StopPDWConnStatusTimer();

	SetEvent(m_hKillIQFileEvent);	//#FA_C_IgnoredReturnValue_T1
	CloseHandle(hThread_IQFile);
	DeleteCriticalSection(&m_csIQFile);
	// 프로그램 종료시 한번만 호출하면 됩니다.
	for(int i = 0 ; i < ANT_PATH_MAX ; i++)
	{
		free(arrpBigArray[i]);
	}

	RadarDirAlgotirhm::RadarDirAlgotirhm::Close();
}

CDFTaskMngr* CDFTaskMngr::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CDFTaskMngr();		
	}

	return m_pInstance;
}

void CDFTaskMngr::Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData)
{
	TRACE("[Log] DFTaskMngr received.\n");

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


void CDFTaskMngr::ProcessMsg(STMsg& i_stMsg)
{
	STR_LOGMESSAGE stMsg;

	// OPCODE별로 경우를 나열하여 처리하는 로직 구현 필요
	// 하단에 구현

	/*TRACE("[Log] DFTaskMngr ProcessMsg 호출.\n");

	UEL_BITMAP_OPCode stOpCode= UEL_BITMAP_OPCode() ;
	stOpCode.w32 = i_stMsg.uiOpcode;*/
	bool bRtnSend = false;

	switch(i_stMsg.uiOpcode)
	{
	case OPCODE_BD_TF_SETSYSTEM: //시스템 설정(분석->방탐-> PDW)
		{
			TRACE("[수신]레이더분석-레이더방탐 시스템변수 설정 요구\n");

			STxSystemVariable stSystemVal;
			memcpy(&stSystemVal, i_stMsg.buf, i_stMsg.usMSize);

			//**For TEST**
			stSystemVal.uiCWDecisionPW = 2450000;

			//OPCODE_TF_DP_SYSSET OPCODE_CMD 14 확인 필요
			int opPDW_SetsysVal = MakeOPCode(CMDCODE_DF_TX_PDW_SYS_SET, DEVICECODE_TRD, DEVICECODE_TDP);

			bRtnSend = m_hCommIF.Send(opPDW_SetsysVal, sizeof(stSystemVal), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&stSystemVal); 

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-PDW 시스템변수 설정 요구\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-PDW 시스템변수 설정 요구\n");
			}

		}
		break;
	case OPCODE_BD_TF_REQACQTASK: //수집과제 요구 (분석->방탐)
		{
			//TRACE("[수신]레이더분석-레이더방탐 수집 과제 요구\n");
            sprintf( stMsg.szContents, "레이더분석-레이더방탐 수집 과제 요구" );
            ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enRECV, (LPARAM) & stMsg.szContents[0] );

			//수집과제 수신
			m_listTaskData.clear();
			//수집과제 수신 결과 전송 (방탐->분석)
			bRtnSend =  ProcessTaskMsg(i_stMsg.buf);

			if(bRtnSend == true)
			{
				//TRACE("[송신]레이더방탐-레이더분석 수집과제 수진결과\n");
                sprintf( stMsg.szContents, "레이더방탐-레이더분석 수집과제 수집결과" );
                ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 수집과제 수진결과\n");
			}
		}
		break;

	case OPCODE_BD_TF_REQNEXTTASK:
		{
			m_listTaskData.clear();

			ProcessNextTaskMsg(i_stMsg.buf, m_bIsTaskStop);

			//TRACE("[수신]레이더분석-레이더방탐 수집 다음 과제 요구\n");	
            sprintf( stMsg.szContents, "레이더분석-레이더방탐 수집 다음 과제 요구" );
            ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enRECV, (LPARAM) & stMsg.szContents[0] );
		}
		break;

	case OPCODE_BD_TF_REQIQDATA:
		{
			//PDW발생판에 전송			
			TRACE("[수신]레이더분석-레이더방탐 IQ 시작/중지 요구\n");	

			_IQ_REQ stIQReq;
			memcpy(&stIQReq, i_stMsg.buf, i_stMsg.usMSize);

			_IQ_REQ_TO_PDW stIQReqToPDW;

			if(m_TaskUseStat == TK_USE)
			{
				memcpy(stIQReqToPDW.aucTaskID, m_stCurTaskData.aucTaskID, sizeof(m_stCurTaskData.aucTaskID));

				stIQReqToPDW.uiCollectorID = stIQReq.uiCollectorID;
				stIQReqToPDW.uiReqIQInfo = stIQReq.uiReqIQInfo;

				if(stIQReq.uiReqIQInfo == IQ_START || stIQReq.uiReqIQInfo == IQ_STOP)   //IQ 시작이면 
				{
					m_nStep = 0;
				}	

				bRtnSend = m_hCommIF.Send(OPCODE_TF_DP_REQSIQ, sizeof(stIQReqToPDW), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&stIQReqToPDW); 

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더방탐-레이더분석 IQ 시작/중지 요구\n");
				}
				else
				{
					TRACE("[송신 실패]레이더방탐-레이더분석 IQ 시작/중지 요구\n");
				}
			}			
		}
		break;

	case OPCODE_BD_TF_REQSTARTACQ: //수집과제 시작(분석->방탐)
		{
			//TRACE("[수신]레이더분석-레이더방탐 수집시작 요구\n");
			sprintf( stMsg.szContents, "레이더분석-레이더방탐 수집시작 요구" );
			::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enRECV, (LPARAM) & stMsg.szContents[0] );

			STxAcqStartRequest stAcqStartReq;
			memcpy(&stAcqStartReq, i_stMsg.buf, i_stMsg.usMSize);
			STxAcqStartRslt acqStartRslt;
			m_bIsTaskStop = false;

			if(m_listTaskData.empty() != true)
			{
				if(stAcqStartReq.uiMode == ACQ_START)
				{
					m_iMode = MODE_CRT_TYPE;
					iter = m_listTaskData.begin();
					m_stCurTaskData = m_listTaskData.front();
					//Add onpoom Haeloox
					miAntPathDir = stAcqStartReq.uiAntPathDir;

					/////////분석에서 과제관리해서 주석처리///////////
					/*if(m_listTaskData.size() > 1)
					{
						iter++;
						m_iNextTaskCnt++;
					}		*/		
					/////////분석에서 과제관리해서 주석처리///////////

					//첫번째 과제
					//{
					//Step 1. 온품에 레이더 주파수 설정 제어 명령 전송

					if(m_TaskUseStat == TK_USE)
					{
						SetFreqToRadarUnit(m_stCurTaskData.uiFreq);

						//PDW 수집설정
						SetPDWAcqCollect(m_stCurTaskData, 0);
					}				

					//PDW 응답받으면

					//PDW 요청

					//step 2. 스펙트럼 전송 요청				
					SendSptrCmdToPDW(1);				
				}
				else if(stAcqStartReq.uiMode == ACQ_STOP)
				{					
					//가용한 수신국 정보 플래그를 위한 과제정보 클리어
					//if(m_TaskUseStat == TK_USE)
					//{
						m_bIsTaskStop = true;
						m_listTaskData.clear();
						//PDW 중지(ICD 추가 필요)
						SetPDWAcqCollect(m_stCurTaskData, 1);
					//}
					//스펙트럼 전송 중지
					SendSptrCmdToPDW(2); //중지				
					//IQ 운용 중지
				

					//정지결과 전송(방탐->분석)
				}
				else if(stAcqStartReq.uiMode == ACQ_RESTART)
				{
					//재 시작 처리

					//재시작결과 전송(방탐->분석)
				}
				else if(stAcqStartReq.uiMode == ACQ_PAUSE)
				{
					//일시정지 처리

					//일시정지결과 전송(방탐->분석)
				}
				else;		

				if(m_TaskUseStat == TK_USE)
				{
					//시작결과 전송(방탐->분석)	//결과 전송(시작,중지, 재시작, 일시정지)				
					acqStartRslt.uiMode = stAcqStartReq.uiMode;
					acqStartRslt.uiResult = 0;

					bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTSTARTACQ, sizeof(acqStartRslt), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)&acqStartRslt); 

					if(bRtnSend == true)
					{
						//TRACE("[송신]레이더방탐-레이더분석 수집시작 결과\n");
                        sprintf( stMsg.szContents, "레이더방탐-레이더분석 수집시작 결과" );
                        ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );
					}
					else
					{
						//TRACE("[송신 실패]레이더방탐-레이더분석 수집시작 결과\n");
                        sprintf( stMsg.szContents, "레이더방탐-레이더분석 수집시작 결과" );
                        ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );
					}
				}
			}
		}
		break;

		case OPCODE_BD_TF_REQCHCALIB: //채널보정
		{
			//TRACE("[수신]레이더분석-레이더방탐 채널 보정 요구\n");
            sprintf( stMsg.szContents, "레이더분석-레이더방탐 채널 보정 요구" );
            ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enRECV, (LPARAM) & stMsg.szContents[0] );

			m_iMode = MODE_CH_TYPE;

			//채널보정 진행 후
			//스펙트럼 전송 중지
			//PDW 중지(ICD 추가 필요)
			//IQ 운용 중지


			//StartChannelCorrect(MODE_INIT_TYPE);
			//StartCheckPDWConTimer(); 

			//채널보정 CW시간 빠르게 조절
			//m_stSystemVal.uiCWDecisionPW = 245;
			//m_stSystemVal.uiCWChoppinginterval = 245;

			int opPDW_SetsysVal = MakeOPCode(CMDCODE_DF_TX_PDW_SYS_SET, DEVICECODE_TRD, DEVICECODE_TDP);

			//m_hCommIF.Send(opPDW_SetsysVal, sizeof(m_stSystemVal), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&m_stSystemVal); 
			TRACE("채널보정을 빠르게 진행하기 위한 CW 초기 설정################### \n");
			m_bStartChannelCorrect = FALSE;
			if (m_bStartChannelCorrect == FALSE)
			{
				m_bStartChannelCorrect = TRUE;
				InitFileFromStartChCorrect();

				//g_RcvFunc.Finish();
				//g_RcvTempFunc = &g_RcvFunc;

				StartChannelCorrect(MODE_INIT_TYPE);
				//g_RcvTempFunc->Finish();
			}
			else
			{
				// 			CString  strcommand;
				// 			strcommand.Format("SENSe:FREQuency:CENTer?");
				// 			bool bfail = g_RcvFunc.SCPI_CommendQuery(strcommand);
			}
			
			//채널보정 결과 전송(100)
			//주기적으로 채널보정 상태 전송 -timer로 구현 필요
			STxChCorrectRslt chCorrectRslt;
			chCorrectRslt.uiResult = 1;
			bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_STATECHCALIB, sizeof(chCorrectRslt), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)&chCorrectRslt); 
		
			if(bRtnSend == true)
			{
				//TRACE("[송신]레이더방탐-레이더분석 채널 진행 상태\n");
                sprintf( stMsg.szContents, "레이더방탐-레이더분석 채널 진행 상태" );
                ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );
                
			}
			else
			{
				//TRACE("[송신 실패]레이더방탐-레이더분석 채널 진행 상태\n");
                sprintf( stMsg.szContents, "레이더방탐-레이더분석 채널 진행 상태" );
                ::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND_FAIL, (LPARAM) & stMsg.szContents[0] );
			}			
		}
		break;

		case OPCODE_BD_TF_REQCHCALIBCHECK:	//채널 보정 필요 상태 확인 요구
			{
				TRACE("[수신]레이더분석-레이더방탐 채널 보정 필요 상태 확인 요구\n");
				//PDW에 메시지 전송
				m_hCommIF.Send(OPCODE_TF_DP_REQCHCALIBCHECK, i_stMsg.usMSize, m_LinkInfo+1, Equip_Rev_DP, m_POSNIP, i_stMsg.buf); ////채널 보정 필요 상태 확인 요구				
			}
			break;

		case OPCODE_BD_TF_SETRESET:		//장비 리셋 요구
		{
			TRACE("[수신]레이더분석-레이더방탐 장비 리셋 요구\n");		
			SRxSendCtrlResult stReqRestart;
			memcpy(&stReqRestart, i_stMsg.buf, i_stMsg.usMSize);
			
			if(stReqRestart.uiResult == 0) //0:소프트웨어, 1:시스템 제어기, 2:레이더방탐기, 3: 전체
			{
				//메미로리 해제

				DWORD dwExitCode = 0;
				DWORD dwPID = GetCurrentProcessId();
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPID);

				if (NULL != hProcess)
				{
					// ASAE 다시 실행 - 경로설정
					CString sPath = GetModulePath();
					sPath.Replace("\\", "\\\\");
					sPath += "ICAA.exe";

					ShellExecute(NULL, "open", sPath, NULL, NULL, SW_SHOW);

					GetExitCodeProcess(hProcess, &dwExitCode);	// 프로세스 나가기 코드 얻어오기
					TerminateProcess(hProcess, dwExitCode);		// 프로세스 연결 끊기
					WaitForSingleObject(hProcess, INFINITE);	// 종료 될때까지 대기
					CloseHandle(hProcess);		// 프로세스 핸들 닫기
				}						
			}

			//전송 CMD
			//OPCODE_TF_BD_RSTRESETEQUIP

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-레이더분석 장비 리셋 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 장비 리셋 결과\n");
			}			
		}
		break;

		case OPCODE_BD_TF_CONTROLRECVR:		//수신기 제어
		{
			TRACE("[수신]레이더분석-레이더방탐 수신기 제어\n");

			//전송 CMD
			//OPCODE_TF_BD_RSTRECVCONTROL

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-레이더분석 수신기 제어 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 수신기 제어 결과\n");
			}	
		}
		break;

		case OPCODE_BD_TF_REQSYSTEMSET:		//레이더방탐 시스템변수 설정
		{
			TRACE("[수신]레이더분석-레이더방탐 레이더방탐 시스템변수 설정 요구\n");
			
			STxReqRadarSysVal stReqRadarSysVal;
			memcpy(&stReqRadarSysVal, i_stMsg.buf, i_stMsg.usMSize);


			//전송 CMD
			//OPCODE_TF_BD_RSTSYSTEMSET

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 레이더방탐 시스템변수 설정 결과\n");
			}	
		}
		break;

		case OPCODE_BD_TF_REQCHANGEEMIT:		//레이더/위협 변경
		{
			TRACE("[수신]레이더분석-레이더방탐 레이더/위협 변경 요구\n");

			//전송 CMD
			//OPCODE_TF_BD_RSTCHANGEEMIT

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-레이더분석 레이더/위협 변경 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 레이더/위협 변경 결과\n");
			}	
		}
		break;

		case OPCODE_BD_TF_REQREJFREQSET:		//수신배제주파수 설정
		{
			TRACE("[수신]레이더분석-레이더방탐 수신배제주파수 설정 요구\n");

			//전송 CMD
			//OPCODE_TF_BD_RSTREJFREQSET

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-레이더분석 수신배제주파수 수신결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 수신배제주파수 수신결과\n");
			}	
		}
		break;

		///////////////////////////////////////////////////////////////////////////////////
		///////////////////////PDW ///////////////////////////////////////////////////////
		case OPCODE_DP_TF_RSTCHCALIBCHECK: // 레이더방탐-PDW 채널 보정 필요 상태 결과
			{
				bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTCHCALIBCHECK, i_stMsg.usMSize, m_LinkInfo, Equip_Rev_BD, m_POSNIP, i_stMsg.buf); 
			}
			break;

		case OPCODE_DP_TF_ACQDONESTATUS: //수집완료 상태 전송 결과
		{
			if(m_iMode == MODE_INIT_TYPE || m_iMode == MODE_CH_TYPE)
			{
				_PDW_COLECTDONE_STAT stColectDoneStat;
				memcpy(&stColectDoneStat, i_stMsg.buf, i_stMsg.usMSize);
			
				if(stColectDoneStat.iNumOfPDW <= 3 || stColectDoneStat.iNumOfPDW == 0)
					m_pdwCntZero++;

				if(m_pdwCntZero >= 3)
				{
					//m_bPdwCntZeroFlag = true;
					m_RetryCnt++;
					TRACE("m_RetryCnt =================== \n", m_RetryCnt);
					m_pdwCntZero = 0;
					RetrySignalSend();
					stColectDoneStat.uiDoneStatus = 0;
				}

				TRACE("수집완료 상태 전송 결과 PDW 개수 %d \n", stColectDoneStat.iNumOfPDW);
				TRACE("NOISE LEVEL #1 %d \n", (stColectDoneStat.iNoiseLevel_1 /4) - 110 );
				TRACE("NOISE LEVEL #2 %d \n", (stColectDoneStat.iNoiseLevel_2 /4) - 110 );
				TRACE("NOISE LEVEL #3 %d \n", (stColectDoneStat.iNoiseLevel_3 /4) - 110 );
				TRACE("NOISE LEVEL #4 %d \n", (stColectDoneStat.iNoiseLevel_4 /4) - 110 );
				TRACE("NOISE LEVEL #5 %d \n", (stColectDoneStat.iNoiseLevel_5 /4) - 110 );

				int DoneFlag = 8;
				int isbDone = 0;
				isbDone = DoneFlag & stColectDoneStat.uiDoneStatus; 
				//isbDone = 1;
				TRACE("PDW 수집완료 상태 Flag %d \n", isbDone);
			
				if(/*isbDone != 1 ||*/  stColectDoneStat.iNumOfPDW < 3 /*|| m_bPdwCntZeroFlag == true*/)  
				{				
					if(m_hRetryColectTimerQueue == NULL)
					{							
						m_hRetryColectTimerQueue = CreateTimerQueue();
						CreateTimerQueueTimer(&m_hRetryColectStatTimer, m_hRetryColectTimerQueue, (WAITORTIMERCALLBACK)GRColectStatusTimer, this, COLSTAT_START_INTERVAL, COLSTAT_INTERVAL, 0);
					}				
				}
				else
				{
					m_bisRetrySigz = false;
					if(m_hRetryColectTimerQueue != NULL)
						StopRetryCollectStatusTimer();

					_PDW_DATA_Req req_PDWData;		 
				
					//PDW 요청
					if(m_iMode == MODE_INIT_TYPE) //채널보정
					{					
						strcpy_s( (char *) req_PDWData.aucTaskID, sizeof(req_PDWData.aucTaskID), "채널보정_초기셋팅" );
						req_PDWData.iPDWCnt = 10;					
					}
					//else if(m_iMode == MODE_CRT_TYPE)
					//{
					//	//PDW 요청
					//	int nPDWCnt = stColectDoneStat.iNumOfPDW;
					//
					//	strcpy( (char *) req_PDWData.aucTaskID, (char *)m_stCurTaskData.aucTaskID);
					//	req_PDWData.iPDWCnt = nPDWCnt;
					//}				
					else;

					//PDW 요청
					if(m_bIsTaskStop == false)
						bRtnSend = m_hCommIF.Send(OPCODE_TF_DP_REQPDW, sizeof(req_PDWData), m_LinkInfo+1, Equip_Rev_DP, m_POSNIP, (void*)&req_PDWData); 

					if(bRtnSend == true)
					{
						TRACE("[송신]레이더방탐-PDW PDW 전송 요청\n");
					}
					else
					{
						TRACE("[송신 실패]레이더방탐-PDW PDW 전송 요청\n");
					}	
				}
			}
		}
		break;

		case OPCODE_DP_TF_TASKACQDONESTATUS: //과제베이스 수집완료 상태 전송 결과
		{
			if(m_iMode == MODE_INIT_TYPE || m_iMode == MODE_CH_TYPE)
				return;

			_PDW_COLECTDONE_STAT stColectDoneStat;
			memcpy(&stColectDoneStat, i_stMsg.buf, i_stMsg.usMSize);
			
			StopReqTaskRetryTimer();
			isRqTaskAcqSigFromPDW = true;
			//다음과제 요청
			if(stColectDoneStat.iNumOfPDW == 0 && (stColectDoneStat.uiDoneStatus == 1 || stColectDoneStat.uiDoneStatus == 2))
			{
				/////////분석에서 과제관리해서 주석처리///////////
				//다음과제에 대해 PDW요청
				//ReqPDWNextTask(m_bIsTaskStop);
				/////////분석에서 과제관리해서 주석처리///////////
				TRACE("==================== 신호 없음 =================================\n");
				//ReqPDWNextTask(m_bIsTaskStop);

				//혼잡도 0%전송
				STR_DensityData  stDensityData;
				stDensityData.uiCollectorID = m_LinkInfo;
				memcpy(stDensityData.aucTaskID, m_stCurTaskData.aucTaskID, sizeof(m_stCurTaskData.aucTaskID));
				stDensityData.fDensity = 0;
				stDensityData.uiFreq = m_stCurTaskData.uiFreq;

				//혼잡도 전송
				bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_SENDDESITY, sizeof(stDensityData), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)&stDensityData); 

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더방탐-레이더분석 혼잡도 0 전송\n");
				}
				else
				{
					TRACE("[송신 실패]레이더방탐-레이더분석 혼잡도 0 전송\n");
				}			
				//혼잡도 0%전송
			}
			else
			{
				_PDW_DATA_Req req_PDWData;		 
				
				if(m_iMode == MODE_CRT_TYPE)
				{
					//PDW 요청
					int nPDWCnt = stColectDoneStat.iNumOfPDW;
				
					memcpy(req_PDWData.aucTaskID, m_stCurTaskData.aucTaskID, sizeof(m_stCurTaskData.aucTaskID));
					req_PDWData.iPDWCnt = nPDWCnt;
				}				
				else;

				//PDW 요청
				if(m_bIsTaskStop == false)
				{
					bRtnSend = m_hCommIF.Send(OPCODE_TF_DP_REQPDW, sizeof(req_PDWData), m_LinkInfo+1, Equip_Rev_DP, m_POSNIP, (void*)&req_PDWData); 
				}

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더방탐-PDW 과제 BASE PDW 전송 요청\n");
				}
				else
				{
					TRACE("[송신 실패]레이더방탐-PDW 과제 BASE PDW 전송 요청\n");
				}	
			}			

			TRACE("과제 수집완료 상태 전송 결과 PDW 개수 %d \n", stColectDoneStat.iNumOfPDW);
			TRACE("NOISE LEVEL #1 %d \n", (stColectDoneStat.iNoiseLevel_1 /4) - 110 );
			TRACE("NOISE LEVEL #2 %d \n", (stColectDoneStat.iNoiseLevel_2 /4) - 110 );
			TRACE("NOISE LEVEL #3 %d \n", (stColectDoneStat.iNoiseLevel_3 /4) - 110 );
			TRACE("NOISE LEVEL #4 %d \n", (stColectDoneStat.iNoiseLevel_4 /4) - 110 );
			TRACE("NOISE LEVEL #5 %d \n", (stColectDoneStat.iNoiseLevel_5 /4) - 110 );		
		}
		break;

		case OPCODE_DP_TF_RSTPDW: //PDW 데이터 결과
		{
			RX_STR_PDWDATA stRXPDWData;
			TRACE("recv buf size %d\n", i_stMsg.usMSize);
			memcpy(&stRXPDWData, i_stMsg.buf, i_stMsg.usMSize);			

			PDW_DATA *pPDW;		
			STR_PDWDATA stPDWData;

            stPDWData.pstPDW = ( _PDW * ) malloc( sizeof(_PDW) * 10000 );

			_PDW *pPDWData;

			int nCnt = stRXPDWData.count;
			pPDW = stRXPDWData.stPDW;
			pPDWData = & stPDWData.pstPDW[0];

			//////////////LOB데이터 생성
			memcpy(stPDWData.x.xb.aucTaskID, stRXPDWData.aucTaskID, sizeof(stRXPDWData.aucTaskID));
			//stPDWDataToAOA.iIsStorePDW = stPDWData.stPDW;			// 0 또는 1, PDW 저장되었으면 1로 설정함.
			stPDWData.x.xb.enCollectorID = (EN_RADARCOLLECTORID) stRXPDWData.iCollectorID;			// 1, 2, 3 중에 하나이어야 한다. (수집소)			
			stPDWData.x.xb.iIsStorePDW = 1;
			stPDWData.x.xb.stCommon.uiPDWID = m_uiPDWID++;

			struct timeval time_spec; 
			
			gettimeofday( & time_spec, NULL );
			stPDWData.SetColTime( time_spec.tv_sec, ( time_spec.tv_usec / 1000 ) % 1000 );

			/* data 확인 필요 */
			if(m_stCurTaskData.uiNBDRBandWidth == 1)
				stPDWData.x.xb.enBandWidth = XBAND::en120MHZ_BW;
			else
				stPDWData.x.xb.enBandWidth = XBAND::en5MHZ_BW;
			//////////////LOB데이터 생성

			int Freq, freqLOB;
			float fph[5] = {0.0,};	
			int bufcnt = 0;

			STR_DensityData  stDensityData;

			if(m_iMode != MODE_INIT_TYPE)
			{
				stDensityData.uiCollectorID = m_LinkInfo;
				memcpy(stDensityData.aucTaskID, m_stCurTaskData.aucTaskID, sizeof(m_stCurTaskData.aucTaskID));				
				stDensityData.uiFreq = m_stCurTaskData.uiFreq;

				//혼잡도 0%전송				
				if(nCnt == 0)
				{
					stDensityData.fDensity = 0 ;
				}
				else
				{
					//stDensityData.fDensity = (((nCnt - 3) / (m_stCurTaskData.uiAcquisitionTime)) * 100) / 3 ;
					stDensityData.fDensity =((( (float)(nCnt - 3) /( (float)(stPDWData.pstPDW[nCnt-1].ullTOA - stPDWData.pstPDW[3].ullTOA) /  (float)(122000 * m_stCurTaskData.uiAcquisitionTime)) )) * 100.0) / 50000.0 ;					
				
					if(stDensityData.fDensity < 0)
					{
						stDensityData.fDensity = 1;
					}
				}

				//혼잡도 전송
				bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_SENDDESITY, sizeof(stDensityData), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)&stDensityData); 
				//TRACE("혼잡도 freq %d, count %d, 혼잡도 %d, TOA시간 %d  \n", stDensityData.uiFreq, nCnt,  stDensityData.fDensity, (stPDWData.stPDW[nCnt-1].llTOA - stPDWData.stPDW[3].llTOA));
				TRACE("혼잡도 freq %d, count %d, 혼잡도 %d  \n", stDensityData.uiFreq, nCnt,  stDensityData.fDensity);

				if(bRtnSend == true)
				{
					TRACE("[송신]레이더방탐-레이더분석 혼잡도 %d 전송\n", stDensityData.fDensity);
				}
				else
				{
					TRACE("[송신 실패]레이더방탐-레이더분석 혼잡도 %d 전송\n", stDensityData.fDensity);
				}
				
			}			

			for(int i = 0; i <nCnt; ++i )
			{					
				//Freq = pPDW->iFreq;
				if(m_iMode == MODE_INIT_TYPE) 
					Freq = m_Freq;
				else
				{


					/*	Freq = ROUNDING(((float)(m_stCurTaskData.uiFreq + (short)(pPDW->iFreq)) / 1000), 0);
					freqLOB = m_stCurTaskData.uiFreq + (short)(pPDW->iFreq) ;*/

					if(Freq < 8100) //haeloox
					{
						Freq = ROUNDING(((float)(m_stCurTaskData.uiFreq + (short)(pPDW->iFreq)*10) / 1000), 0);
						freqLOB = m_stCurTaskData.uiFreq + (short)(pPDW->iFreq)*10 ;
					}
					else
					{
						Freq = ROUNDING(((float)(m_stCurTaskData.uiFreq - (short)(pPDW->iFreq)*10)/ 1000), 0);
						freqLOB = m_stCurTaskData.uiFreq - (short)(pPDW->iFreq)*10 ;
					}
					
				}
				
				fph[0] = (float) (pPDW->iph[0] * PH_DIFF);
				fph[1] = (float) (pPDW->iph[1] * PH_DIFF);
				fph[2] = (float) (pPDW->iph[2] * PH_DIFF);
				fph[3] = (float) (pPDW->iph[3] * PH_DIFF);
				fph[4] = (float) (pPDW->iph[4] * PH_DIFF);				

				TRACE("Freq_r %d, llTOA %10I64d, Freq %d, iPA %d, iPulseType %d, iPW %d, iPFTag %d, iph_1 %d, iph_2 %d, iph_3 %d, iph_4 %d, iph_5 %d  \n",
					Freq, pPDW->llTOA, pPDW->iFreq, pPDW->iPA, pPDW->iPulseType, pPDW->iPW, pPDW->iPFTag, pPDW->iph[0], pPDW->iph[1], pPDW->iph[2],
					pPDW->iph[3], pPDW->iph[4]);			

				if(m_iMode == MODE_INIT_TYPE) //채널보정
				{					
					if(i == 5)
					{
						SetDataChanelCorrect(Freq, fph); //저장 //맴버의 주파수설정값으로 입력할지 확인
						break;
					}
				}	
				else if(m_iMode == MODE_CRT_TYPE )	//정확도
				{			
					if(i > 3) 
					{
						int idxFreq = 0;
						int iCloseFreq = 0;
						iCloseFreq = GetCloseDFFreq(Freq);
						idxFreq = GetIndexFreq(iCloseFreq);
						int i_AOA =0;
						//이명식 수석님 함수 호출해서 pdw 위상차를 aoa 변환 후
						i_AOA = GetAOADataFromAlgrism(iCloseFreq, idxFreq, fph) ;
						TRACE("=======  ORA AOA  VAL  %d =======\n", i_AOA);
						//i_AOA = AOA_BASE - i_AOA;
						i_AOA += ((miAntPathDir * 9000) - 6000);
						i_AOA += m_iAOAOffset;
						TRACE("=======  ORA+OFFSET AOA  VAL  %d =======\n", i_AOA);

						if(i_AOA < 0)
							i_AOA += AOA_BASE;

						if(i_AOA > AOA_BASE)
							i_AOA = i_AOA % AOA_BASE;

						TRACE("======= RESULT AOA  VAL  %d =======\n", i_AOA);
						//값 확인 필요 pPDW->iFreq, pPDW->iPulseType, pPDW->iPA, pPDW->iPW, pPDW->llTOA
						pPDWData->uiFreq = freqLOB;
						pPDWData->uiAOA = i_AOA;
						pPDWData->iPulseType = pPDW->iPulseType;
						pPDWData->uiPA = pPDW->iPA;
						pPDWData->uiPW = pPDW->iPW;
						pPDWData->ullTOA = pPDW->llTOA;		
						pPDWData->iPFTag = pPDW->iPFTag;
						pPDWData->x.xb.fPh1 = fph[0];
						pPDWData->x.xb.fPh2 = fph[1];
						pPDWData->x.xb.fPh3 = fph[2];
						pPDWData->x.xb.fPh4 = fph[3];
                        pPDWData->x.xb.fPh5 = fph[4];

						bufcnt++;
					}
				}
				else;				

				if(i > 3) 
				{
					++ pPDW;
					++ pPDWData;
				}
			}			

            stPDWData.SetTotalPDW( bufcnt );
			//stPDWData.uiTotalPDW = bufcnt;				// 수집 PDW 개수는 최대 4096 개

			//재귀호출
			if(m_iMode == MODE_INIT_TYPE || m_iMode == MODE_CH_TYPE) // 채널보정
			{
				if(m_Freq == 10000) //수정
				{
					TRACE("freq ============= %d \n", m_Freq);
					m_listAutoFreqData.pop_front();
				}

				if(!m_listAutoFreqData.empty())
				{
					StartChannelCorrect(m_iMode);

					//채널보정 결과 전송(100)
					//주기적으로 채널보정 상태 전송 -timer로 구현 필요
					STxChCorrectRslt chCorrectRslt;
					float fdata = (float)((float)m_iIncrCoretFreqCnt/m_iTotCoretFreqDataCnt) * 100;
					
					unsigned int stateCH = (unsigned int)fdata;
					chCorrectRslt.uiResult = stateCH;

					TRACE("진행결과 값 =============================================%f 정수 =========== %d\n",fdata, stateCH );

					if(chCorrectRslt.uiResult == 30 ||  chCorrectRslt.uiResult == 50 || chCorrectRslt.uiResult == 80 )
					{
						bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_STATECHCALIB, sizeof(chCorrectRslt), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)&chCorrectRslt); 
						TRACE("채널보정 진행상태 ============= %d\n", chCorrectRslt.uiResult);
					}
				}
				else
				{
					//V/UHF 제어 설정
					int opcode = MakeOPCode(CMDCODE_UHF_TX_SET_ANT_MODE, DEVICECODE_TRD, DEVICECODE_TVU);	
					STxUHFSetAntMode SetAntMode;
					SetAntMode.ucCalAntMode = 1;
					//m_hCommIF.Send(opcode, sizeof(SetAntMode),m_LinkInfo+1, Equip_Rev_VU, m_POSNIP, (void*)&SetAntMode);					

					STxChCorrectRslt chCorrectRslt;
					chCorrectRslt.uiResult = 100;
					bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_STATECHCALIB, sizeof(chCorrectRslt), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)&chCorrectRslt); 

					/*haeloox*/
					CString strcommand;
					bool bfail = false;
					g_RcvTempFunc = &g_RcvFunc;
					strcommand.Format("SENSe:CALMode:STATe 0");
					bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);

					GetAutoFreqChCorrect();
					m_bEndChannelCorrect = true;

					//if(m_SetReqInitFlag == true) //초기과 결과 전송
					//{
					//	m_SetReqInitFlag = false;
					//	SendReqInitRslt();
					//}
					
					TRACE("방탐보정 끝 \n");

					//채널보정 끝날때 원래대로 설정
					m_stSystemVal.uiCWDecisionPW = 245000;
					m_stSystemVal.uiCWChoppinginterval = 245000;

					int opPDW_SetsysVal = MakeOPCode(CMDCODE_DF_TX_PDW_SYS_SET, DEVICECODE_TRD, DEVICECODE_TDP);

					bRtnSend = m_hCommIF.Send(opPDW_SetsysVal, sizeof(m_stSystemVal), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&m_stSystemVal); 

					SRxSendCtrlResult stDoneChCorrect;
					stDoneChCorrect.uiResult = 1;

					bRtnSend = m_hCommIF.Send(OPCODE_TF_DP_DONECHCALIBCHECK, sizeof(stDoneChCorrect), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&stDoneChCorrect);  //채널보정 완료메시지					
					//채널보정 끝날때 원래대로 설정
				}			
			}
			else if(m_iMode == MODE_CRT_TYPE)
			{
				int i;

				///////////////////////////////////////////////////////////////////////////////
				//조철희 수석님의 PDW데이타에서 LOB 추출 알고리즘 호출			
				RadarDirAlgotirhm::RadarDirAlgotirhm::Start( & stPDWData );

				int nCoLOB=RadarDirAlgotirhm::RadarDirAlgotirhm::GetCoLOB();
                //LONG lOpInitID=RadarDirAlgotirhm::RadarDirAlgotirhm::GetOPInitID();

				SRxLOBData *pLOBData=RadarDirAlgotirhm::RadarDirAlgotirhm::GetLOBData();
				///////////////////////////////////////////////////////////////////////////////

				//TRACE("**************[송신]레이더방탐-레이더분석 LOB 전송 개수%d============\n", nCoLOB);

				if( nCoLOB >= 1 ) {
					char buffer[100];
					struct tm *pstTime;

					time_t ts=stPDWData.GetColTime();

					pstTime = localtime( & ts );
					strftime( buffer, 100, "%Y-%m-%d_%H_%M_%S", pstTime );

					sprintf( stMsg.szContents, "레이더방탐-레이더분석[%s] LOB 전송/PDW 개수 : %d, %d", buffer, nCoLOB, stPDWData.GetTotalPDW() );
					::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );

					SRxLOBData *ppLOBData=pLOBData;
					for( i=0 ; i < nCoLOB ; ++i ) {
						sprintf( stMsg.szContents, "  [#%02d] %s %04.1f %s(%.3f, %.3f) %s(%.1f,%.1f),[%2d] %.1fdBm(%.1f,%.1f) %.1f[ns](%.1f,%.1f)[%d]", i+1, g_szAetSignalType[ppLOBData->iSignalType], ppLOBData->fDOAMean, g_szAetFreqType[ppLOBData->iFreqType], ppLOBData->fFreqMin, ppLOBData->fFreqMax, g_szAetPriType[ppLOBData->iPRIType], ppLOBData->fPRIMin, ppLOBData->fPRIMax, ppLOBData->iPRIPositionCount, ppLOBData->fPAMean, ppLOBData->fPAMin, ppLOBData->fPAMax, ppLOBData->fPWMean, ppLOBData->fPWMin, ppLOBData->fPWMax, ppLOBData->iNumOfPDW );
						::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );
						++ ppLOBData;
					}
				}
				else {				
					sprintf( stMsg.szContents, "레이더방탐-레이더분석 신호 수집이 전혀 되지 않고 있습니다." );
					::SendMessage( g_DlgHandle, UWM_USER_LOG_MSG, (WPARAM) enSEND, (LPARAM) & stMsg.szContents[0] );
				}


				/*if(nCoLOB == 0)
				{
				stDensityData.fDensity = 0;
				}*/					

				/*if(bRtnSend == true)
				{
				TRACE("[송신]레이더방탐-레이더분석 혼잡도 %d 전송\n", stDensityData.fDensity);
				}
				else
				{
				TRACE("[송신 실패]레이더방탐-레이더분석 혼잡도 %d 전송\n", stDensityData.fDensity);
				}	*/
			
				if(nCoLOB > 0)
				{
					bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_SENDLOB, nCoLOB * sizeof(SRxLOBData), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)pLOBData); 

					if(bRtnSend == true)
					{
						TRACE("[송신]레이더방탐-레이더분석 LOB 전송 freq %d \n", Freq);
					}
					else
					{
						TRACE("[송신 실패]레이더방탐-레이더분석 LOB 전송\n");
					}	
				}
				else {
					/////////분석에서 과제관리해서 주석처리///////////
					//다음과제에 대해 PDW요청
					ReqPDWNextTask(false);
					/////////분석에서 과제관리해서 주석처리///////////			
				}


			}

            free( stPDWData.pstPDW );
		}

		break;

		//IQ 결과
		case OPCODE_DP_TF_RSTIQ:
		{
			TRACE("[수신]PDW-레이더방탐 IQ결과 \n");
			//m_nStep++;

			//RX_STR_PDWDATA stPDWData;
			TRACE("recv IQ buf size %d\n", i_stMsg.usMSize);
					
			//_IQ *pIQ;		
			STR_IQDATA stIQData;

			memcpy(&stIQData, i_stMsg.buf, i_stMsg.usMSize);	
			//m_pIQData = &stIQData;

			EnterCriticalSection(&m_csIQFile);	
			m_IQDataStore.push_back(stIQData);		
			LeaveCriticalSection(&m_csIQFile);				
			
			//IQ데이터 파일로 저장
			ThreadIQFile();
			//SaveAll_IQFile();

			//전송 CMD
			//OPCODE_TF_BD_RSTCHANGEEMIT

			/*if(bRtnSend == true)
			{
			TRACE("[송신]레이더방탐-레이더분석 IQ결과\n");
			}
			else
			{
			TRACE("[송신 실패]레이더방탐-레이더분석 IQ결과\n");
			}	*/
		}
		break;
	

	default:
		break;
	}
}

void CDFTaskMngr::ThreadIQFile()
{
	if(m_IQFileInit)
	{
		m_IQFileInit = false;
		hThread_IQFile = (HANDLE) _beginthreadex(NULL, 0, IQFileThread, (void*) this, 0, &m_dwID);
	}

	SetEvent(m_hSignalIQFileEvent);	//#FA_C_IgnoredReturnValue_T1
}

UINT CDFTaskMngr::IQFileThread(void* lpParam)
{
	bool bRst = true;
	
	CDFTaskMngr* pThis = reinterpret_cast<CDFTaskMngr*>(lpParam);

	while ( 1 )	//#FA_C_PotentialUnboundedLoop_T1 
	{		
		HANDLE hObjects[2];
		hObjects[0] = pThis->m_hKillIQFileEvent;
		hObjects[1] = pThis->m_hSignalIQFileEvent;

		DWORD dwWait1 = WaitForMultipleObjects(2,hObjects,FALSE,INFINITE);
		if (dwWait1 == WAIT_OBJECT_0)		
		{
			break;
		}

		if (dwWait1 == WAIT_OBJECT_0 + 1)
		{
			pThis->SaveAll_IQFile();			
		}
	}		

	return bRst;

}

void CALLBACK GRColectStatusTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CDFTaskMngr* pThis = ((CDFTaskMngr*)lpParam);

	//pThis->m_PingThread.StartPing(NULL, PING_ID_HEARTBEAT);

	pThis->RetryCollectStatusReq();
}

void CDFTaskMngr::RetryCollectStatusReq()
{
	int opcode_done_status = MakeOPCode(CMDCODE_DF_RX_PDW_COLECT_DONE_STAT, DEVICECODE_TRD, DEVICECODE_TDP);

	_PDW_COLECTDONE_STAT_REQ  req_collStat;
	req_collStat.uiDoneStatus = 1;

	m_hCommIF.Send(opcode_done_status, sizeof(req_collStat), LINK1_ID, Equip_Rev_DP, m_POSNIP, (void*)&req_collStat); 
}

void CDFTaskMngr::SendDummyMsg()
{
	/*char acTemp[sizeof(unsigned short)*8192+sizeof(int)+sizeof(unsigned int)] = {0,};

	acTemp[0] = 1;
	acTemp[1] = 2;
	acTemp[2] = 3;	*/
	//STMsg i_stMsg;
	//memset(&i_stMsg.buf, 0, 65535);
	//m_hCommIF.Send(OPCODE_TF_DP_REQCHCALIBCHECK, 0, m_LinkInfo+1, Equip_Rev_DP, m_POSNIP, i_stMsg.buf); ////채널 보정 필요 상태 확인 요구		

	//m_hCommIF.Send(0x82022300,sizeof(unsigned short)*8192+sizeof(int)+sizeof(unsigned int),0, 0, 0, (void*)&acTemp);
}

void CDFTaskMngr::StopReqTaskRetryTimer()
{
	if ( m_hRqTaskRetryTimerQueue == NULL )
		return;

	BOOL bRtn = DeleteTimerQueueTimer(m_hRqTaskRetryTimerQueue, m_hRqTaskRetryTimer, NULL);

	if(bRtn)
	{
		m_hRqTaskRetryTimerQueue = NULL;
	}
	else
	{
		if ( ERROR_IO_PENDING == GetLastError() || m_hRqTaskRetryTimerQueue == NULL || m_hRqTaskRetryTimer == NULL )
		{
			m_hRqTaskRetryTimerQueue = NULL;
		}
		else
		{
			while(DeleteTimerQueueTimer(m_hRqTaskRetryTimerQueue, m_hRqTaskRetryTimer, NULL) == 0)
			{
				if ( ERROR_IO_PENDING == GetLastError() || m_hRqTaskRetryTimerQueue == NULL || m_hRqTaskRetryTimer == NULL )
				{
					m_hRqTaskRetryTimerQueue = NULL;
					break;
				}
			}			
		}
	}	
}

void CDFTaskMngr::StopRetryCollectStatusTimer()
{
	if ( m_hRetryColectTimerQueue == NULL )
		return;

	BOOL bRtn = DeleteTimerQueueTimer(m_hRetryColectTimerQueue, m_hRetryColectStatTimer, NULL);

	if(bRtn)
	{
		m_hRetryColectTimerQueue = NULL;
	}
	else
	{
		if ( ERROR_IO_PENDING == GetLastError() || m_hRetryColectTimerQueue == NULL || m_hRetryColectStatTimer == NULL )
		{
			m_hRetryColectTimerQueue = NULL;
		}
		else
		{
			while(DeleteTimerQueueTimer(m_hRetryColectTimerQueue, m_hRetryColectStatTimer, NULL) == 0)
			{
				if ( ERROR_IO_PENDING == GetLastError() || m_hRetryColectTimerQueue == NULL || m_hRetryColectStatTimer == NULL )
				{
					m_hRetryColectTimerQueue = NULL;
					break;
				}
			}			
		}
	}	
}

void CDFTaskMngr::StopPDWConnStatusTimer()
{	
	if ( m_hPDWConTimerQueue == NULL )
		return;

	BOOL bRtn = DeleteTimerQueueTimer(m_hPDWConTimerQueue, m_hPDWConStatTimer, NULL);

	if(bRtn)
	{
		m_hPDWConTimerQueue = NULL;
	}
	else
	{
		if ( ERROR_IO_PENDING == GetLastError() || m_hPDWConTimerQueue == NULL || m_hPDWConStatTimer == NULL )
		{
			m_hPDWConTimerQueue = NULL;
		}
		else
		{
			while(DeleteTimerQueueTimer(m_hPDWConTimerQueue, m_hPDWConStatTimer, NULL) == 0)
			{
				if ( ERROR_IO_PENDING == GetLastError() || m_hPDWConTimerQueue == NULL || m_hPDWConStatTimer == NULL )
				{
					m_hPDWConTimerQueue = NULL;
					break;
				}
			}			
		}
	}	
}

void CDFTaskMngr::RetrySignalSend()
{
	m_bisRetrySigz = true;
	
	UINT64 uifrequency = (UINT64)((double)m_Freq * 1000000.0f);//Hz
	bool bfail = false;

	CString  strcommand;
	strcommand.Format("SENSe:FREQuency:CENTer %I64d", uifrequency);
	bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);	
//	g_RcvTempFunc->Finish();

	TRACE("result_1 %d\n", bfail);
	
	if(m_RetryCnt >= 1)
	{
		uifrequency = -40;
		m_RetryCnt = 0;
		m_RetryAginCnt++;
	}
	else
		uifrequency = -35;

	if(m_RetryAginCnt >=2)
	{
		uifrequency =-45;
		m_RetryAginCnt =0;
	}

// 	strcommand.Format("SENSe:GCONtrol %I64d", uifrequency);
// 	bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);	
// //	g_RcvTempFunc->Finish();
// 	TRACE("GCONtrol %d\n", bfail);

	int opcode_Coll = MakeOPCode(CMDCODE_DF_TX_PDW_COLECT_SET, DEVICECODE_TRD, DEVICECODE_TDP); //신호수집 설정

	_PDW_COLECTSET set_collect;
	strcpy( (char *) set_collect.aucTaskID, "채널보정_초기셋팅" );
		
	set_collect.iRxThresholdValue = (-35+110)*4;
	set_collect.uiAcquisitionTime = 100000000;
	set_collect.uiNumOfAcqPulse = 100; // 윤상범
	set_collect.uiNBDRBandwidth =1;
	set_collect.uiMode = ACQ_START;

	m_hCommIF.Send(opcode_Coll, sizeof(set_collect), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&set_collect); //수집완료 상태 요청	
}

bool CDFTaskMngr::ProcessTaskMsg(void *i_pvData)
{
	//UINT nTaskNum = 0;
	bool bRtnSend = false;
	STxAcqTraskRequest AcqtaskReq; 
	memcpy(&AcqtaskReq, i_pvData, sizeof(AcqtaskReq));

	m_iTaskCnt = AcqtaskReq.uiNumOfTask; 

	STxAcqTraskData *pTaskData;
	pTaskData = AcqtaskReq.stTaskData;

	STxAcqTraskData stTaskMngrData;

	for(int i = 0; i <m_iTaskCnt; ++i )
	{					
		//과제정보 관리		
		/*stTaskMngrData.iRxThresholdValue = ENDIAN32(pTaskData->iRxThresholdValue);
		stTaskMngrData.uiAcquisitionTime = ENDIAN32(pTaskData->uiAcquisitionTime);
		stTaskMngrData.uiNBDRBandWidth = ENDIAN32(pTaskData->uiNBDRBandWidth);
		stTaskMngrData.uiNumOfAcqPuls = ENDIAN32(pTaskData->uiNumOfAcqPuls);
		stTaskMngrData.uiFreq = ENDIAN32(pTaskData->uiFreq);*/

		memcpy(stTaskMngrData.aucTaskID, pTaskData->aucTaskID, sizeof(stTaskMngrData.aucTaskID)); 						
		if(m_LinkInfo == LINK1_ID)
		{
			stTaskMngrData.iRxThresholdValue1 =  pTaskData->iRxThresholdValue1;
			m_RadarGain = pTaskData->iRadarEqGain1;
			m_TaskUseStat = pTaskData->uiRvcUseStat1;
		}
		else if(m_LinkInfo == LINK2_ID)
		{
			stTaskMngrData.iRxThresholdValue2 =  pTaskData->iRxThresholdValue2;
			m_RadarGain = pTaskData->iRadarEqGain2;
			m_TaskUseStat = pTaskData->uiRvcUseStat2;
		}
		else if(m_LinkInfo == LINK3_ID)
		{
			stTaskMngrData.iRxThresholdValue3 =  pTaskData->iRxThresholdValue3;
			m_RadarGain = pTaskData->iRadarEqGain3;
			m_TaskUseStat = pTaskData->uiRvcUseStat3;
		}

		//stTaskMngrData.iRxThresholdValue = pTaskData->iRxThresholdValue;
		stTaskMngrData.uiAcquisitionTime = pTaskData->uiAcquisitionTime;
		stTaskMngrData.uiNBDRBandWidth = pTaskData->uiNBDRBandWidth;
		stTaskMngrData.uiNumOfAcqPuls = pTaskData->uiNumOfAcqPuls;
		stTaskMngrData.uiFreq = pTaskData->uiFreq;
		//m_Freq = stTaskMngrData.uiFreq / 1000;
		//m_uiLastCurFreq = m_Freq;
		m_listTaskData.push_back(stTaskMngrData);				
			
		TRACE("\n ================= PDW 개수 : %d,  설정시간 : %d ====================\n", pTaskData->uiNumOfAcqPuls, pTaskData->uiAcquisitionTime); 
		++ pTaskData;
	}		

	//수집과제 수신 결과 전송 (방탐->분석)	
	SRxTSGCtrlResult taskRslt;
	taskRslt.iResult = 1;
	bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTACQTASK, sizeof(taskRslt), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)&taskRslt); 

	return bRtnSend;
}

void CDFTaskMngr::ProcessNextTaskMsg(void *i_pvData, bool isTaskStop)
{
	//UINT nTaskNum = 0;
	if(m_listTaskData.size() > 0)
	{
		m_listTaskData.clear();
	}

	m_iMode = MODE_CRT_TYPE;
	bool bRtnSend = false;
	STxAcqTraskRequest AcqtaskReq; 
	memcpy(&AcqtaskReq, i_pvData, sizeof(AcqtaskReq));

	m_iTaskCnt = AcqtaskReq.uiNumOfTask; 

	STxAcqTraskData *pTaskData;
	pTaskData = AcqtaskReq.stTaskData;

	STxAcqTraskData stTaskMngrData;

	for(int i = 0; i <m_iTaskCnt; ++i )
	{			
		memcpy(stTaskMngrData.aucTaskID, pTaskData->aucTaskID, sizeof(stTaskMngrData.aucTaskID)); 
		if(m_LinkInfo == LINK1_ID)
		{
			stTaskMngrData.iRxThresholdValue1 =  pTaskData->iRxThresholdValue1;
			m_RadarGain = pTaskData->iRadarEqGain1;
			m_TaskUseStat = pTaskData->uiRvcUseStat1;
		}
		else if(m_LinkInfo == LINK2_ID)
		{
			stTaskMngrData.iRxThresholdValue2 =  pTaskData->iRxThresholdValue2;
			m_RadarGain = pTaskData->iRadarEqGain2;
			m_TaskUseStat = pTaskData->uiRvcUseStat2;
		}
		else if(m_LinkInfo == LINK3_ID)
		{
			stTaskMngrData.iRxThresholdValue3 =  pTaskData->iRxThresholdValue3;
			m_RadarGain = pTaskData->iRadarEqGain3;
			m_TaskUseStat = pTaskData->uiRvcUseStat3;
		}
		//stTaskMngrData.iRxThresholdValue = pTaskData->iRxThresholdValue;
		stTaskMngrData.uiAcquisitionTime = pTaskData->uiAcquisitionTime;
		stTaskMngrData.uiNBDRBandWidth = pTaskData->uiNBDRBandWidth;
		stTaskMngrData.uiNumOfAcqPuls = pTaskData->uiNumOfAcqPuls;
		stTaskMngrData.uiFreq = pTaskData->uiFreq;
		m_listTaskData.push_back(stTaskMngrData);				
			
		TRACE("\n ================= NEXT TASK id : %s PDW 개수 : %d,  설정시간 : %d ====================\n", pTaskData->aucTaskID, pTaskData->uiNumOfAcqPuls, pTaskData->uiAcquisitionTime); 
		++ pTaskData;
	}		

	m_stCurTaskData = m_listTaskData.front();

	if(isTaskStop != true)
	{
		if(m_TaskUseStat == TK_USE)
		{
			//Step 1. 온품에 레이더 주파수 설정 제어 명령 전송
			SetFreqToRadarUnit(m_stCurTaskData.uiFreq);

			//PDW 수집설정
			SetPDWAcqCollect(m_stCurTaskData, 0);
		}	

		//step 2. 스펙트럼 전송 요청				
		SendSptrCmdToPDW(1);	
	}
}

void CDFTaskMngr::testTask()
{
	//STxAcqTraskData stTaskMngrData;
	//CString stTask;
	//
	//m_iTaskCnt = 1;
	//m_iNextTaskCnt = 0;
	//for(int i=0; i<1; i++)
	//{
	//	stTask.Format("과제_%d", i);
	//	memcpy(stTaskMngrData.aucTaskID, stTask.GetBuffer(), stTask.GetLength()); 						
	//	stTaskMngrData.iRxThresholdValue = 1;
	//	stTaskMngrData.uiAcquisitionTime = 1000;
	//	stTaskMngrData.uiNBDRBandWidth = 1;
	//	stTaskMngrData.uiNumOfAcqPuls = 1;
	//	stTaskMngrData.uiFreq = 1000;
	//	//m_Freq = 10000 / 1000;
	//	//m_uiLastCurFreq = m_Freq;
	//	m_listTaskData.push_back(stTaskMngrData);	
	//}

	//iter = m_listTaskData.begin();
	//m_stCurTaskData = m_listTaskData.front();

	//if(m_listTaskData.size() > 1)
	//{
	//	iter++;
	//	m_iNextTaskCnt++;
	//}				
	//			
	//ReqPDWNextTask();

	//ReqPDWNextTask();

	//ReqPDWNextTask();

	//ReqPDWNextTask();

	//ReqPDWNextTask();

	//ReqPDWNextTask();
	
}

void CDFTaskMngr::ReqPDWNextTask(bool isTaskStop)
{
	/*if(m_iTaskCnt ==  m_iNextTaskCnt)
	{
		iter = m_listTaskData.begin();		
		m_iNextTaskCnt = 0;
	}

	if(m_iTaskCnt != 1)
	{
		m_stCurTaskData = *(iter);		

		iter++;
		m_iNextTaskCnt ++;
	}*/

	if(isTaskStop != true)
	{
		//Step 1. 온품에 레이더 주파수 설정 제어 명령 전송
		SetFreqToRadarUnit(m_stCurTaskData.uiFreq);

		//PDW 수집설정
		SetPDWAcqCollect(m_stCurTaskData, 0);

		//step 2. 스펙트럼 전송 요청				
		SendSptrCmdToPDW(1);	
	}
}

void CDFTaskMngr::StartCheckPDWConTimer()
{
	if(m_hPDWConTimerQueue == NULL)
	{				
		m_bStartChannelCorrect = FALSE;
		m_hPDWConTimerQueue = CreateTimerQueue();
		CreateTimerQueueTimer(&m_hPDWConStatTimer, m_hPDWConTimerQueue, (WAITORTIMERCALLBACK)PDWConnStatusTimer, this, 0, COLSTAT_INTERVAL, 0);
	}		
}

void CALLBACK PDWConnStatusTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CDFTaskMngr* pThis = ((CDFTaskMngr*)lpParam);	

	pThis->CheckPDWConnStatus();
}

void CDFTaskMngr::CheckPDWConnStatus()
{
	if(m_hCommIF.GetPDWCHCorrectConnStatus() == true)
	{
		StopPDWConnStatusTimer();

		//채널보정 CW시간 빠르게 조절
		//m_stSystemVal.uiCWDecisionPW = 245;
		//m_stSystemVal.uiCWChoppinginterval = 245;

		int opPDW_SetsysVal = MakeOPCode(CMDCODE_DF_TX_PDW_SYS_SET, DEVICECODE_TRD, DEVICECODE_TDP);

		//m_hCommIF.Send(opPDW_SetsysVal, sizeof(m_stSystemVal), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&m_stSystemVal); 
		TRACE("채널보정을 빠르게 진행하기 위한 CW 초기 설정################### \n");

		if (m_bStartChannelCorrect == FALSE)
		{
			m_bStartChannelCorrect = TRUE;
			InitFileFromStartChCorrect();

			g_RcvFunc.Finish();
			g_RcvTempFunc = &g_RcvFunc;

			StartChannelCorrect(MODE_INIT_TYPE);
			g_RcvTempFunc->Finish();
		}
		else
		{
// 			CString  strcommand;
// 			strcommand.Format("SENSe:FREQuency:CENTer?");
// 			bool bfail = g_RcvFunc.SCPI_CommendQuery(strcommand);
		}
	}
}

unsigned char CDFTaskMngr::GetLastIP()
{
	unsigned char ucLastIP = NULL;

	//////// 자신의 IP 획득 ///////
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

	return ucLastIP;
}

bool CDFTaskMngr::SetDataChanelCorrect(UINT iFreq, float* fch_t)
{
	bool result = true;	
	float fch[5];
	memcpy(fch, fch_t, sizeof(fch));

	ofstream fCSVout;
	if(m_iMode == MODE_INIT_TYPE) //채널보정
	{
		fCSVout.open(m_strFilePath_Chennel, ios::out | ios::app);
	}
	
	CString strWriteData = _T("");
	
	strWriteData.Format("%d,%f,%f,%f,%f,%f", iFreq,  fch[0],fch[1],fch[2],fch[3],fch[4]);

	fCSVout << strWriteData << endl;
		
	fCSVout.close();
	return result;
}

// 방사보정 장입용 ROM 데이터(위상) 파일 로드용 함수
BOOL CDFTaskMngr::LoadDfCalRomDataPh() 
{
	FILE	*fpFile;
	CString	fileName, szDefaultFileName;
	CString msg;

	float fGetGenBear= 0.f;  //LMS_ADD_20171221
	float fGetGenBearOld= 0.f;  //LMS_ADD_20220206
	float fGetGenFreq= 0.f;  //LMS_ADD_20171221
	float fGetGenFreqOld= 0.f;  //LMS_ADD_20220206
	UINT  uiBand = 0;        //LMS_ADD_20171221
	UINT uiFreqRangeStartMHzTemp = 0;//LMS_ADD_20171221
	UINT uiFreqReadStartMHzTemp  = 0;//LMS_ADD_20171221
	UINT uiFreqReadEndMHzTemp    = 0;//LMS_ADD_20171221

	for(int iAntIdx = 0 ; iAntIdx < ANT_PATH_MAX ; iAntIdx++)
	{
		m_strAntFullFileName.Format("C:\\IdexFreq\\LIG_MF_Data_%d.txt", iAntIdx);
		memset(arrpBigArray[iAntIdx]->m_usPhDfRomData,        0, sizeof(arrpBigArray[iAntIdx]->m_usPhDfRomData));//LMS_MODIFY_20171222   memset(m_usPhDfRomData,  0, sizeof(m_usPhDfRomData));  // 방사보정용 위상 ROM 데이터[채널][방위][주파수] 멤버 변수 초기화
		memset(arrpBigArray[iAntIdx]->m_iRadicalDataFreqMHz,  0, sizeof(arrpBigArray[iAntIdx]->m_iRadicalDataFreqMHz)); //LMS_ADD_20220206

		fpFile = fopen(m_strAntFullFileName, "rt");

		fGetGenFreq = 0;
		fGetGenBearOld = 0;
		fGetGenBear = 0;
		fGetGenFreqOld = 0;
		uiBand = 0;
		uiFreqRangeStartMHzTemp = 0;
		uiFreqReadStartMHzTemp = 0;
		uiFreqReadEndMHzTemp = 0;
		

		if (fpFile == NULL)
		{
			TRACE("%s 매니폴드 파일을 읽을 수 없습니다!", m_strAntFullFileName);
			return FALSE;
		}
		else
		{
			//LMS_DEL_20190705 BeginWaitCursor();

			memset(arrpBigArray[iAntIdx]->m_usPhDfRomData,  0, sizeof(arrpBigArray[iAntIdx]->m_usPhDfRomData)); //LMS_MODIFY_20171222  memset(m_usPhDfRomData,  0, sizeof(m_usPhDfRomData));  // 방사보정용 위상 ROM 데이터[채널][방위][주파수] 멤버 변수 초기화

			CHAR szGetTemp[30];
			for (INT nTempIndex = 0; nTempIndex < COMINT_MAX_CHANNEL_COUNT+3; nTempIndex++)//LMS_MODIFY_20171221 for (INT nTempIndex = 0; nTempIndex < COMINT_MAX_CHANNEL_COUNT+2; nTempIndex++) // 파일에서 제목 타이틀 줄 읽어서 버림
				fscanf(fpFile, "%s", szGetTemp );

			UINT	uiGetPhase[DF_CH_NUM];//LMS_MODIFY_20220207 UINT	uiGetPhase[5];
			INT		iGetFileStaus;

			USHORT usAngleStep = (USHORT) 1; //(m_uiRomDataLoadStepAngle);

			//LMS_ADD_20171221
			UINT m_uiFreqRangeStartMHz = (BAND1_START_FREQ);//20;
			float m_fStartFreq = (float)(BAND1_START_FREQ);//20.f;
			UINT m_uiFreqRangeEndMHz = (FREQ_RANGE_END_MHz);//18000;
			float m_fStopFreq = (float)(FREQ_RANGE_END_MHz);//18000.f;
			float m_fStepFreq = 1.f;
			float m_fStartAngle = 0.f;
			float m_fStopAngle = 359.f;
			//LMS_ADD_20171221

			if( (m_uiFreqRangeStartMHz >= (BAND1_START_FREQ)  ) &&  //20
				(m_uiFreqRangeStartMHz <= (FREQ_RANGE_END_MHz))     // 8000
				)
			{
				m_fStartFreq = (float)m_uiFreqRangeStartMHz;
			} else {
				m_fStartFreq = (BAND1_START_FREQ) ;//20;
			}
			//LMS_ADD_20171221
			if( (m_uiFreqRangeEndMHz >= (BAND1_START_FREQ)   ) &&  //20
				(m_uiFreqRangeEndMHz <= (FREQ_RANGE_END_MHz))    //18000
				)
			{
				m_fStopFreq = (float)m_uiFreqRangeEndMHz;
			} else {
				m_fStopFreq = (FREQ_RANGE_END_MHz);//18000;
			}

			INT iSetFreqPt = 0;
			INT iSetAnglePt = 0;
			INT iRadicalAzimuthDegree = 0; //LMS_ADD_20220206

			INT iSetFreqTotalCount = 0;  //LMS_ADD_20220206
			INT iSetAngleTotalCount = 0; //LMS_ADD_20220206
			iSetAnglePt = 0;
			for (;;)//LMS_MODIFY_20220206 for (USHORT usSetFreq = (USHORT) m_fStartFreq; usSetFreq <= m_fStopFreq; usSetFreq += (USHORT) m_fStepFreq) // 주파수 루프 
			{

				iGetFileStaus =	fscanf(fpFile, "%d\t%f\t%f", &uiBand, &fGetGenFreq, &fGetGenBear);//LMS)MODIFY_20220207 iGetFileStaus =	fscanf(fpFile, "%d\t%f\t%f\t", &uiBand, &fGetGenFreq, &fGetGenBear);

				for (USHORT usChannel = 0; usChannel < COMINT_MAX_CHANNEL_COUNT; usChannel++){
					iGetFileStaus =	fscanf(fpFile, "\t%d", &uiGetPhase[usChannel]);
				}


				if (iGetFileStaus != EOF)
				{

					if(fGetGenBear < fGetGenBearOld){
						// 보정데이터 파일 로딩할때 방위각이 작아지면 ( ex 120도->0도) 다음주파수로 바뀐것으로 본다.
						//fGetGenBearOld = fGetGenBear;
						iSetAnglePt = 0;
						iSetFreqPt++; 

					} else if(fGetGenBearOld == 0){
						// 보정데이터 방위각0도->1도 읽을때 주파수값 업데이트
						arrpBigArray[iAntIdx]->m_iRadicalDataFreqMHz[iSetFreqPt] = (int)fGetGenFreq; //방사보정데이터 주파수(MHz)
					} else {
						iSetAnglePt++;	
					}
					fGetGenBearOld = fGetGenBear;


					for (USHORT usChannel = 0; usChannel < COMINT_MAX_CHANNEL_COUNT; usChannel++) // 채널 Loop
					{
						if((fGetGenBear >= 0.f          && fGetGenBear <= 360.f) && 
							(fGetGenFreq >= m_fStartFreq && fGetGenFreq <= m_fStopFreq)
							)
						{
							iRadicalAzimuthDegree = (int)fGetGenBear;
							arrpBigArray[iAntIdx]->m_usPhDfRomData[iSetFreqPt][iRadicalAzimuthDegree][usChannel] = (USHORT) uiGetPhase[usChannel];

						} else {
							TRACE("xband 방향탐지용 방사보정데이터 파일에서 방위각 또는 주파수가 정상범위가 아닙니다!!!");
						}
					}

				}
				else
				{
					break;
				}


				if (iGetFileStaus == EOF) //LMS_ADD_20220206
				{
					break;
				}

			}	

			fclose(fpFile); 
		}
	}

	return TRUE;
}

void CDFTaskMngr::SetDFCrectFreqList()
{
	string strFilePath;
	std::ifstream in;
	strFilePath = "C:\\IdexFreq\\LIG_index_freq.csv";
	int n_Index = 0, n_Freq = 0;
	STAutoFreqDataLIst stAutoFreqData;

	in.open(strFilePath, std::ifstream::in);	

	if(in.is_open() == true) {

		char buf[40] = {0, 0};
		int i = 0;
		while( in.good() == true )	//#FA_C_PotentialUnboundedLoop_T1
		{		
			in.getline(buf, CSV_BUF, ',');
			n_Index = _ttoi(buf);

			in.getline(buf, CSV_BUF, '\n');
			n_Freq = _ttoi(buf);

			stAutoFreqData.index = n_Index;
			stAutoFreqData.freq = n_Freq;	

			m_listDFFreqData.push_back(n_Freq);		

			if(in.eof() == true)
			{
				break;
			}
		}
		in.close();		
	}
}

int CDFTaskMngr::GetIndexFreq(UINT iFreq)
{	
	string strFilePath;
	std::ifstream in;
	strFilePath = "C:\\IdexFreq\\LIG_index_freq.csv";
	//strFilePath = "C:\\IdexFreq\\LIG_no3_index_freq_rev.csv";
	int n_Index = 0, n_Freq = 0;

	in.open(strFilePath, std::ifstream::in);	

	if(in.is_open() == true) {

		char buf[40] = {0, 0};
		int i = 0;
		while( in.good() == true )	//#FA_C_PotentialUnboundedLoop_T1
		{		
			in.getline(buf, CSV_BUF, ',');
			n_Index = _ttoi(buf);

			in.getline(buf, CSV_BUF, '\n');
			n_Freq = _ttoi(buf);

			if(iFreq == n_Freq)
			{
				break;
			}

			if(in.eof() == true)
			{
				break;
			}
		}
		in.close();		
	}

	return n_Index;
}

int CDFTaskMngr::GetAOADataFromAlgrism(UINT iFreq, int i_idxFreq, float * fchMeasPhDiffData)
{
	bool result = true;
	int iAOA;
	string strFilePath;
	std::ifstream in;
	strFilePath = "C:\\IdexFreq\\CHData_Done.csv";
	_CH_CORRECT nchCorrectData;
	float nTemp;
	float fchcalPHDiff[5];
	memcpy(fchcalPHDiff, fchMeasPhDiffData, sizeof(fchcalPHDiff));
	in.open(strFilePath, std::ifstream::in);	

	if(in.is_open() == true) {

		char buf[40] = {0, 0};
		int i = 0;
		while( in.good() == true )	//#FA_C_PotentialUnboundedLoop_T1
		{		
			in.getline(buf, CSV_BUF, ',');
			nchCorrectData.iFreq = _ttoi(buf);

			in.getline(buf, CSV_BUF, ',');
			nTemp = (float) _ttof(buf);
			nchCorrectData.fph[0] = nTemp;

			in.getline(buf, CSV_BUF, ',');
			nTemp = (float) _ttof(buf);
			nchCorrectData.fph[1] = nTemp;

			in.getline(buf, CSV_BUF, ',');
			nTemp = (float) _ttof(buf);
			nchCorrectData.fph[2] = nTemp;

			in.getline(buf, CSV_BUF, ',');
			nTemp = (float) _ttof(buf);
			nchCorrectData.fph[3] = nTemp;

			in.getline(buf, CSV_BUF, '\n');
			nTemp = (float) _ttof(buf);
			nchCorrectData.fph[4] = nTemp;	

			if(nchCorrectData.iFreq == iFreq)
			{
				break;
			}

			if(in.eof() == true)
			{
				break;
			}
		}
		in.close();		
	}

	TRACE("채널보정 ch_1 %f, ch_2 %f, ch_3 %f, ch_4 %f, ch_5 %f \n", nchCorrectData.fph[0],  nchCorrectData.fph[1], nchCorrectData.fph[2], nchCorrectData.fph[3], nchCorrectData.fph[4]);

	TRACE("측정데이터 Index %d, ch_1 %f, ch_2 %f, ch_3 %f, ch_4 %f, ch_5 %f \n", i_idxFreq, fchcalPHDiff[0],  fchcalPHDiff[1], fchcalPHDiff[2], fchcalPHDiff[3], fchcalPHDiff[4]);
	//이명식 수석님 함수 호출

	//iAOA = (int) ( DllDoCvDfAlgoOperation((INT)i_idxFreq, nchCorrectData.fph, fchcalPHDiff, pstBigArray) * 100.f );
	iAOA = (int) ( DllDoCvDfAlgoOperation((INT)i_idxFreq, nchCorrectData.fph, fchcalPHDiff, arrpBigArray[miAntPathDir], 0, 120) * 100.f );



	return iAOA;
}


int CDFTaskMngr::GetCloseDFFreq(UINT Freq)
{
	int nFreq = 0;
	int nBeforeIndex = 0;
	int bigDiff = 0;
	int MinDiff = 0;
	for(std::list<INT>::iterator it = m_listDFFreqData.begin(); it != m_listDFFreqData.end(); it++)
	{
		nFreq = *it;

		if(Freq == nFreq)
		{
			break;
		}

		if( nFreq >= (int)Freq )  
		{
			bigDiff = abs(nFreq  - (int)Freq);
			break;
		}		

		nBeforeIndex = nFreq;

	}

	MinDiff = abs(nBeforeIndex - (int)Freq);

	if(bigDiff > MinDiff) 
	{
		nFreq = nBeforeIndex;		
	}	

	return nFreq;
}

void CDFTaskMngr::InitFileFromStartChCorrect()
{
	ofstream fout;
	fout.open(m_strFilePath_Chennel, ios::out | ios::binary | ios::trunc);
	fout.close();
}

void CDFTaskMngr::StartChannelCorrect(int i_nModeType)
{
	if(i_nModeType == MODE_CH_TYPE || i_nModeType == MODE_INIT_TYPE)
	{
		m_iIncrCoretFreqCnt++;
	}	

	m_iMode  = i_nModeType;
	//V/UHF
	STAutoFreqDataLIst stAutoFreq = m_listAutoFreqData.front();
	m_Freq = stAutoFreq.freq;
	m_listAutoFreqData.pop_front();

	int opcode = MakeOPCode(CMDCODE_UHF_TX_SET_ANT_MODE, DEVICECODE_TRD, DEVICECODE_TVU);	

	//V/UHF 제어 설정
	STxUHFSetAntMode SetAntMode;
	SetAntMode.ucCalAntMode =0; //0 시작 : 1 : 종료
	m_hCommIF.Send(opcode, sizeof(SetAntMode), m_LinkInfo+1, Equip_Rev_VU, m_POSNIP, (void*)&SetAntMode); //134고유 ip끝번호 // 경로설정
	CString  strcommand;
	bool bfail = false;

	g_RcvTempFunc = &g_RcvFunc;
	strcommand.Format("SENSe:CALMode:STATe 1");
	bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);

	Sleep(100);
	int opcode2 = MakeOPCode(CMDCODE_UHF_TX_SET_CAL_FREQ, DEVICECODE_TRD, DEVICECODE_TVU);
	
	
	STxUHFSetCalFreq setCalFreq;	
	setCalFreq.uiCal_Freq = m_Freq * 1000;
	/*haeloox*/
	strcommand.Format("SENSe:CALMode:FREQuency %d", m_Freq);
	bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);
	//V/UHF 주파수 설정	
	//m_hCommIF.Send(opcode2, sizeof(setCalFreq),m_LinkInfo+1, Equip_Rev_VU, m_POSNIP, (void*)&setCalFreq); //134고유 ip끝번호 //주파수설정
	//Sleep(10);
	

	//레이더방탐 장비 제어
 	UINT64 uifrequency = (UINT64)((double)m_Freq * 1000000.0f);//Hz
	strcommand.Format("SENSe:FREQuency:CENTer %I64d\r\n", uifrequency );
	bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);
	//g_RcvTempFunc->Finish();
	
	TRACE("result_1 %d\n", bfail);

	strcommand.Format("SENSe:BANDwidth:RFConverter 1");
	bfail = g_RcvFunc.SCPI_CommendWrite(strcommand);
	TRACE("BANDWITH %d\n", bfail);

	INT64 iGain = -30;
	//INT64 iGain = 0;

	/*strcommand.Format("SENSe:GCONtrol %I64d", iGain);
	bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);
	TRACE("GCONtrol %d\n", bfail);*/
	Sleep(100);
	_PDW_COLECTSET set_collect;
	strcpy_s( (char *) set_collect.aucTaskID, sizeof(set_collect.aucTaskID), "채널보정_초기셋팅" );

	//8.1 이하 -15
	if(m_Freq <= 8100){

		set_collect.iRxThresholdValue = (-15+110)*4;
	}
	else{ //8.1 이상 -25
		set_collect.iRxThresholdValue = (-25+110)*4;
	}
	
	set_collect.uiAcquisitionTime = 100000000;
	set_collect.uiNumOfAcqPulse = 100;
	set_collect.uiNBDRBandwidth = 1;	
	set_collect.uiMode = ACQ_START;

	m_hCommIF.Send(OPCODE_TF_DP_ACQSET, sizeof(set_collect), m_LinkInfo+1, Equip_Rev_DP, m_POSNIP, (void*)&set_collect); //PDW 수집설정

	int opcode_done_status = MakeOPCode(CMDCODE_DF_RX_PDW_COLECT_DONE_STAT, DEVICECODE_TRD, DEVICECODE_TDP);

	_PDW_COLECTDONE_STAT_REQ  req_collStat;
	req_collStat.uiDoneStatus = 1;

	m_hCommIF.Send(opcode_done_status, sizeof(req_collStat), m_LinkInfo+1, Equip_Rev_DP, m_POSNIP, (void*)&req_collStat); //134고유 ip끝번호 //주파수설정
}

void CDFTaskMngr::GetAutoFreqChCorrect()
{
	string strFilePath;
	std::ifstream in;
	strFilePath = "C:\\IdexFreq\\LIG_Assign_freq.csv";	
	int n_Index = 0, n_Freq = 0;
	STAutoFreqDataLIst stAutoFreqData;

	in.open(strFilePath, std::ifstream::in);	

	if(in.is_open() == true) {

		char buf[40] = {0, 0};
		int i = 0;
		while( in.good() == true )	//#FA_C_PotentialUnboundedLoop_T1
		{		
			in.getline(buf, CSV_BUF, ',');
			n_Index = _ttoi(buf);

			in.getline(buf, CSV_BUF, '\n');
			n_Freq = _ttoi(buf);

			stAutoFreqData.index = n_Index;
			stAutoFreqData.freq = n_Freq;			
			
			m_listAutoFreqData.push_back(stAutoFreqData);				
			
			if(in.eof() == true)
			{
				break;
			}
		}
		in.close();		
	}
}

void CDFTaskMngr::SendReqInitRslt()
{
	bool bRtnSend = false;
	SRxInitReqRslt stInitReqRslt;
	stInitReqRslt.uiInitDFRslt = 1;
	stInitReqRslt.uiInitReciverRslt = 1;
	stInitReqRslt.uiInitPDWRslt = m_hCommIF.GetPDWCHCorrectConnStatus();

	bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTINIT, sizeof(stInitReqRslt), m_LinkInfo, Equip_Rev_BD, m_POSNIP, (void*)&stInitReqRslt); 
			
	if(bRtnSend == true)
	{
		TRACE("[송신]레이더방탐-레이더분석 초기화 결과\n");
	}
	else
	{
		TRACE("[송신 실패]레이더방탐-레이더분석 초기화 결과\n");
	}
}

void CDFTaskMngr::SendSptrCmdToPDW(int i_ModeType)
{	
	SRxSpectrumCmd sptrCmd;
	sptrCmd.uiReqSprtinfo = i_ModeType; //시작
	memcpy(sptrCmd.aucTaskID, m_stCurTaskData.aucTaskID, sizeof(m_stCurTaskData.aucTaskID));
	
	m_hCommIF.Send(OPCODE_TF_DP_REQSPTRUM, sizeof(sptrCmd), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&sptrCmd); //스펙트럼 시작요청
}

void CDFTaskMngr::SetPDWAcqCollect(STxAcqTraskData stAcqTaskData, int i_ModeType)
{	
	isRqTaskAcqSigFromPDW = false;
	_PDW_COLECTSET set_collect;
	memcpy(set_collect.aucTaskID, stAcqTaskData.aucTaskID,  sizeof(stAcqTaskData.aucTaskID));
	
	if(m_LinkInfo == LINK1_ID)
		set_collect.iRxThresholdValue = stAcqTaskData.iRxThresholdValue1;
	else if(m_LinkInfo == LINK2_ID)
		set_collect.iRxThresholdValue = stAcqTaskData.iRxThresholdValue2;
	else if(m_LinkInfo == LINK3_ID)
		set_collect.iRxThresholdValue = stAcqTaskData.iRxThresholdValue3;

	//set_collect.iRxThresholdValue = stAcqTaskData.iRxThresholdValue;
	set_collect.uiAcquisitionTime = stAcqTaskData.uiAcquisitionTime * TASK_ACQTIME ;
	set_collect.uiNumOfAcqPulse = stAcqTaskData.uiNumOfAcqPuls; 	
	set_collect.uiNBDRBandwidth = stAcqTaskData.uiNBDRBandWidth;
	set_collect.uiMode = i_ModeType; //시작 -0 , 중지 1

	m_hCommIF.Send(OPCODE_TF_DP_ACQSET, sizeof(set_collect), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&set_collect); 

	//if(i_ModeType == 1) //중지면 
	//{
	//	StopReqTaskRetryTimer();
	//}
	//else
	//{
	//	if(m_hRqTaskRetryTimerQueue == NULL)
	//	{							
	//		m_hRqTaskRetryTimerQueue = CreateTimerQueue();
	//		CreateTimerQueueTimer(&m_hRqTaskRetryTimer, m_hRqTaskRetryTimerQueue, (WAITORTIMERCALLBACK)ReqTaskRetryTimer, this, stAcqTaskData.uiAcquisitionTime*2, stAcqTaskData.uiAcquisitionTime*2, 0);
	//	}	
	//}
}

void CALLBACK ReqTaskRetryTimer(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	CDFTaskMngr* pThis = ((CDFTaskMngr*)lpParam);

	pThis->ReqTaskRetry();
}

void CDFTaskMngr::ReqTaskRetry()
{
	if(isRqTaskAcqSigFromPDW == false)
	{
		//STOPTIMER
		TRACE("=========================타이머 구동 ==================================================\n"); 
		StopReqTaskRetryTimer();

		_PDW_COLECTSET set_collect;
		strcpy_s( (char *) set_collect.aucTaskID, sizeof(set_collect.aucTaskID), (char *)m_stCurTaskData.aucTaskID );
	
		if(m_LinkInfo == LINK1_ID)
			set_collect.iRxThresholdValue = m_stCurTaskData.iRxThresholdValue1;
		else if(m_LinkInfo == LINK2_ID)
			set_collect.iRxThresholdValue = m_stCurTaskData.iRxThresholdValue2;
		else if(m_LinkInfo == LINK3_ID)
			set_collect.iRxThresholdValue = m_stCurTaskData.iRxThresholdValue3;

		//set_collect.iRxThresholdValue = m_stCurTaskData.iRxThresholdValue;
		set_collect.uiAcquisitionTime = m_stCurTaskData.uiAcquisitionTime;
		set_collect.uiNumOfAcqPulse = m_stCurTaskData.uiNumOfAcqPuls; 	
		set_collect.uiNBDRBandwidth = m_stCurTaskData.uiNBDRBandWidth;
		set_collect.uiMode = ACQ_START;
	}
}

void CDFTaskMngr::SetFreqToRadarUnit(UINT Freq)
{
	///////////////////////////////////////////////////////////////////////////////////
	///////////for test
//	UINT nFreq = Freq/1000;
//	int opcode = MakeOPCode(CMDCODE_UHF_TX_SET_ANT_MODE, DEVICECODE_TRD, DEVICECODE_TVU);	
//
//	//V/UHF 제어 설정
//	STxUHFSetAntMode SetAntMode;
//	SetAntMode.ucCalAntMode =0; //0 시작 : 1 : 종료
//	m_hCommIF.Send(opcode, sizeof(SetAntMode), m_LinkInfo+1, Equip_Rev_VU, m_POSNIP, (void*)&SetAntMode); //134고유 ip끝번호 // 경로설정
//
//	Sleep(100);
//	int opcode2 = MakeOPCode(CMDCODE_UHF_TX_SET_CAL_FREQ, DEVICECODE_TRD, DEVICECODE_TVU);
//
//	STxUHFSetCalFreq setCalFreq;	
//	setCalFreq.uiCal_Freq = nFreq * 1000;
//
//	//V/UHF 주파수 설정	
//	m_hCommIF.Send(opcode2, sizeof(setCalFreq),m_LinkInfo+1, Equip_Rev_VU, m_POSNIP, (void*)&setCalFreq); //134고유 ip끝번호 //주파수설정
//	Sleep(10);
//	
//	//레이더방탐 장비 제어
//	UINT64 uifrequency = (UINT64)((double)nFreq * 1000000.0f);//Hz
//	bool bfail = false;
//
//	CString  strcommand;
//	strcommand.Format("SENSe:FREQuency:CENTer %I64d", uifrequency );
//	bfail = g_RcvFunc.SCPI_CommendWrite(strcommand);
////	g_RcvTempFunc->Finish();
//
//	TRACE("result_1 %d\n", bfail);
//
//	INT64 iGain = -35;
//
//	strcommand.Format("SENSe:GCONtrol %I64d", iGain);
//	bfail = g_RcvFunc.SCPI_CommendWrite(strcommand);
////	g_RcvTempFunc->Finish();
//
//	uifrequency = 1;
//	strcommand.Format("SENSe:BANDwidth:RFConverter %I64d", uifrequency);
//	//bfail = g_RcvFunc.SCPI_CommendWrite(strcommand);
//	TRACE("BANDWITH %d\n", bfail);

	///////////////////////////////////////////////////////////////////////////////////
	///////////for test
	//while(1)
	//{
	//	int i;	
	//	STR_PDWDATA stPDWData;
	//	_PDW *pPDW;

	//	// 시작시 한번만 호출하면 됩니다.
	//	RadarDirAlgotirhm::RadarDirAlgotirhm::Init();

	//	//테스트 코드_START
	//	// 분석 프로그램 호출시 아래 함수를 콜하면 됩니다.
 //		strcpy( (char *) stPDWData.aucTaskID, "과제 무명" );
 //		stPDWData.iIsStorePDW = 1;			// 0 또는 1, PDW 저장되었으면 1로 설정함.
 //		stPDWData.iCollectorID = 0;			// 1, 2, 3 중에 하나이어야 한다. (수집소)
 //		stPDWData.count = 3;	
	//	stPDWData.enBandWidth = en50MHZ_BW;// 수집 PDW 개수는 최대 4096 개		
	//			
 //		pPDW = stPDWData.stPDW;
 //
 //		for( i=0 ; i < 3 ; ++i ) {

 //			pPDW->iFreq = 901000;
 //			pPDW->iAOA = 100;
 //			pPDW->iPulseType = 0;
 //			pPDW->iPA = -30;
 //			pPDW->iPW = 30;
	//		pPDW->iPFTag = 0;
 //			pPDW->llTOA = ( i * 5000 );
	//		pPDW->fPh1 = 0;
	//					pPDW->fPh2 = 0;
	//					pPDW->fPh3 = 0;
	//					pPDW->fPh4 = 0;
 //
 //			++ pPDW;
 //		}

	//	RadarDirAlgotirhm::RadarDirAlgotirhm::Start( & stPDWData );

	//			int nCoLOB=RadarDirAlgotirhm::RadarDirAlgotirhm::GetCoLOB();

	//			SRxLOBData *pLOBData=RadarDirAlgotirhm::RadarDirAlgotirhm::GetLOBData();
	/*
	float fch[5] = {0.0, 0.1, 0.2, 0.3, 0.4};
	GetAOADataFromAlgrism(nFreq, 0, fch);*/

	UINT nFreq = Freq/1000;
	int opcode = MakeOPCode(CMDCODE_UHF_TX_SET_ANT_MODE, DEVICECODE_TRD, DEVICECODE_TVU);	

	//V/UHF 제어 설정
	STxUHFSetAntMode SetAntMode;
	SetAntMode.ucCalAntMode = 1; //0 시작 : 1 : 종료
	m_hCommIF.Send(opcode, sizeof(SetAntMode), m_LinkInfo, Equip_Rev_VU, m_POSNIP, (void*)&SetAntMode); //134고유 ip끝번호 // 경로설정

	CString strcommand;
	bool bfail = false;
	g_RcvTempFunc = &g_RcvFunc;
	strcommand.Format("SENSe:CALMode:STATe 0");
	bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);

	//Add Onpoom haeloox 
	Sleep(10);
	strcommand.Format("SENSe:ANTenna:PATh %d", miAntPathDir);
	bfail = g_RcvTempFunc->SCPI_CommendWrite(strcommand);

	/*UINT nCloseFreq = GetCloseDFFreq(nFreq);
	nFreq = nCloseFreq;*/

	UINT64 uifrequency = (UINT64)((double)nFreq * 1000000.0f);//Hz
	strcommand.Format("SENSe:FREQuency:CENTer %I64d", uifrequency);
	bfail = g_RcvFunc.SCPI_CommendWrite(strcommand);
	TRACE("result_1 %d\n", bfail);

	//uifrequency = -35;
	/*
	uifrequency = (UINT64)m_RadarGain; //iRadarEqGain1 설정 변수
	strcommand.Format("SENSe:GCONtrol %I64d", uifrequency);
	bfail = g_RcvFunc.SCPI_CommendWrite(strcommand);
	Sleep(100);
	*/

	int iBwIdx = 0;
	if(m_stCurTaskData.uiNBDRBandWidth == 0)
	{
		iBwIdx = 2;
	}
	else
	{
		iBwIdx = 3;
	}
	
	strcommand.Format("SENSe:BANDwidth:RFConverter %d", iBwIdx);
	bfail = g_RcvFunc.SCPI_CommendWrite(strcommand);
	TRACE("BANDWITH %d\n", bfail);

	/*Sleep(3000);
	}*/
}

void CDFTaskMngr::SendToRadarAnlys(int i_opCode, int i_cmd)
{
	//int nPDWCnt = stColectDoneStat.iNumOfPDW;
	//int opcode_PDW = MakeOPCode(CMDCODE_DF_TX_PDW_REQ, DEVICECODE_TRD, DEVICECODE_TDP);

	//_PDW_DATA_Req req_PDWData;		 
	//strcpy( (char *) req_PDWData.aucTaskID, "채널보정_초기셋팅" );
	//req_PDWData.iPDWCnt = 10;

	//m_hCommIF.Send(opcode_PDW, sizeof(req_PDWData), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&req_PDWData); //134고유 ip끝번호 //주파수설정

	//switch(i_cmd)
	//{
	//	case 
	//}

	//m_hCommIF.Send(i_opCode, sizeof(stSystemVal), m_LinkInfo, Equip_Rev_DP, m_POSNIP, (void*)&stSystemVal); 
}

CString CDFTaskMngr::GetModulePath()
{
	char szPath[MAX_PATH];
	memset(szPath, 0x00, MAX_PATH);

	::GetModuleFileName(NULL, szPath, MAX_PATH);

	CString sTempPath = szPath;	

	//int iLength = sTempPath.GetLength();
	int iPos = sTempPath.ReverseFind(TCHAR('\\'));

	CString sModulePath = sTempPath.Left(iPos);
	sModulePath += "\\";

	return sModulePath;
}

void CDFTaskMngr::AlgrismInit()
{
	TRACE("[수신]레이더분석-레이더방탐 IQ 시작/중지 요구\n");	

	bool bRtnSend = false;

	_IQ_REQ_TO_PDW stIQReq;
	CString tempTask;

	memset(stIQReq.aucTaskID, 0, 20);
	//SaveAll_IQFile();

	//return;
	if(m_bTurnButten == false)
	{
		m_bTurnButten = true;

		tempTask = "testTask3";
		memcpy(stIQReq.aucTaskID, tempTask.GetBuffer(), tempTask.GetLength());

		stIQReq.uiCollectorID = 0;
		stIQReq.uiReqIQInfo = 1;
		//memcpy(&stIQReq, i_stMsg.buf, i_stMsg.usMSize);

		if(stIQReq.uiReqIQInfo == 1)   //IQ 시작이면 
		{
			m_nStep = 0;
		}	

		bRtnSend = m_hCommIF.Send(OPCODE_TF_DP_REQSIQ, sizeof(stIQReq), m_LinkInfo+1, Equip_Rev_DP, m_POSNIP, &stIQReq); 	

	}
	else if(m_bTurnButten == true)
	{
		m_bTurnButten = false;

		tempTask = "testTask";
		memcpy(stIQReq.aucTaskID, tempTask.GetBuffer(), tempTask.GetLength());

		stIQReq.uiCollectorID = 0;
		stIQReq.uiReqIQInfo = 2;
		//memcpy(&stIQReq, i_stMsg.buf, i_stMsg.usMSize);

		if(stIQReq.uiReqIQInfo == 2)   //IQ 중지이면 
		{
			m_nStep = 0;
		}	

		bRtnSend = m_hCommIF.Send(OPCODE_TF_DP_REQSIQ, sizeof(stIQReq), m_LinkInfo+1, Equip_Rev_DP, m_POSNIP, &stIQReq); 	
	}


	if(bRtnSend == true)
	{
		TRACE("[송신]레이더방탐-레이더분석 수집시작 결과\n");
	}
	else
	{
		TRACE("[송신 실패]레이더방탐-레이더분석 수집시작 결과\n");
	}
}

void CDFTaskMngr::SaveAll_IQFile()
{
	EnterCriticalSection(&m_csIQFile);
	UINT nSizeNum = m_IQDataStore.size();
	LeaveCriticalSection(&m_csIQFile);	

	while (nSizeNum > 0)
	{
		STR_IQDATA stIQData;
		EnterCriticalSection(&m_csIQFile);
		nSizeNum = m_IQDataStore.size();

		if(nSizeNum > 0)
		{
			stIQData = m_IQDataStore.front();
			m_IQDataStore.pop_front();	
		}

		LeaveCriticalSection(&m_csIQFile);

		if(nSizeNum != 0)
		{
			CFile cFile;
			BOOL bRet;
			//CString strIPAddress=GetIpAddress();

			sprintf_s( m_szDirectory, "%s\\수집소_%d\\%s", LOCAL_DATA_DIRECTORY, m_LinkInfo+1, stIQData.aucTaskID );

			bRet = CreateDir( m_szDirectory );

			struct tm stTime;
			char buffer[100];
			__time32_t tiNow;

			tiNow = _time32(NULL);

			_localtime32_s( &stTime, & tiNow );

			if(m_bChStep ==  false)
			{
				strftime( buffer, 100, "%Y-%m-%d %H_%M_%S", & stTime);
				wsprintf(m_filename, _T("%s/_COL%d_%s_%05d.%s"), m_szDirectory, stIQData.iCollectorID, buffer, m_nStep, IQ_EXT );
				m_bChStep = true;
			}

			//strftime( buffer, 100, "%Y-%m-%d %H_%M_%S", & stTime);

			int nSize = stIQData.count * sizeof(_IQ);
			m_uiIQSize += nSize;
			
			if(m_uiIQSize > IQ_PACKET_SIZE * 10 )
			{				
				m_nStep++;
				m_uiIQSize -= IQ_PACKET_SIZE * 10 ;
				strftime( buffer, 100, "%Y-%m-%d %H_%M_%S", & stTime);	
				wsprintf( m_filename, _T("%s/_COL%d_%s_%05d.%s"), m_szDirectory, stIQData.iCollectorID, buffer, m_nStep, IQ_EXT );
			}

			//wsprintf( filename, _T("%s/_COL%d_%s_%05d.%s"), szDirectory, stIQData.iCollectorID, buffer, m_nStep, IQ_EXT );

			cFile.Open( m_filename, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite );
			cFile.SeekToEnd();
			
			//int nSize = sizeof( STR_IQDATA ) - ( ( _MAX_IQ - stIQData.count ) * sizeof(_IQ) );
			if(nSize != 0)
			{
				cFile.Write( &stIQData.stIQ, nSize );
			}
			
			//LeaveCriticalSection(&m_csIQFile);
			cFile.Close();
		}
	}

	TRACE("=****[수신]PDW-레이더방탐 IQ결과 FILE 저장 완료=**** \n");
}

BOOL CDFTaskMngr::CreateDir( char *pPath )
{
	BOOL bRet;
	char dirName[256];
	char *p=pPath;
	char *q=dirName;

	while( *p ) {
		if( ('\\' == *p) || ('/'==*p)) {
			if( ':' != *(p-1) ) {
				CreateDirectory( dirName, NULL );
			}
		}

		*q++ = *p++;
		*q = '\0';
	}
	bRet = CreateDirectory( dirName, NULL );

	return bRet;
}


int CDFTaskMngr::gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970 
	static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime( &system_time );
	SystemTimeToFileTime( &system_time, &file_time );
	time =  ((uint64_t)file_time.dwLowDateTime )      ;
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
	tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
	return 0;

}