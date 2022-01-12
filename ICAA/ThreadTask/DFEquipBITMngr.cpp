#include "stdafx.h"
#include "DFEquipBITMngr.h"
#include "..\MsgQueueThread.h"

// ���̴��м�-���̴���Ž OPCODE
#define OPCODE_BD_TF_REQINIT		0x02820100		// ���̴��м�-���̴���Ž �ʱ�ȭ �䱸
#define OPCODE_BD_TF_REQSWVER		0x02820200		// ���̴��м�-���̴���Ž SW���� �䱸
#define OPCODE_BD_TF_REQPBIT		0x02820300		// ���̴��м�-���̴���Ž �ʱ���ü���� �䱸
#define OPCODE_BD_TF_REQIBIT		0x02820400		// ���̴��м�-���̴���Ž �������ü���� �䱸


// ���̴���Ž-���̴��м� OPCODE
#define OPCODE_TF_BD_RSTINIT		0x82020100		// ���̴���Ž-���̴��м� �ʱ�ȭ ���
#define OPCODE_TF_BD_RSTSWVER		0x82020200		// ���̴���Ž-���̴��м� SW���� ���
#define OPCODE_TF_BD_RSTPBIT		0x82020300		// ���̴���Ž-���̴��м� �ʱ���ü���� ���
#define OPCODE_TF_BD_RSTIBIT		0x82020400		// ���̴���Ž-���̴��м� �������ü���� ���

CDFEquipBITMngr* CDFEquipBITMngr::m_pInstance=nullptr;

CDFEquipBITMngr::CDFEquipBITMngr()
:m_hCommIF_DFEquipBITMngr(m_hCommIF)
{
	//�ʱ�ȭ ���
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_INIT_SYS_RESULT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//SW���� ���
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_SW_VERSION_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//�ʱ���ü���� ���	
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_PBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//�������ü���� ���				
	m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_IBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	////ä�κ��� �������				
	//m_hCommIF.RegisterOpcode(MakeOPCode(CMDCODE_DF_TX_CHNEL_CORECT, DEVICECODE_BRA, DEVICECODE_TRD), this);				
}

CDFEquipBITMngr::~CDFEquipBITMngr()
{
	//�ʱ�ȭ ��� 
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_INIT_SYS_RESULT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//SW���� ���
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_SW_VERSION_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//�ʱ���ü���� ���	
	m_hCommIF.UnregisterOpcode(MakeOPCode(CMDCODE_DF_TX_PBIT_RSLT, DEVICECODE_BRA, DEVICECODE_TRD), this);	
	//�������ü���� ���				
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

	// ���Ŵܿ��� Msg ������ ����
	STMsg stInstanceMsg;

	stInstanceMsg.uiOpcode = i_uiOpcode;
	stInstanceMsg.usMSize = i_usSize;
	stInstanceMsg.ucRevOperID = i_ucRevOperID;
	stInstanceMsg.ucLinkID = i_ucLink;
	stInstanceMsg.ucOperatorID = ucOpertorID;

	memcpy(stInstanceMsg.buf, i_pvData, i_usSize);

	// ������ ó���� ���� Msg Queue�� ����ֱ�
	PushMsg(stInstanceMsg); 

	m_cond.LIGSetEvent();
}

void CDFEquipBITMngr::SetDFPoint(void* pPoint)
{
	pDFMngr = (CDFTaskMngr*)pPoint;
}

void CDFEquipBITMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE���� ��츦 �����Ͽ� ó���ϴ� ���� ���� �ʿ�
	// �ϴܿ� ����
		
	TRACE("[Log] CDFEquipBITMngr ProcessMsg ȣ��.\n");
	bool bRtnSend = false;

	/*UEL_BITMAP_OPCode stOpCode= UEL_BITMAP_OPCode() ;
	stOpCode.w32 = i_stMsg.uiOpcode;*/


	switch(i_stMsg.uiOpcode)
	{
	//�ʱ�ȭ ���
	case OPCODE_BD_TF_REQINIT:
		{			
			TRACE("[����]���̴��м�-���̴���Ž �ʱ�ȭ �䱸\n");			
			SRxInitReqRslt stInitReqRslt;

			/*if(pDFMngr->m_bEndChannelCorrect == false)
			{
				pDFMngr->m_SetReqInitFlag = true;
			}*/
			//else //���̴���Ž �����Ϸ� �Ǹ� �ʱ�ȭ��� ����
			{

				if(m_hCommIF.GetRadarEquipConnStatus() == false)
				{
					stInitReqRslt.uiInitDFRslt = 1;  //���̴���Ž
					stInitReqRslt.uiInitReciverRslt = 1; //���ű�
				}		

				if(m_hCommIF.GetPDWEquipConnStatus() == false)
					stInitReqRslt.uiInitPDWRslt = 1; //PDW�߻���

				bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTINIT, sizeof(stInitReqRslt), pDFMngr->m_LinkInfo, Equip_Rev_BD, pDFMngr->m_POSNIP, (void*)&stInitReqRslt); 
			
				if(bRtnSend == true)
				{
					TRACE("[�۽�]���̴���Ž-���̴��м� �ʱ�ȭ ���\n");
				}
				else
				{
					TRACE("[�۽� ����]���̴���Ž-���̴��м� �ʱ�ȭ ���\n");
				}
			}
		}
		break;
	//SW���� ���
	case OPCODE_BD_TF_REQSWVER:
		{		
			TRACE("[����]���̴��м�-���̴���Ž SW���� �䱸\n");
			SRxInitReqRslt stSWVer;

			stSWVer.uiInitDFRslt = 3;
			stSWVer.uiInitReciverRslt = 3;
			stSWVer.uiInitPDWRslt = 3;
			
			bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTSWVER, sizeof(stSWVer), pDFMngr->m_LinkInfo, Equip_Rev_BD, pDFMngr->m_POSNIP, (void*)&stSWVer); //134���� ip����ȣ //���ļ�����

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴���Ž-���̴��м� SW���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴���Ž-���̴��м� SW���� ���\n");
			}
		}
		break;
		//�ʱ���ü���� ���	
	case OPCODE_BD_TF_REQPBIT:
		{
			TRACE("[����]���̴��м�-���̴���Ž �ʱ���ü���� �䱸\n");

			SRxBITReqRslt stPBITRst;
			//SRxInitReqRslt stPBIT;

			CString  strcommand;
			strcommand.Format("TEST:BOARd:STATe?");
			CString strPBITRslt = g_RcvFunc.SCPI_CommendQuery(strcommand);

			stPBITRst.uiInitDFRslt = 0;

			if(m_hCommIF.GetPDWEquipConnStatus() == true)
				stPBITRst.uiInitPDWRslt = 0;

			unsigned int iPBIcTRslt = (UINT)_atoi64(strPBITRslt);
			
			if((iPBIcTRslt & 0x03) == 0x00000001) //����
				stPBITRst.uiRcvCH1 = 0;
			
			if((iPBIcTRslt & 0x30) == 0x00000020)
				stPBITRst.uiRcvCH2 = 0;

			if((iPBIcTRslt & 0x300)  == 0x00000100) //����
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

			bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTPBIT, sizeof(stPBITRst), pDFMngr->m_LinkInfo, Equip_Rev_BD, pDFMngr->m_POSNIP, (void*)&stPBITRst); //134���� ip����ȣ //���ļ�����
			
			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴���Ž-���̴��м� SW���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴���Ž-���̴��м� SW���� ���\n");
			}
		}
		break;
	//�������ü���� ���				
	case OPCODE_BD_TF_REQIBIT:
		{
			TRACE("[����]���̴��м�-���̴���Ž �������ü���� �䱸\n");

			SRxBITReqRslt stIBITRst;
			//SRxInitReqRslt stPBIT;

			CString  strcommand;
			strcommand.Format("TEST:BOARd:STATe?");
			CString strIBITRslt = g_RcvFunc.SCPI_CommendQuery(strcommand);

			stIBITRst.uiInitDFRslt = 0;

			if(m_hCommIF.GetPDWEquipConnStatus() == true)
				stIBITRst.uiInitPDWRslt = 0;

			unsigned int iIBIcTRslt = (UINT)_atoi64(strIBITRslt);

			if((iIBIcTRslt & 0x03) == 0x00000001) //����
				stIBITRst.uiRcvCH1 = 0;

			if((iIBIcTRslt & 0x30) == 0x00000020)
				stIBITRst.uiRcvCH2 = 0;

			if((iIBIcTRslt & 0x300)  == 0x00000100) //����
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

			bRtnSend = m_hCommIF.Send(OPCODE_TF_BD_RSTIBIT, sizeof(stIBITRst), pDFMngr->m_LinkInfo, Equip_Rev_BD, pDFMngr->m_POSNIP, (void*)&stIBITRst); //134���� ip����ȣ //���ļ�����

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴���Ž-���̴��м� �������ü���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴���Ž-���̴��м� �������ü���� ���\n");
			}

		}
		break;
	
	default:
		break;
	}
}
