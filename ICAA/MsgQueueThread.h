#pragma once

#include "ThreadLIB\LIGThread.h"
#include <queue>

using namespace std;

struct STMsg
{
	unsigned int uiOpcode;
	unsigned short usMSize;
	unsigned char	ucLinkID;
	unsigned char	ucRevOperID;
	unsigned char	ucOperatorID;
	unsigned char buf[65535];

	STMsg::STMsg()
		:uiOpcode(0), usMSize(0),  ucLinkID(0), ucRevOperID(0), ucOperatorID(0)
	{
		memset(buf, 0, 65535);
	}
};

class CMsgQueueThread : public LIGThread
{
public:
	CMsgQueueThread(void);
	virtual ~CMsgQueueThread(void);

	virtual void ProcessMsg(STMsg& i_stMsg);

protected:
	virtual void Run();
	bool GetMsgFromQ(STMsg& i_stMsg);
	void PushMsg(STMsg& i_stMsg);
	void DeleteAllQData();

	inline unsigned int GetMsgQSize() { return m_queMsg.size(); }

protected:
	std::queue<STMsg> m_queMsg;	
	LIGCondition m_cond;
	LIGLock m_QLock;		
};