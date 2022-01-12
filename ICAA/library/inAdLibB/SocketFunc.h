#if !defined(AFX_SOCKETFUNC_H__CE082EB7_D55B_410A_B184_9ED1ECEE1480__INCLUDED_)
#define AFX_SOCKETFUNC_H__CE082EB7_D55B_410A_B184_9ED1ECEE1480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketFunc.h : header file
//

#include "../library/inAdLibB\SockServer.h"

//////////////////////////////////////////////////////////////////////////

#include "../library/inAdLibB\def_socket.h"

//////////////////////////////////////////////////////////////////////////

#define	SOCKET_POLL_EVENT		TRUE				// 소켓 정보 읽기 처리 방식 (TRUE:시스템이벤트 FALSE:폴링)
#define	CONV_BYTE_ORDER			TRUE				// 네트워크 바이트 배열의 변경 여부 (TRUE:적용 FALSE:미적용)

#define	SOCKET_SEND_MAX			SIZE_40K			// 한번에 송신 가능한 전문의 크기
#define	SOCKET_SEND_BUFF		40976//SIZE_40K + SIZE_16	// 전문 송신용 소켓 버퍼 (+ Header Size 포함, 현재는 최대 16바이트까지 헤더정보를 사용)
#define	SOCKET_RECV_BUFF		SIZE_64K			// 전문 수신용 소켓 버퍼

/////////////////////////////////////////////////////////////////////////////
// CSocketFunc command target

class _SocketFunc_API CSocketFunc : public CObject
{
// Attributes
public:
			CSocketFunc();
			
	virtual ~CSocketFunc();

// Operations
public:


	//-----------------------------------------------------------------------------
	// TCP Socket Server Create : using the service file
	//-----------------------------------------------------------------------------
	BOOL	CreateSocketServer(HWND hWnd, CSockServer* pSockServer, char* szSockPort, UINT uWmSocketAccept, int nSocketID, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// TCP Socket Server Create : not using the service file
	//-----------------------------------------------------------------------------
	BOOL	CreateSocketServer(HWND hWnd, CSockServer * pSockServer, int nSockPort, UINT uWmSocketAccept, int nSocketID, CString * pstrMsg);

	//-----------------------------------------------------------------------------
	// UDP Socket Server Create : not using the service file
	//-----------------------------------------------------------------------------
	BOOL	CreateUdpSocketServer(HWND hWnd, CSockClient* pClientSocket, UINT uSocketEvent, char* szRemoteIP, char* szLocalIP, int nSockPort, int nSocketID, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// 통신테그 유효성 검사
	//-----------------------------------------------------------------------------
	BOOL	CheckDamagePacket(int iLength, int emTag);

	//-----------------------------------------------------------------------------
	// 소켓이 '연결'상태이면 강제 연결 종료
	//-----------------------------------------------------------------------------
	void	CloseSocketClient(CSockClient* pClientSocket);

	//-----------------------------------------------------------------------------
	// 소켓이 '연결'상태이면 강제 연결 종료 / 소켓 삭제	- TCP 소켓
	//-----------------------------------------------------------------------------
	void	CloseTcpSocketClient(CSockClient** ppClientSocket);

	//-----------------------------------------------------------------------------
	// 소켓이 '연결'상태이면 강제 연결 종료 / 소켓 삭제	- UDP 소켓
	//-----------------------------------------------------------------------------
	void	CloseUdpSocketClient(CSockClient** ppClientSocket);

	//-----------------------------------------------------------------------------
	// TCP 소켓 서버에의 연결 : service 파일 사용
	//-----------------------------------------------------------------------------
	BOOL	ConnectSocketServer(HWND hWnd, CSockClient** ppClientSocket, UINT uSocketEvent, char* szSocketIP, char* szSockPort, int nSocketID);

	//-----------------------------------------------------------------------------
	// 소켓 서버에의 연결 : service 파일 미사용
	//-----------------------------------------------------------------------------
	BOOL	ConnectSocketServer(HWND hWnd, CSockClient** ppClientSocket, UINT uSocketEvent, char* szSocketIP, int nSockPort, int nSocketID);

	//-----------------------------------------------------------------------------
	// 소켓 클라이언트 Accept 
	//-----------------------------------------------------------------------------
	void	AcceptSocketClient(HWND hWnd, CSockServer* pServerSocket, CSockClient* pClientSocket, UINT uSocketEvent, int nSocketID);

	//-----------------------------------------------------------------------------
	// 송신할 전문을 작성 (1)
	//-----------------------------------------------------------------------------
	BOOL	MakeupSendSocketData(CSockClient* pClientSocket, int emTag, void* szData, int iLength, BOOL bConvByteOrder, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// TCP 소켓으로 메세지를 송신
	//-----------------------------------------------------------------------------
	BOOL	SendSocketPacket(CSockClient* pClientSocket, void* sMessage, int iLength, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// (UDP) 송신할 전문을 작성 (1)
	//-----------------------------------------------------------------------------
	BOOL	MakeupUdpSendSocketData(CSockClient* pClientSocket, int emTag, void* szData, int iLength, CString strRemoteIpAddress, UINT uPort, BOOL bConvByteOrder, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// UDP 소켓으로 메세지를 송신
	//-----------------------------------------------------------------------------
	BOOL	SendUdpSocketPacket(CSockClient* pClientSocket, void* sMessage, int iLength, CString strRemoteIpAddress, UINT uPort, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// 새로운 데이타가 들어왔을때 TCP 소켓에서 데이타를 읽는다
	//-----------------------------------------------------------------------------
	LONG	ReceiveTcpSocketPacket(CSockClient* pClientSocket, char* szBuff, int* nReadByte, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// 새로운 데이타가 들어왔을때 UDP 소켓에서 데이타를 읽는다
	//-----------------------------------------------------------------------------
	LONG	ReceiveUdpSocketPacket(CSockClient* pClientSocket, char* szBuff, int* nReadByte, CString* pstrMsg);

	//-----------------------------------------------------------------------------
	// 수신된 전문을 분석
	//-----------------------------------------------------------------------------
	BOOL	DevideReceiveSocketData(CSockClient* pClientSocket, int iReadSize, char* szData, int* emTag, BOOL bConvByteOrder);

	//-----------------------------------------------------------------------------
	// 해당채널에 해당되는 OP 소켓 IP Address 가져오기
	//-----------------------------------------------------------------------------
	CString GetPeerIPAddress(CSockClient* pClientSocket);

	//-----------------------------------------------------------------------------
	// 수신 통신테그에 해당되는 테그문자열 가져오기
	//-----------------------------------------------------------------------------
	CString GetPacketTagString(int emTag);

	SOCKADDR_IN		m_sockAddrRemote;

// Overrides

// Implementation
protected:


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETFUNC_H__CE082EB7_D55B_410A_B184_9ED1ECEE1480__INCLUDED_)
