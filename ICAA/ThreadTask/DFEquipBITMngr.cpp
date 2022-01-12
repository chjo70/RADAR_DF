#include "stdafx.h"
#include "DFEquipBITMngr.h"
#include "..\MsgQueueThread.h"

// 레이더분석-레이더방탐 OPCODE
#define OPCODE_BD_TF_REQINIT		0x02820100		// 레이더분석-레이더방탐 초기화 요구
#define OPCODE_BD_TF_REQSWVER		0x02820200		// 레이더분석-레이더방탐 SW버전 요구
#define OPCODE_BD_TF_REQPBIT		0x02820300		// 레이더분석-레이더방탐 초기자체점검 요구
#define OPCODE_BD_TF_REQIBIT		0x02820400		// 레이더분석-레이더방탐 운용자자체점검 요구


// 레이더방탐-레이더분석 OPCODE
#define OPCODE_TF_BD_RSTINIT		0x82020100		// 레이더방탐-레이더분석 초기화 결과
#define OPCODE_TF_BD_RSTSWVER		0x82020200		// 레이더방탐-레이더분석 SW버전 결과
#define OPCODE_TF_BD_RSTPBIT		0x82020300		// 레이더방탐-레이더분석 초기자체점검 결과
#define OPCODE_TF_BD_RSTIBIT		0x82020400		// 레이더방탐-레이더분석 운용자자체점검 결과

CDFEquipBITMngr* CDFEquipBITMngr::m_pInstance=nullptr;

CDFEquipBITMngr::CDFEquipBITMngr()
:m_hCommIF_DFEquipBITMngr(m_hCommIF)
{
	//초기화 결과
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_INIT_SYS_RESULT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//SW버전 결과
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_SW_VERSION_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//초기자체점검 결과	
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_PBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//운용자자체점검 결과				
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_IBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	////채널보정 진행상태				
	//m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_CHNEL_CORECT, DEVICECODE_BRA, DEVICECODE_TRD), this);				
}

CDFEquipBITMngr::~CDFEquipBITMngr()
{
	//초기화 결과 
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_INIT_SYS_RESULT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//SW버전 결과
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_SW_VERSION_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//초기자체점검 결과	
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_PBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//운용자자체점검 결과				
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_IBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);
}

CDFEquipBITMngr* CDFEquipBITMngr::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CDFEquipBITMngr();		
	}

	return m_pInstance;
}

void CDFEquipBITMngr::Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData)
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

void CDFEquipBITMngr::SetDFPoint(void* pPoint)
{
	pDFMngr = (CDFTaskMngr*)pPoint;
}

void CDFEquipBITMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE별로 경우를 나열하여 처리하는 로직 구현 필요
	// 하단에 구현
		
	TRACE("[Log] CDFEquipBITMngr ProcessMsg 호출.\n");
	bool bRtnSend = false;

	/*UEL_BITMAP_OPCode stOpCode= UEL_BITMAP_OPCode() ;
	stOpCode.w32 = i_stMsg.uiOpcode;*/


	switch(i_stMsg.uiOpcode)
	{
	//초기화 결과
	case OPCODE_BD_TF_REQINIT:
		{			
			TRACE("[수신]레이더분석-레이더방탐 초기화 요구\n");			
			SRxInitReqRslt stInitReqRslt;

			/*if(pDFMngr->m_bEndChannelCorrect == false)
			{
				pDFMngr->m_SetReqInitFlag = true;
			}*/
			//else //레이더방탐 보정완료 되면 초기화결과 전송
			{

				if(m_hCommIF.GetRadarEquipConnStatus() == false)
				{
					stInitReqRslt.uiInitDFRslt = 1;  //레이더방탐
					stInitReqRslt.uiInitReciverRslt = 1; //수신기
				}		

				if(m_hCommIF.GetPDWEquipConnStatus() == false)
					stInitReqRslt.uiInitPDWRslt = 1; //PDW발생판

				bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTINIT, sizeof(stInitReqRslt), pDFMngr->m_LinkInfo, Equip_Rev_BD, pDFMngr->m_POSNIP, (void*)&stInitReqRslt); 
			
				if(bRtnSend == true)
				{
					TRACE("[송신]레이더방탐-레이더분석 초기화 결과\n");
				}
				else
				{
					TRACE("[송신 실패]레이더방탐-레이더분석 초기화 결과\n");
				}
			}
		}
		break;
	//SW버전 결과
	case OPCODE_BD_TF_REQSWVER:
		{		
			TRACE("[수신]레이더분석-레이더방탐 SW버전 요구\n");
			SRxInitReqRslt stSWVer;

			stSWVer.uiInitDFRslt = 3;
			stSWVer.uiInitReciverRslt = 3;
			stSWVer.uiInitPDWRslt = 3;
			
			bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTSWVER, sizeof(stSWVer), pDFMngr->m_LinkInfo, Equip_Rev_BD, pDFMngr->m_POSNIP, (void*)&stSWVer); //134고유 ip끝번호 //주파수설정

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-레이더분석 SW버전 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 SW버전 결과\n");
			}
		}
		break;
		//초기자체점검 결과	
	case OPCODE_BD_TF_REQPBIT:
		{
			TRACE("[수신]레이더분석-레이더방탐 초기자체점검 요구\n");

			SRxBITReqRslt stPBITRst;
			//SRxInitReqRslt stPBIT;

			CString  strcommand;
			strcommand.Format("TEST:BOARd:STATe?");
			CString strPBITRslt = g_RcvFunc.SCPI_CommendQuery(strcommand);

			stPBITRst.uiInitDFRslt = 0;

			if(m_hCommIF.GetPDWEquipConnStatus() == true)
				stPBITRst.uiInitPDWRslt = 0;

			unsigned int iPBIcTRslt = (UINT)_atoi64(strPBITRslt);
			
			if((iPBIcTRslt & 0x03) == 0x00000001) //정상
				stPBITRst.uiRcvCH1 = 0;
			
			if((iPBIcTRslt & 0x30) == 0x00000020)
				stPBITRst.uiRcvCH2 = 0;

			if((iPBIcTRslt & 0x300)  == 0x00000100) //정상
				stPBITRst.uiRcvSYN = 0;

			if((iPBIcTRslt & 0x30000) == 0x00010000)
				stPBITRst.uiRcvLO1 = 0;

			if((iPBIcTRslt & 0xC0000) == 0x00040000)
				stPBITRst.uiRcvLO2 = 0;

			if((iPBIcTRslt & 0x300000) == 0x00100000)
				stPBITRst.uiRcvCH3 = 0;

			if((iPBIcTRslt & 0x3000000) == 0x01000000)
				stPBITRst.uiRcvCH4 = 0;
		
			if((iPBIcTRslt & 0x30000000) == 0x10000000)
				stPBITRst.uiRcvCH5 = 0;
			

			/*stPBIT.uiInitDFRslt = 1;
			stPBIT.uiInitReciverRslt = 1;
			stPBIT.uiInitPDWRslt = 1;*/

			bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTPBIT, sizeof(stPBITRst), pDFMngr->m_LinkInfo, Equip_Rev_BD, pDFMngr->m_POSNIP, (void*)&stPBITRst); //134고유 ip끝번호 //주파수설정
			
			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-레이더분석 SW버전 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 SW버전 결과\n");
			}
		}
		break;
	//운용자자체점검 결과				
	case OPCODE_BD_TF_REQIBIT:
		{
			TRACE("[수신]레이더분석-레이더방탐 운용자자체점검 요구\n");

			SRxBITReqRslt stIBITRst;
			//SRxInitReqRslt stPBIT;

			CString  strcommand;
			strcommand.Format("TEST:BOARd:STATe?");
			CString strIBITRslt = g_RcvFunc.SCPI_CommendQuery(strcommand);

			stIBITRst.uiInitDFRslt = 0;

			if(m_hCommIF.GetPDWEquipConnStatus() == true)
				stIBITRst.uiInitPDWRslt = 0;

			unsigned int iIBIcTRslt = (UINT)_atoi64(strIBITRslt);

			if((iIBIcTRslt & 0x03) == 0x00000001) //정상
				stIBITRst.uiRcvCH1 = 0;

			if((iIBIcTRslt & 0x30) == 0x00000020)
				stIBITRst.uiRcvCH2 = 0;

			if((iIBIcTRslt & 0x300)  == 0x00000100) //정상
				stIBITRst.uiRcvSYN = 0;

			if((iIBIcTRslt & 0x30000) == 0x00010000)
				stIBITRst.uiRcvLO1 = 0;

			if((iIBIcTRslt & 0xC0000) == 0x00040000)
				stIBITRst.uiRcvLO2 = 0;

			if((iIBIcTRslt & 0x300000) == 0x00100000)
				stIBITRst.uiRcvCH3 = 0;

			if((iIBIcTRslt & 0x3000000) == 0x01000000)
				stIBITRst.uiRcvCH4 = 0;

			if((iIBIcTRslt & 0x30000000) == 0x10000000)
				stIBITRst.uiRcvCH5 = 0;

			/*SRxInitReqRslt stIBIT;
			stIBIT.uiInitDFRslt = 1;
			stIBIT.uiInitReciverRslt = 1;
			stIBIT.uiInitPDWRslt = 1;*/

			bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTIBIT, sizeof(stIBITRst), pDFMngr->m_LinkInfo, Equip_Rev_BD, pDFMngr->m_POSNIP, (void*)&stIBITRst); //134고유 ip끝번호 //주파수설정

			if(bRtnSend == true)
			{
				TRACE("[송신]레이더방탐-레이더분석 운용자자체점검 결과\n");
			}
			else
			{
				TRACE("[송신 실패]레이더방탐-레이더분석 운용자자체점검 결과\n");
			}

		}
		break;
	
	default:
		break;
	}
}
