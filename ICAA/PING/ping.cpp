//
// THIS CODE IS BASED ON THE CODE FROM 
// THE BOOK WINSOCK 2.0 BY LEWIS NAPPER...
// 
//

#include "stdafx.h"
#include "ping.h"
#include <Icmpapi.h>
/**
* 보안 장비 자체점검
* @param nRetries 재시도 횟수
* @param pstrHost 대상 IP
* @param hWnd 결과를 전달할 윈도우 핸들
* @param iBITKind BIT 종료. SMBIT 또는 UMBIT
*/
bool CPing::Ping(UINT nRetries,LPCSTR pstrHost,HWND hWnd, int iBITKind)
{
	SOCKET	  rawSocket;
	LPHOSTENT lpHost;
	UINT	  nLoop;
	int       nRet;
	struct    sockaddr_in saDest;
	struct    sockaddr_in saSrc;
	DWORD	  dwTimeSent;
	u_char    cTTL;

	bool		bRst = true;
	
	m_hWnd = hWnd;

	CString str;

	// Create a Raw socket
	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	//TRACE("rawSocket = %d, iKind = %d\r\n", rawSocket, iBITKind);
	if (rawSocket == SOCKET_ERROR) 
	{
		WSAError("socket()");
		return bRst;
	}
	
	// Lookup host
	lpHost = gethostbyname(pstrHost);
	if (lpHost == NULL)
	{
		str.Format("Host not found: %s", pstrHost);
		return bRst;
	}
	
	// Setup destination socket address
	saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
	saDest.sin_family = AF_INET;
	saDest.sin_port = 0;

	// Tell the user what we're doing
	str.Format("Pinging %s [%s] with %d bytes of data:",
				pstrHost,
				inet_ntoa(saDest.sin_addr),
				REQ_DATASIZE);

	// Ping multiple times
	for (nLoop = 0; nLoop < nRetries; nLoop++)
	{
		// Send ICMP echo request
		SendEchoRequest(rawSocket, &saDest);

		//for (nLoop2 = 0; nLoop2 < 3; nLoop2++)
		//{
			nRet = WaitForEchoReply(rawSocket);
			if (nRet == SOCKET_ERROR)
			{
				WSAError("select()");
				break;
			}

			if (!nRet)
			{
				// 실패
				//::SendMessage(m_hWnd, WM_DISPLAY_BIT_SUMM_RST, false, iBITKind);
			}
			else
			{
				// Receive reply
				dwTimeSent = RecvEchoReply(rawSocket, &saSrc, &cTTL);

				ULONG iLastVal1 = saDest.sin_addr.S_un.S_addr;
				ULONG iLastVal2 = saSrc.sin_addr.S_un.S_addr;

				//TRACE("iLastVal1:%d - iLastVal2:%d\r\n", iLastVal1, iLastVal2);

				// 성공
				if ( iLastVal1 == iLastVal2 )
				{
					bRst = false;
					//TRACE("OK : %u \n ", dwTimeSent);
					break;
				}
				else
				{
					if(str == "192.168.0.254")
						//TRACE("FAIL : %u \n ", dwTimeSent);
					bRst = true;
				}				
			}
		//}
	}
	
	nRet = closesocket(rawSocket);
	
	if (nRet == SOCKET_ERROR)
		WSAError("closesocket()");

	return bRst;
}


bool CPing::SendPing(LPCSTR pstrHost)
{
	bool bRst = true;
	LPHOSTENT lpHost = gethostbyname(pstrHost);
	struct    sockaddr_in saDest;
	saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
	HANDLE hICMP=IcmpCreateFile();
	if (hICMP == INVALID_HANDLE_VALUE)
		return NULL;

	IP_OPTION_INFORMATION option;
	memset(&option, 0, sizeof(option));
	option.Ttl = 32;
	option.Tos = 0;
	option.Flags = IP_FLAG_DF;

	char * bufOut= new char[REQ_DATASIZE];
	memset(bufOut, 0, REQ_DATASIZE);
	char * bufIn = new char[REQ_DATASIZE*32];
	DWORD dwRet = IcmpSendEcho2(hICMP, NULL, NULL, NULL, *(IPAddr*)(&saDest.sin_addr), bufOut, REQ_DATASIZE, &option, bufIn, REQ_DATASIZE, 1000);

	PICMP_ECHO_REPLY *pRet = new PICMP_ECHO_REPLY;
	if (dwRet != 0) 
	{
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY) bufIn;
		*pRet = pEchoReply;

		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;

		/*
		if (dwRet > 1) 
		{
			TRACE("\tReceived %ld icmp message responses\n", dwRet);
			TRACE("\tInformation from the first response:\n");
		} 
		else {
			TRACE("\tReceived %ld icmp message response\n", dwRet);
			TRACE("\tInformation from this response:\n");
		}
		TRACE("\t  Received from %s\n", inet_ntoa(ReplyAddr));
		TRACE("\t  Status = %ld  ", pEchoReply->Status);
		*/

		switch (pEchoReply->Status) 
		{
		case IP_DEST_HOST_UNREACHABLE:
			//printf("(Destination host was unreachable)\n");
			bRst = true;
			break;
		case IP_DEST_NET_UNREACHABLE:
			//printf("(Destination Network was unreachable)\n");
			bRst = true;
			break;
		case IP_REQ_TIMED_OUT:
			//printf("(Request timed out)\n");
			bRst = true;
			break;
		default:
			bRst = false;
			printf("\n");
			break;
		}


	}

	delete pRet;

	return bRst;
}


