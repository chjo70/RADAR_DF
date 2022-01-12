#if !defined(AFX_SOCKSERVER_H__61D0F509_6873_4635_B109_6861A321464E__INCLUDED_)
#define AFX_SOCKSERVER_H__61D0F509_6873_4635_B109_6861A321464E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SockServer.h : header file
//

#include "SockCommon.h"
#include "SockClient.h"

/////////////////////////////////////////////////////////////////////////////
// CSockServer command target

class _SocketFunc_API CSockServer : public CAsyncSocket
{
// Attributes
public:
	HWND				m_hWnd;							// 메세지를 받을 핸들
	BOOL				m_bCreated;						// 소켓 생성 상태 플래그 (TRUE:생성 FALSE:삭제)
	BOOL				m_bConnected;					// 소켓 연결 상태 플래그 (TRUE:연결 FALSE:절단)
	CSockCommon			m_pSocketCommon;				// 소켓 공통 함수 클래스
	struct sockaddr_in	m_StSockAddrLocal;				// Local 소켓의 네크워크 주소 구조체 (internet style)
	unsigned int		m_uiEventAccept;				// 소켓 ACCEPT 이벤트 ID
	int					m_iSocketID;					// 소켓 구분 ID
	unsigned int		m_uiConnectClinetPort;			// 서버에 접속된 클라이언트 소켓의 포트 정보 (확인용!)
	CString				m_sConnectClientIPAddress;		// 서버에 접속된 클라이언트 소켓의 네크워크 주소 (확인용!)
	char				m_sErrorString[BUFFMAX];		// 오류 메세지

// Operations
public:
			CSockServer();
	virtual ~CSockServer();

			// 소켓으로부터오는 메세지를 받을 핸들을 설정
	void	SetMessageHandle(HWND hWnd);			

			// 소켓의 이벤트 정보등을 설정
	void	SetConfig(unsigned int uiEventAccept, int iSocketID);	
			
			// 소켓을 TCP (Stream Socket)방식으로 생성 (Host,Services 파일 사용)
	BOOL	CreateSocket(char* sPortNo);			

			// 소켓을 TCP (Stream Socket)방식으로 생성 (Host,Services 파일 미사용)
	BOOL	CreateSocket(int iPortNo);					

	void	CloseSocket();

			// OnAccept 함수실행후 새로운 클라이언트와 연결된 클라이언트 소켓에 넘겨준다
	void	GetAcceptSocket(CSockClient* pCSockClient);	

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockServer)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSockServer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKSERVER_H__61D0F509_6873_4635_B109_6861A321464E__INCLUDED_)
