#if !defined(AFX_SOCKCOMMON_H__B114E321_E906_4CFB_B671_2E4A2616C933__INCLUDED_)
#define AFX_SOCKCOMMON_H__B114E321_E906_4CFB_B671_2E4A2616C933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SockCommon.h : header file
//

#ifdef _SocketFunc_DLL_
#define _SocketFunc_API  _declspec(dllexport)
#else
#define _SocketFunc_API  _declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
#define SPEC_DP_SZ					1601

#define	SIZE_16					16			// 헤더 정보 사이즈
#define SIZE_64					64
#define SIZE_256				256			
#define SIZE_1K					1024
#define SIZE_2K					2048
#define SIZE_4K					4096
#define SIZE_10K				10240
#define SIZE_20K				20480
#define SIZE_40K				40960
#define SIZE_64K				65536
#define SIZE_512K				524288

//////////////////////////////////////////////////////////////////////////

#define	BUFFMIN					64//SIZE_64		// 버퍼 크기
#define	BUFFMAX					4096//SIZE_4K		// 임시 버퍼 크기		// SIZE_256

#define	NORMAL_ERROR			0			// 일반적 오류
#define	FATAL_ERROR				1			// 치명적 오류

#define	SOCKET_QUEUE			10240 //DEL SIZE 524288		// 전문 수신용 버퍼 ((1024 * 512) + 1024) * 2

// 지연 시간 관련
#define	SOCKET_CONNECT_DELAY	200		// 소켓 연결시의 지연 시간 (단위 msec) 1000
#define	SOCKET_DISCONNECT_DELAY	400		// 소켓 절단시의 지연 시간 (단위 msec) 2000
#define	SOCKET_SEND_DELAY		3			// 소켓으로 전문 송신시의 지연 시간 (단위 msec)

// 타이머 관련
#define	SOCKET_POLL_TIMER		100			// 소켓의 버퍼로부터 정보를 읽는 타이머의 ID
#define	SOCKET_POLL_INTEVAL		300			// 소켓의 버퍼로부터 정보를 읽는 시간 간격 (단위 msec)

//////////////////////////////////////////////////////////////////////////
// [ykhong 11/18/2009] 
#define SCKRECV_RECVERR					-1
#define SCKRECV_PACKET_BROKEN			-2

#define SCKRECV_PACKET_COMPLETED		1
#define SCKRECV_PACKET_CONTINUE			2

#define  MAX_SOCKET_RECV_SIZE			16384
#define  MAX_SOCKET_PACKET_SIZE			16384


// 통신 전문 헤더 정보 관련
typedef enum {
	ePacketHeader_1		= 0,
	ePacketHeader_2,						// 통신 전문 헤더 2 : 종류
	ePacketHeader_3,						// 통신 전문 헤더 3 : 주의! 고정방탐연계에서 사용[RMS ID] [2006.12.13 SWR. 안찬길 추가!]
	ePacketHeader_4,						// 통신 전문 헤더 4 : 주의! 고정방탐연계에서 사용[FDF ID] [2006.12.13 SWR. 안찬길 추가!]
	
	eCountPacketHeader,
} enumPacketHeader;


/////////////////////////////////////////////////////////////////////////////
// CSockCommon command target

class _SocketFunc_API CSockCommon : public CObject
{
// Attributes
public:

// Operations
public:
			CSockCommon();
	virtual ~CSockCommon();

	char	m_szLocalName[BUFFMIN];										// 시스템 PC명
	char	m_szLocalIP[BUFFMIN];										// 시스템 IP 정보

	BOOL	GetCurrentNetworkInformation();								// 현 작업 시스템의 네트워크 정보를 설정
	void	GetIPAddressFromName(char* szHostName, char* szIPAddrss);	// 호스트명으로 해당 네크워크 주소를 찾아서 설정

	unsigned short	HostToPcShort(short data);									// 서버에서 수신한 값을 PC의 short정수형으로 변환
	int		HostToPcInt(int data);										// 서버에서 수신한 값을 PC의 int정수형으로 변환
	float	HostToPcFloat(float data);									// 서버에서 수신한 값을 PC의 부동소수점형으로 변환
	double	HostToPcDouble(double data);								// 서버에서 수신한 값을 PC의 배정도 부동소수점형으로 변환

	short	PcToHostShort(short data);									// 서버로 송신할 값을 서버의 short정수형으로 변환
	int		PcToHostInt(int data);										// 서버로 송신할 값을 서버의 int정수형으로 변환
	float	PcToHostFloat(float data);									// 서버로 송신할 값을 서버의 부동소수점형으로 변환
	double	PcToHostDouble(double data);								// 서버로 송신할 값을 서버의 배정도 부동소수점형으로 변환

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockCommon)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSockCommon)
	//}}AFX_MSG

// Implementation

public:
	// [ykhong 11/18/2009] 
	void SetMinMaxTag(int min, int max);
	BOOL CheckSckPacket(int iLength, int emTag);

	static int GetServbyname(const char* sPortNo);
protected:

	int m_iMinTag, m_iMaxTag;
};

_SocketFunc_API extern CSockCommon g_SockCommon;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKCOMMON_H__B114E321_E906_4CFB_B671_2E4A2616C933__INCLUDED_)
