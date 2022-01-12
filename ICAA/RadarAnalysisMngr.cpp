#include "stdafx.h"
#include "RadarAnalysisMngr.h"
#include "ThreadTask/DFMsgDefnt.h"

// ���-���̴��м� OPCODE
#define OPCODE_OV_BD_REQINIT		0x01020100		// ���-���̴��м� �ʱ�ȭ �䱸
#define OPCODE_OV_BD_REQSWVER		0x01020200		// ���-���̴��м� SW���� �䱸
#define OPCODE_OV_BD_REQPBIT		0x01020300		// ���-���̴��м� �ʱ���ü���� �䱸
#define OPCODE_OV_BD_REQIBIT		0x01020400		// ���-���̴��м� �������ü���� �䱸
#define OPCODE_OV_BD_REQCHCALIB		0x01020500		// ���-���̴��м� ä�� ���� �䱸

#define OPCODE_OV_BD_REQSTARTACQ	0x01021100		// ���-���̴��м� �������� �䱸
#define OPCODE_OV_BD_SETRESET		0x01021200		// ���-���̴��м� ��� ���� �䱸
#define OPCODE_OV_BD_CONTROLRECVR	0x01021300		// ���-���̴��м� ���ű� ����
#define OPCODE_OV_BD_REQSYSTEMSET	0x01021400		// ���-���̴��м� ���̴���Ž �ý��ۺ��� ���� �䱸
#define OPCODE_OV_BD_REQCHANGEEMIT	0x01021600		// ���-���̴��м� ���̴�/���� ���� �䱸

#define OPCODE_OV_BD_REQACQTASK		0x01022100		// ���-���̴��м� ���� ���� �䱸
#define OPCODE_OV_BD_REQREJFREQSET	0x01022200		// ���-���̴��м� ���Ź������ļ� ���� �䱸
#define OPCODE_OV_BD_SETSYSTEM		0x01022300		// ���-���̴��м� �ý��� ����

#define OPCODE_OV_BD_SENDERROR		0x0102F100		// ���-���̴��м� ���� ����

// ���̴��м�-��� OPCODE
#define OPCODE_BD_OV_RSTINIT		0x02010100		// ���̴��м�-��� �ʱ�ȭ ���
#define OPCODE_BD_OV_RSTSWVER		0x02010200		// ���̴��м�-��� SW���� ���
#define OPCODE_BD_OV_RSTPBIT		0x02010300		// ���̴��м�-��� �ʱ���ü���� ���
#define OPCODE_BD_OV_RSTIBIT		0x02010400		// ���̴��м�-��� �������ü���� ���
#define OPCODE_BD_OV_STATECHCALIB	0x02010500		// ���̴��м�-��� ä�� ���� ����

#define OPCODE_BD_OV_RSTSTARTACQ	0x02011100		// ���̴��м�-��� �������� ���
#define OPCODE_BD_OV_RSTRESETEQUIP	0x02011200		// ���̴��м�-��� ��� ���� ���
#define OPCODE_BD_OV_RSTRECVCONTROL	0x02011300		// ���̴��м�-��� ���ű� ���� ���
#define OPCODE_BD_OV_RSTSYSTEMSET	0x02011400		// ���̴��м�-��� ���̴���Ž �ý��ۺ��� ���� ���
#define OPCODE_BD_OV_RSTCHANGEEMIT	0x02011600		// ���̴��м�-��� ���̴�/���� ���� ���
#define OPCODE_BD_OV_STATETASK		0x02011700		// ���̴��м�-��� ���� ���� ����

#define OPCODE_BD_OV_RSTACQTASK		0x02012100		// ���̴��м�-��� �������� ���Ű��
#define OPCODE_BD_OV_SENDLOB		0x02013100		// ���̴��м�-��� LOB ����
#define OPCODE_BD_OV_SENDBEAM		0x02013300		// ���̴��м�-��� �� ����

#define OPCODE_BD_OV_SENDERROR		0x0201F100		// ���̴��м�-��� ���� ����

