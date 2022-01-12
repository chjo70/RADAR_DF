#pragma once

#include "..\MsgQueueThread.h"
#include "..\CommIF\CommonMngr.h"
#include "..\CommIF\CGRCommIF.h"

#include "..\ThreadTask/DFMsgDefnt.h"
#include "DFTaskMngr.h"
#include <iostream>
#include <fstream>
#include "..\_lib_AnD\RcvFuncTCP.h"

struct STMsg;

class CDFEquipBITMngr : public CMsgQueueThread, public CCommonMngr
{
public:
	CDFEquipBITMngr();
	~CDFEquipBITMngr(void);

	static CDFEquipBITMngr* GetInstance();	

	void Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData);
	void ProcessMsg(STMsg& i_stMsg);
	
	static CDFEquipBITMngr* m_pInstance;
	CGRCommIF& m_hCommIF_DFEquipBITMngr;
	CDFTaskMngr *pDFMngr;
	void SetDFPoint(void *pPoint);

	CRcvFuncTCP g_RcvFunc;
};

