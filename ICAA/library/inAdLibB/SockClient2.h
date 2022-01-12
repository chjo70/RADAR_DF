// SockClient2.h: interface for the CSockClient2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SockClient2_H__DE913A24_8819_43D6_8B81_E8803AB5A9E3__INCLUDED_)
#define AFX_SockClient2_H__DE913A24_8819_43D6_8B81_E8803AB5A9E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SockCommon.h"

#ifdef _DEBUG //----------s
	typedef struct _SCKRECV_INFO
	{
		int nMaxPacket;
		int nMaxOneRecv;
	}SCKRECV_INFO;

	_SocketFunc_API extern SCKRECV_INFO   g_TcpSckRecvInfo;
	_SocketFunc_API extern SCKRECV_INFO   g_UdpSckRecvInfo;
#endif // _DEBUG //----------s


class _SocketFunc_API CSockClient2 : public CSockClient  
{
public:
	CSockClient2();
	virtual ~CSockClient2();


	void InitReceiveData();
	BOOL CheckSckPacket(int iLength, int emTag);
	int ReceiveSocketData(BOOL bTCP, char* sSocketName);
	int ReceiveSocketDataDF(BOOL bTCP, char* sSocketName);
	int ReceiveSocketData2(BOOL bTCP, char* sSocketName);

	char* GetDataPtr(){
		return m_sRecvQueue + PACKET_HEADER_SIZE;}
	void SetbCONV_BYTE_ORDER(BOOL b){
		m_bCONV_BYTE_ORDER = b;
	}

	BOOL m_bRecvHeader;
	char m_szRecvData[MAX_SOCKET_RECV_SIZE];
	int	 m_iPacketHeader[2];
	BOOL m_bCONV_BYTE_ORDER;

	BOOL m_bUseOPProtocol;

private:
	int		m_iReadSize;
};

#endif // !defined(AFX_SockClient2_H__DE913A24_8819_43D6_8B81_E8803AB5A9E3__INCLUDED_)
