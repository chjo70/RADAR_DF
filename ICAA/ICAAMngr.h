#pragma once

#include "MsgQueueThread.h"
#include "CommIF\CommonMngr.h"
#include "CommIF\CGRCommIF.h"

struct STMsg;

class CICAAMngr : public CMsgQueueThread, public CCommonMngr
{
public:
	CICAAMngr();
	~CICAAMngr(void);

	static CICAAMngr* GetInstance();

	void Receive(unsigned int i_uiOpcode, unsigned short i_usSize, unsigned char i_ucLink, unsigned char i_ucRevOperID, unsigned char ucOpertorID, void *i_pvData);
	void ProcessMsg(STMsg& i_stMsg);

	CGRCommIF& m_hCommIF_ICAAMngr;
	static CICAAMngr* uniqueInstance;
};