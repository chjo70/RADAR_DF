// PingThread.cpp: implementation of the CPingThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PingThread.h"
#include <process.h>    /* _beginthread, _endthread */

#include "..\COMMIF\CGRCommIF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define HEART_BEAT_PING_TRY_CNT	(3)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
* 생성자
*/
CPingThread::CPingThread()
{
	m_nUnitID = NULL;
	m_iBITKind = NULL;
	m_nRetries = 1;	
	m_hWnd = NULL;

	m_strHeartbeatIP_1 = "0.0.0.0";
	m_strHeartbeatIP_2 = "0.0.0.0";

	m_strHeartbeatIP_3 = "0.0.0.0";
	m_strHeartbeatIP_4 = "0.0.0.0";

	m_strLinkIP = "0.0.0.0";
	m_strServerIP = "0.0.0.0";

	m_dwID = 0; 

	m_hThread = NULL;
	m_hKillEvent	= CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hSignalEvent  = CreateEvent(NULL,FALSE,FALSE,NULL);
	
	m_hThread = (HANDLE) _beginthreadex(
		NULL,					// Security
		0,						// Stack size - use default
		ThreadProc,     		// Thread fn entry point
		(void*) this,	     
		0,						// Init flag
		&m_dwID);				// Thread address
}

/**
* 소멸자
*/
CPingThread::~CPingThread()
{
	SetEvent(m_hKillEvent);
	WaitForSingleObject(m_hThread,INFINITE);
}

/**
* PING 스레드(WMF)
* @return 실행결과
*/
UINT CPingThread::ThreadProc(void* lpParam)
{
	CPingThread* pThis = reinterpret_cast<CPingThread*>(lpParam);

	bool bRst = false;
	int iUnitID;
	string strIP;

	while (1)
	{
		HANDLE hObjects[2];
		hObjects[0] = pThis->m_hKillEvent;
		hObjects[1] = pThis->m_hSignalEvent;

		DWORD dwWait = WaitForMultipleObjects(2,hObjects,FALSE,INFINITE);
		if (dwWait == WAIT_OBJECT_0)		
			break;

		if (dwWait == WAIT_OBJECT_0 + 1)
		{
			iUnitID = pThis->m_nUnitID;

			if ( iUnitID == PING_ID_HEARTBEAT_SVR )
			{
				if ( pThis->m_strHeartbeatIP_1 != "0.0.0.0" )
				{
					bRst = pThis->m_ping.Ping(HEART_BEAT_PING_TRY_CNT, pThis->m_strHeartbeatIP_1, pThis->m_hWnd, pThis->m_iBITKind);
					GP_COMM_IF.ReturnPingRst(CLIENT_NO_1, !bRst, true);
				}				
			}
			else if (iUnitID == PING_ID_HEARTBEAT_CLT)
			{
				if ( pThis->m_strHeartbeatIP_1 != "0.0.0.0" )
				{
					bRst = pThis->m_ping.Ping(HEART_BEAT_PING_TRY_CNT, pThis->m_strHeartbeatIP_1, pThis->m_hWnd, pThis->m_iBITKind);
					GP_COMM_IF.ReturnPingRst(CLIENT_NO_1, !bRst);
				}

				if ( pThis->m_strHeartbeatIP_2 != "0.0.0.0" )
				{
					bRst = pThis->m_ping.Ping(HEART_BEAT_PING_TRY_CNT, pThis->m_strHeartbeatIP_2, pThis->m_hWnd, pThis->m_iBITKind);
					GP_COMM_IF.ReturnPingRst(CLIENT_NO_2, !bRst);
				}

				if ( pThis->m_strHeartbeatIP_3 != "0.0.0.0" )
				{
					bRst = pThis->m_ping.Ping(HEART_BEAT_PING_TRY_CNT, pThis->m_strHeartbeatIP_3, pThis->m_hWnd, pThis->m_iBITKind);
					GP_COMM_IF.ReturnPingRst(CLIENT_NO_3, !bRst);
				}

				if ( pThis->m_strHeartbeatIP_4 != "0.0.0.0" )
				{
					bRst = pThis->m_ping.Ping(HEART_BEAT_PING_TRY_CNT, pThis->m_strHeartbeatIP_4, pThis->m_hWnd, pThis->m_iBITKind);
					GP_COMM_IF.ReturnPingRst(CLIENT_NO_4, !bRst);
				}
			}
		}
	}	
	
	return 0;
}

/**
* PING 시작
* @param nRetries 재시도 회수
* @param strHost 호스트 IP
* @param hWnd 쓰레드 메시지 핸들
* @param iBITKind 자체점검 종류
*/
void CPingThread::StartPing(int i_nBitType, int i_nUnitID)
{
	m_iBITKind = i_nBitType;
	m_nUnitID = i_nUnitID;

	SetEvent(m_hSignalEvent);
}

void CPingThread::SetLinkIP(CString i_strLinkIP)
{
	m_strLinkIP = i_strLinkIP;
}
void CPingThread::SetServerIP(CString i_strServerIP)
{
	m_strServerIP = i_strServerIP;
}

void CPingThread::SetHeartbeatIP_1(CString i_strIP)
{
	m_strHeartbeatIP_1 = i_strIP;
}

void CPingThread::SetHeartbeatIP_2(CString i_strIP)
{
	m_strHeartbeatIP_2 = i_strIP;
}

void CPingThread::SetHeartbeatIP_3(CString i_strIP)
{
	m_strHeartbeatIP_3 = i_strIP;
}

void CPingThread::SetHeartbeatIP_4(CString i_strIP)
{
	m_strHeartbeatIP_4 = i_strIP;
}