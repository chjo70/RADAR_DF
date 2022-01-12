#include <stdAfx.h>
#include "MsgQueueThread.h"

CMsgQueueThread::CMsgQueueThread(void)
{	
	if(m_cond.CreateLIGEvent(true) == FALSE ) // unsignal 인 상태. manual reset 상태로 이벤트를 만든다.
	{
		return;
	}
	this->Start();	
}

CMsgQueueThread::~CMsgQueueThread(void)
{	
	this->Stop();

	DeleteAllQData();

	// 타이머 스레드, Q스레드 등이 모두 멈출 시간을 벌어줘야 한다. 그렇지 않으면 스레드 안에서 사용하는 Critical Section 객체들이 이미 delete
	// 되었음에도 ScopedLock에서 critical section 객체를 통해 lock을 걸기 때문에 프로그램 종료 시 죽는 문제 발생. 2015.05.26. 이정남.
	Sleep(2000); 	
}

void CMsgQueueThread::Run()
{
	bool bRslt = true;
	STMsg stMsg;	

	while( IsRunning() == TRUE)
	{
		m_cond.Wait(); // 시그널을 기다리는 중이다. 

		bRslt = GetMsgFromQ(stMsg);
		if(bRslt)
		{
			ProcessMsg(stMsg);			
		}
		else // 큐에 더 이상 메시지가 없다는 이야기다.
		{
			m_cond.LIGResetEvent();
		}				
	}
}

bool CMsgQueueThread::GetMsgFromQ(STMsg& i_stMsg)
{
	bool bRtn = false;

	ScopedLock lock(&m_QLock);
	if(m_queMsg.size() != 0)
	{		
		i_stMsg = m_queMsg.front();
		m_queMsg.pop();
		bRtn = true;
	}
	
	return bRtn;
}

void CMsgQueueThread::PushMsg(STMsg& i_stMsg)
{
	ScopedLock lock(&m_QLock);
	m_queMsg.push(i_stMsg);
}

void CMsgQueueThread::DeleteAllQData()
{
	ScopedLock lock(&m_QLock);
	int nQsize = m_queMsg.size();
	if(nQsize != 0)
	{		
		for(int i=0; i< nQsize; ++i)
		{
			m_queMsg.pop();
		}		
	}
}

void CMsgQueueThread::ProcessMsg(STMsg& i_stMsg)
{
	// 상속받은 클래스에서 재정의 필요
}