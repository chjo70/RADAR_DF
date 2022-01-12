// PingThread.h: interface for the CPingThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_)
#define AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ping.h"

/** 
* SDD 식별자 : D-CM-SFD-ACIF
*
* (1) 기능
*  - 보안장비 점검을 위해 사용
*  - 안정적인 통신상태 확인(CPing)을 위한 쓰레드 구동 기능 담당
*
* (2) 설계결정사항
*  - 해당 사항 없음
*/
class CPingThread  
{
public:
	CPingThread();
	virtual ~CPingThread();

	/**
	* 쓰레드 ID
	*/
	UINT	m_dwID;
	/**
	* 쓰레드핸들
	*/
	HANDLE	m_hThread;
	/**
	* 쓰레드 종료를 위한 이벤트 핸들
	*/
	HANDLE	m_hKillEvent;
	/**
	* 쓰레드 이벤트 처리를 위한 신호발생 핸들
	*/
	HANDLE	m_hSignalEvent;

	static UINT __stdcall  ThreadProc(void* pThis);
	void StartPing(int i_nBitType, int i_nUnitID);

	/**
	* 통신연결상태 확인 가능을 수행하는 클래스 인스턴스
	*/
	CPing	m_ping;
	/**
	* 호스트 IP
	*/
	CString m_strHost;

	CString m_strLinkIP;
	CString m_strServerIP;

	CString m_strHeartbeatIP_1;
	CString m_strHeartbeatIP_2;

	CString m_strHeartbeatIP_3;
	CString m_strHeartbeatIP_4;

	void SetHeartbeatIP_1(CString i_strIP);
	void SetHeartbeatIP_2(CString i_strIP);

	void SetHeartbeatIP_3(CString i_strIP);
	void SetHeartbeatIP_4(CString i_strIP);

	void SetLinkIP(CString i_strLinkIP);
	void SetServerIP(CString i_strServerIP);

	/**
	* 쓰레드 메시지 핸들
	*/
	HWND	m_hWnd;
	/**
	* 재시도 회수
	*/
	UINT	m_nRetries;
	
	/**
	* 자체점검 종류
	*/
	int		m_iBITKind;
		
	int		m_nUnitID;
};

#endif // !defined(AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_)