// ���̴��м�-���̴���Ž OPCODE
#define OPCODE_BD_TF_REQINIT		0x02820100		// ���̴��м�-���̴���Ž �ʱ�ȭ �䱸
#define OPCODE_BD_TF_REQSWVER		0x02820200		// ���̴��м�-���̴���Ž SW���� �䱸
#define OPCODE_BD_TF_REQPBIT		0x02820300		// ���̴��м�-���̴���Ž �ʱ���ü���� �䱸
#define OPCODE_BD_TF_REQIBIT		0x02820400		// ���̴��м�-���̴���Ž �������ü���� �䱸
#define OPCODE_BD_TF_REQCHCALIB		0x02820500		// ���̴��м�-���̴���Ž ä�� ���� �䱸

#define OPCODE_BD_TF_REQSTARTACQ	0x02821100		// ���̴��м�-���̴���Ž �������� �䱸
#define OPCODE_BD_TF_SETRESET		0x02821200		// ���̴��м�-���̴���Ž ��� ���� �䱸
#define OPCODE_BD_TF_CONTROLRECVR	0x02821300		// ���̴��м�-���̴���Ž ���ű� ����
#define OPCODE_BD_TF_REQSYSTEMSET	0x02821400		// ���̴��м�-���̴���Ž ���̴���Ž �ý��ۺ��� ���� �䱸

#define OPCODE_BD_TF_REQACQTASK		0x02822100		// ���̴��м�-���̴���Ž ���� ���� �䱸
#define OPCODE_BD_TF_REQREJFREQSET	0x02822200		// ���̴��м�-���̴���Ž ���Ź������ļ� ���� �䱸
#define OPCODE_BD_TF_SETSYSTEM		0x02822300		// ���̴��м�-���̴���Ž �ý��� ����

#define OPCODE_BD_TF_SENDERROR		0x0282F100		// ���̴��м�-���̴���Ž ���� ����

// ���̴���Ž-���̴��м� OPCODE
#define OPCODE_TF_BD_RSTINIT		0x82020100		// ���̴���Ž-���̴��м� �ʱ�ȭ ���
#define OPCODE_TF_BD_RSTSWVER		0x82020200		// ���̴���Ž-���̴��м� SW���� ���
#define OPCODE_TF_BD_RSTPBIT		0x82020300		// ���̴���Ž-���̴��м� �ʱ���ü���� ���
#define OPCODE_TF_BD_RSTIBIT		0x82020400		// ���̴���Ž-���̴��м� �������ü���� ���
#define OPCODE_TF_BD_STATECHCALIB	0x82020500		// ���̴���Ž-���̴��м� ä�� ���� ����

#define OPCODE_TF_BD_RSTSTARTACQ	0x82021100		// ���̴���Ž-���̴��м� �������� ���
#define OPCODE_TF_BD_RSTRESETEQUIP	0x82021200		// ���̴���Ž-���̴��м� ��� ���� ���
#define OPCODE_TF_BD_RSTRECVCONTROL	0x82021300		// ���̴���Ž-���̴��м� ���ű� ���� ���
#define OPCODE_TF_BD_RSTSYSTEMSET	0x82021400		// ���̴���Ž-���̴��м� ���̴���Ž �ý��ۺ��� ���� ���
#define OPCODE_TF_BD_STATETASK		0x82021700		// ���̴���Ž-���̴��м� ���� ���� ����
#define OPCODE_TF_BD_RSTACQTASK		0x82022100		// ���̴���Ž-���̴��м� �������� ���Ű��

#define OPCODE_TF_BD_SENDLOB		0x82023100		// ���̴���Ž-���̴��м� LOB ����
#define OPCODE_TF_BD_SENDPDW		0x82023200		// ���̴���Ž-���̴��м� PDW ����

#define OPCODE_TF_BD_SENDERROR		0x8202F100		// ���̴���Ž-���̴��м� ���� ����



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
		// ���۽� �ѹ��� ȣ���ϸ� �˴ϴ�.
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

		// ���α׷� ����� �ѹ��� ȣ���ϸ� �˴ϴ�.
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