/**
* 에코요청 전송
* @param s 소켓
* @param lpstToAddr 송신 어드레스
* @return 실행결과
*/
int CPing::SendEchoRequest(SOCKET s,LPSOCKADDR_IN lpstToAddr) 
{
	static ECHOREQUEST echoReq;
	static int nId = 1;
	static int nSeq = 1;
	int nRet;

	// Fill in echo request
	echoReq.icmpHdr.Type		= ICMP_ECHOREQ;
	echoReq.icmpHdr.Code		= 0;
	echoReq.icmpHdr.Checksum	= 0;
	echoReq.icmpHdr.ID			= nId++;
	echoReq.icmpHdr.Seq			= nSeq++;

	// Fill in some data to send
	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' '+nRet;

	// Save tick count when sent
	echoReq.dwTime				= GetTickCount();

	// Put data in packet and compute checksum
	echoReq.icmpHdr.Checksum = in_cksum((u_short *)&echoReq, sizeof(ECHOREQUEST));

	// Send the echo request  								  
	nRet = sendto(s,						/* socket */
				 (LPSTR)&echoReq,			/* buffer */
				 sizeof(ECHOREQUEST),
				 0,							/* flags */
				 (LPSOCKADDR)lpstToAddr, /* destination */
				 sizeof(SOCKADDR_IN));   /* address length */

	if (nRet == SOCKET_ERROR) 
		WSAError("sendto()");
	return (nRet);
}

/**
* 에코응답 수신
* @param s 소켓
* @param lpsaFrom 수신 소켓
* @param pTTL TTL 시간
* @return Dwell 시간
*/
DWORD CPing::RecvEchoReply(SOCKET s, LPSOCKADDR_IN lpsaFrom, u_char *pTTL) 
{
	ECHOREPLY echoReply;
	int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);

	// Receive the echo reply	
	nRet = recvfrom(s,					// socket
					(LPSTR)&echoReply,	// buffer
					sizeof(ECHOREPLY),	// size of buffer
					0,					// flags
					(LPSOCKADDR)lpsaFrom,	// From address
					&nAddrLen);			// pointer to address len

	// Check return value
	if (nRet == SOCKET_ERROR) 
		WSAError("recvfrom()");

	// return time sent and IP TTL
	*pTTL = echoReply.ipHdr.TTL;

	return(echoReply.echoRequest.dwTime);   		
}

/**
* 에코 응답 대기
* @param s 소켓 인스턴스
* @return 실행결과
*/
int CPing::WaitForEchoReply(SOCKET s)
{
	int iRst;
	struct timeval Timeout;
	fd_set readfds, stCopyReads;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = 0;
    Timeout.tv_usec = 10000;

	stCopyReads = readfds;
	iRst = select(0, &stCopyReads, NULL, NULL, &Timeout);
	
	if(SOCKET_ERROR == iRst)  // -1 
	{
		return iRst;
	}
	else if(0 != iRst) // select timeout 이 아니면 
	{
		for(unsigned int i=0; i<readfds.fd_count; i++)
		{
			if(FD_ISSET(readfds.fd_array[i], &stCopyReads))
			{
				if(readfds.fd_array[i] == s)
					return iRst;
			}
		}

		return NULL;
	}
	else
	{
		return iRst;
	}

	return iRst;
}

/**
* 에러 기록 기능
* @param pstrFrom 에러 문장
*/
void CPing::WSAError(LPCSTR lpMsg)
{
}

/**
* 체크섬 기능
* @param addr 주소 포인터
* @param len 크기
* @return 실행결과
*/
u_short CPing::in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if( nleft == 1 ) {
		u_short	u = 0;

		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}
