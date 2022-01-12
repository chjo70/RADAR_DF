#if !defined(AFX_SOCKCLIENT_H__32F68844_012F_4421_8958_7135BE53BDCB__INCLUDED_)
#define AFX_SOCKCLIENT_H__32F68844_012F_4421_8958_7135BE53BDCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SockClient.h : header file
//

#include "SockCommon.h"

/////////////////////////////////////////////////////////////////////////////
// CSockClient command target
class CRcvFuncTCP;

class _SocketFunc_API CSockClient : public CAsyncSocket
{
// Attributes
public:
	HWND				m_hWnd;						// 메세지를 받을 핸들
	BOOL				m_bCreated;					// 소켓 생성 상태 플래그 (TRUE:생성 FALSE:삭제)
	BOOL				m_bConnected;				// 소켓 연결 상태 플래그 (TRUE:연결 FALSE:절단)
	CSockCommon			m_pSocketCommon;			// 소켓 공통 함수 클래스
	struct sockaddr_in	m_StSockAddrRemote;			// Remote 소켓의 네크워크 주소 구조체 (internet style)
	struct sockaddr_in	m_StSockAddrLocal;			// Local 소켓의 네크워크 주소 구조체 (internet style)
	unsigned int		m_uiEventReceive;			// 소켓 전문 수신 이벤트 ID
	unsigned int		m_uiEventError;				// 소켓 오류 이벤트 ID
	unsigned int		m_uiEventClose;
	int					m_iSocketID;				// 소켓 구분 ID
	BOOL				m_bRecvEventPoll;			// 소켓 정보 읽기 처리 방식 (TRUE:이벤트 FALSE:폴링)
													// TRUE  : 소켓의 수신 이벤트로 정보를 읽는다
													// FALSE : 일정 간격의 시간마다 정보를 읽는다

	unsigned int		m_uiRecvQueueSize;			// 미완성 전문의 임시 보관 큐의 크기
//	char				m_sRecvQueue[SOCKET_QUEUE];	// 미완성 전문의 임시 보관 큐
    char				m_sRecvQueue[SIZE_512K];	// 미완성 전문의 임시 보관 큐
	char				m_sErrorString[BUFFMAX];	// 오류 메세지

	CRcvFuncTCP			*m_CRcvFuncTCP;
	void SetThis(LPVOID lParam);
	BOOL m_bSetThis;
	char m_byteReceive[BUFFMAX];   //8-bit data item.

// Operations
public:
			CSockClient();

	virtual ~CSockClient();

			// 소켓으로부터오는 메세지를 받을 핸들을 설정
	void	SetMessageHandle(HWND hWnd);			

			// 소켓의 이벤트 정보등을 설정
	void	SetConfig(	UINT uiEventReceive, 
						UINT uiEventError, 
						UINT uiEventClose, 
						BOOL bFlag);					

			// 소켓의 ID를 사용하여 이벤트 정보등을 설정
	void	SetConfig(	UINT uiEventReceive, 
						UINT uiEventError, 
						UINT uiEventClose, 
						int iSocketID, 
						BOOL bFlag);	
			// 소켓을 TCP (Stream Socket)방식으로 생성 (Host,Services 파일 사용)
	BOOL	CreateSocket(char* sIPAddressRemote, char* sPortNo);

			// 소켓을 TCP (Stream Socket)방식으로 생성 (Host,Services 파일 미사용)
	BOOL	CreateSocket();	

			// 소켓을 UDP (Datagram Socket)방식으로 생성 (Host,Services 파일 사용)
	BOOL	CreateSocket(char* sIPAddressRemote, char* sIPAddressLocal, char* sPortNo);
			
			// 소켓을 UDP (Datagram Socket)방식으로 생성 (Host,Services 파일 미사용)
	BOOL	CreateSocket(char* sIPAddressLocal, int iPortNo);

	void	CloseSocket();																						


// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockClient)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSockClient)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKCLIENT_H__32F68844_012F_4421_8958_7135BE53BDCB__INCLUDED_)