void CRadarAnalysisMngr::ProcessMsg(STMsg& i_stMsg)
{
	// OPCODE���� ��츦 �����Ͽ� ó���ϴ� ���� ���� �ʿ�
	// �ϴܿ� ����
	bool bRtnSend = false;

	switch(i_stMsg.uiOpcode)
	{
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// ��� To ���̴��м�
	case OPCODE_OV_BD_REQINIT:		// ���-���̴��м� �ʱ�ȭ �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž �ʱ�ȭ �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQINIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž �ʱ�ȭ �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž �ʱ�ȭ �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQSWVER:		// ���-���̴��м� SW���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž SW���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSWVER, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž SW���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž SW���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQPBIT:		// ���-���̴��м� �ʱ���ü���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž �ʱ���ü���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQPBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž �ʱ���ü���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž �ʱ���ü���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQIBIT:		// ���-���̴��м� �������ü���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž �������ü���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQIBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž �������ü���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž �������ü���� �䱸\n");
			}
		}
		break;
	
	case OPCODE_OV_BD_REQCHCALIB:		// ���-���̴��м� ä�� ���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž ä�� ���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQCHCALIB, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ä�� ���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ä�� ���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQSTARTACQ:	// ���-���̴��м� �������� �䱸
		{
			TRACE("[����]���-���̴��м� �������� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSTARTACQ, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž �������� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž �������� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_SETRESET:		// ���-���̴��м� ��� ���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž ��� ���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETRESET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ��� ���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ��� ���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_CONTROLRECVR:		// ���-���̴��м� ���ű� ����
		{
			TRACE("[����]���̴��м�-���̴���Ž ���ű� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_CONTROLRECVR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ���ű� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ���ű� ����\n");
			}
		}
		break;
	
	case OPCODE_OV_BD_REQSYSTEMSET:		// ���-���̴��м� �ý��ۺ��� ���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž �ý��ۺ��� ���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQSYSTEMSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �������� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž �ý��ۺ��� ���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž �ý��ۺ��� ���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQCHANGEEMIT:	// ���-���̴��м� ���̴�/���� ���� �䱸
		{
			TRACE("[����]���̴��м�-���̴���Ž ���̴�/���� ���� �䱸\n");

			// ���̴��м�-���̴���Ž ���̴�/���� ���� �䱸 ���� ICD �޽����� ����
		}
		break;

	case OPCODE_OV_BD_REQACQTASK:	// ���-���̴��м� ���� ���� �䱸
		{
			TRACE("[����]���-���̴��м� ���� ���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQACQTASK, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž ���� ���� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ���� ���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ���� ���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_REQREJFREQSET:	// ���-���̴��м� ���Ź������ļ� ���� �䱸
		{
			TRACE("[����]���-���̴��м� ���Ź������ļ� ���� �䱸\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_REQREJFREQSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž ���� ���� �䱸

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ���Ź������ļ� ���� �䱸\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ���Ź������ļ� ���� �䱸\n");
			}
		}
		break;

	case OPCODE_OV_BD_SETSYSTEM:	// ���-���̴��м� �ý��� ����
		{
			TRACE("[����]���-���̴��м� �ý��� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SETSYSTEM, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �ý��� ����

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž �ý��� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž �ý��� ����\n");
			}
		}
		break;

	case OPCODE_OV_BD_SENDERROR:	// ���-���̴��м� ���� ����
		{
			TRACE("[����]���-���̴��м� ���� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_TF_SENDERROR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-���̴���Ž �ý��� ����

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-���̴���Ž ���� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-���̴���Ž ���� ����\n");
			}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// ���̴���Ž To ���̴��м�
	case OPCODE_TF_BD_RSTINIT:	// ���̴���Ž-���̴��м� �ʱ�ȭ ���
		{
			TRACE("[����]���̴���Ž-���̴��м� �������� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTINIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �ʱ�ȭ ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �ʱ�ȭ ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSWVER:	// ���̴���Ž-���̴��м� SW���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� SW���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSWVER, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� SW���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� SW���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTPBIT:	// ���̴���Ž-���̴��м� �ʱ���ü���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� �ʱ���ü���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTPBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �ʱ���ü���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �ʱ���ü���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTIBIT:	// ���̴���Ž-���̴��м� �������ü���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� �������ü���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTIBIT, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �������ü���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �������ü���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_STATECHCALIB:	// ���̴���Ž-���̴��м� ä�� ���� ���� ����
		{
			TRACE("[����]���̴���Ž-���̴��м� ä�� ���� ���� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_STATECHCALIB, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ä�� ���� ���� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ä�� ���� ���� ����\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSTARTACQ:	// ���̴���Ž-���̴��м� �������� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� �������� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSTARTACQ, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �������� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �������� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTRESETEQUIP:	// ���̴���Ž-���̴��м� ��� ���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� ��� ���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTRESETEQUIP, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ��� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ��� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTRECVCONTROL:	// ���̴���Ž-���̴��м� ���ű� ���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� ���ű� ���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTRECVCONTROL, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ���ű� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ���ű� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTSYSTEMSET:	// ���̴���Ž-���̴��м� ���̴���Ž �ý��ۺ��� ���� ���
		{
			TRACE("[����]���̴���Ž-���̴��м� ���̴���Ž �ý��ۺ��� ���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTSYSTEMSET, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ���̴���Ž �ý��ۺ��� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ���̴���Ž �ý��ۺ��� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_STATETASK:	// ���̴���Ž-���̴��м� ���� ���� ����
		{
			TRACE("[����]���̴���Ž-���̴��м� ���� ���� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_STATETASK, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ���� ���� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ���� ���� ����\n");
			}
		}
		break;

	case OPCODE_TF_BD_RSTACQTASK:
		{
			TRACE("[����]���̴���Ž-���̴��м� �������� ���� ���\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_RSTACQTASK, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� �������� ���� ���\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� �������� ���� ���\n");
			}
		}
		break;

	case OPCODE_TF_BD_SENDLOB:	// ���̴���Ž-���̴��м� LOB ����
		{
			TRACE("[����]���̴���Ž-���̴��м� LOB ����\n");

			//LOB ������ ����
			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDLOB, i_stMsg.usMSize, i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, i_stMsg.buf);	// ���̴��м�-��� �������� ���
			
			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� LOB ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� LOB ����\n");
			}

			// �˰��� ���� �� BEAM �����Ͽ� BEAM ���� �����ϴ� ��ƾ �߰�
			///////////////////////////////////////////////////////////////////////////////
			//��ö�� �������� LOB����Ÿ���� �� ���� �˰��� ȣ��			

			STR_LOBDATA stLOBData;
			memcpy(& stLOBData, i_stMsg.buf, i_stMsg.usMSize);
			STR_ABTDATA *pABTData, txABTData;
			RadarAnlAlgotirhm::RadarAnlAlgotirhm::Start(& stLOBData);
			RadarAnlAlgotirhm::RadarAnlAlgotirhm::UpdateCEDEOBLibrary();
			RadarAnlAlgotirhm::RadarAnlAlgotirhm::GetABTData(pABTData);

			memcpy(&txABTData, pABTData, sizeof(txABTData));
			///////////////////////////////////////////////////////////////////////////////
			
			//�� ������ ����
			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDBEAM, sizeof(txABTData), i_stMsg.ucLinkID, Equip_Rev_0V, m_OVIP, pABTData);	
		}
		break;

	case OPCODE_TF_BD_SENDPDW:	// ���̴���Ž-���̴��м� PDW ����
		{
			TRACE("[����]���̴���Ž-���̴��м� PDW ����\n");

			// ���̴��м�-��� PDW ������ ICD �޽��� ���� �ȵǾ� ����
		}
		break;

	case OPCODE_TF_BD_SENDERROR:	// ���̴���Ž-���̴��м� ���� ����
		{
			TRACE("[����]���̴���Ž-���̴��м� ���� ����\n");

			bRtnSend = m_hCommIF.Send(OPCODE_BD_OV_SENDERROR, i_stMsg.usMSize, i_stMsg.ucLinkID, i_stMsg.ucRevOperID, i_stMsg.ucOperatorID, i_stMsg.buf);	// ���̴��м�-��� �������� ���

			if(bRtnSend == true)
			{
				TRACE("[�۽�]���̴��м�-��� ���� ����\n");
			}
			else
			{
				TRACE("[�۽� ����]���̴��м�-��� ���� ����\n");
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

	//////// �ڽ��� IP ȹ�� //////////////////////////////////////////////////////////////////
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