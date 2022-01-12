#pragma once

#include "MsgQueueThread.h"
#include "CommIF\CommonMngr.h"
#include "CommIF\CGRCommIF.h"
#include "RadarAnlAlgorithm.h"

struct STMsg;

class CRadarAnalysisMngr : public CMsgQueueThread, public CCommonMngr
{
public:
	CRadarAnalysisMngr();
	~CRadarAnalysisMngr(void);

	static CRadarAnalysisMngr* GetInstance();

	void Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData);
	void ProcessMsg(STMsg& i_stMsg);

	CGRCommIF& m_hCommIF_RAMngr;
	static CRadarAnalysisMngr* uniqueInstance;

	int m_OVIP;
	unsigned char GetLastIP();
};