//
// THIS CODE IS BASED ON THE CODE FROM 
// THE BOOK WINSOCK 2.0 BY LEWIS NAPPER...
// 
//

#include "stdafx.h"
#include "pingICMP2.h"


CPingICMP2::CPingICMP2(void)
{
	
}

CPingICMP2::~CPingICMP2(void)
{

}

bool CPingICMP2::Ping(UINT nRetries, LPCSTR pstrHost, HWND hWnd, int iBITKind)
{
	HANDLE hIcmpFile;
	unsigned long ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	DWORD dwError = 0;
	char SendData[32] = "Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;

	ipaddr = inet_addr(pstrHost);
	if(ipaddr == INADDR_NONE)
	{
		TRACE("usage: %s IP address\n", pstrHost);
		return 1;
	}

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		TRACE("\tUnable to open handle.\n");
		TRACE("IcmpCreatefile returned error: %ld\n", GetLastError() );
		return 1;
	}

	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData) + 8;
	ReplyBuffer = (VOID*) malloc(ReplySize);
	if (ReplyBuffer == NULL) {
		TRACE("\tUnable to allocate memory fro reply buffer\n");
		return 1;
	}

	dwRetVal = IcmpSendEcho2(hIcmpFile, NULL, NULL, NULL, 
							ipaddr, SendData, sizeof(SendData), NULL, 
							ReplyBuffer, ReplySize, 250);

	if (dwRetVal != 0) 
	{
		CString strSrc = _T(""), strDes = _T("");
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		TRACE("\tSent icmp message to %s\n", pstrHost);
		strSrc.Format("%s", pstrHost);

		if (dwRetVal > 1) {
			TRACE("\tReceived %ld icmp message responses\n", dwRetVal);
			TRACE("\tInformation from the first response:\n");
		}
		else {
			TRACE("\tReceived %ld icmp message response\n", dwRetVal);
			TRACE("\tInformation from this response:\n");
		}
		TRACE("\t Received from %s\n", inet_ntoa( ReplyAddr ) );
		strDes.Format("%s", inet_ntoa( ReplyAddr ));
		TRACE("\t Status = %ld\n", pEchoReply->Status);

		if(strSrc.Compare(strDes) != 0)
			return 1;

		switch(pEchoReply->Status)
		{
		case IP_DEST_HOST_UNREACHABLE:
			TRACE("Destination host was unreachable\n");
			break;
		case IP_DEST_NET_UNREACHABLE:
			TRACE("Destination Network was unreachable\n");
			break;
		case IP_REQ_TIMED_OUT:
			TRACE("Request timed out\n");
			break;
		default:
			break;
		}

		TRACE("\t Roundtrip time = %ld milliseconds\n",
			pEchoReply->RoundTripTime);
	}
	else
	{
		TRACE("\tCall to IcmpSendEcho2 failed %s.\n", pstrHost);
		dwError =  GetLastError();
		switch(dwError)
		{
		case IP_BUF_TOO_SMALL:
			TRACE("ReplyBufferSize to small\n");
			break;
		case IP_REQ_TIMED_OUT:
			TRACE("Request timed out\n");
			break;		
		default:
			TRACE("Extended error returned : %ld\n", dwError);
			break;
		}
		return 1;
	}
	return 0;
}

bool CPingICMP2::AsyncPing()
{
	size_t i;
	DWORD  ret;
	HANDLE hIcmp;
	BYTE   PingData[16];

	/* Add IP Addresses to check. */
	/*new_Target( "66.139.79.229" );
	new_Target( "192.168.0.1"   );
	new_Target( "192.168.0.2"   );
	new_Target( "192.168.0.17"  );
	new_Target( "10.0.0.1"      );
	new_Target( "10.0.0.2"      );*/

	/* You can also check your entire subnet with something like: */
	/*for (i = 1;i < 256;i++)
	{
		char buf[50];
		sprintf(buf, "192.168.0.%d", i);
		new_Target(buf);
	}*/

	hIcmp = IcmpCreateFile();

	/* Send out all our async ping requests. Timeout is 250ms. Change if needed. */
	for (i = 0; i < stListTarget.cTargets; i++)
	{
		IcmpSendEcho2(hIcmp, stListTarget.Events[i], NULL, 0,
			stListTarget.IPAddresses[i].S_un.S_addr, PingData, sizeof(PingData), 
			NULL, stListTarget.Buffers[i], sizeof(stListTarget.Buffers[i]), 250);
	}

	/* Wait for all our ping requests to return or timeout. */
	WaitForMultipleObjects(stListTarget.cTargets, stListTarget.Events, TRUE, 5000);

	/* Check the results. */
	for (i = 0; i < stListTarget.cTargets; i++)
	{
		ret = wrapIcmpParseReplies(stListTarget.Buffers[i], sizeof(stListTarget.Buffers[i]));
		stListTarget.rsult[i] = (ret != 0 ? 0 : 1);
		TRACE("%s:  %s\n", ret != 0 ? "UP  " : "DOWN", inet_ntoa(stListTarget.IPAddresses[i]));
	}

	IcmpCloseHandle(hIcmp);

	getchar();
	return 0;
}

/*
 * Add an IP address to the Targets collection.
 */
BOOL CPingICMP2::new_Target(LPCSTR szIPAddress)
{
	if (stListTarget.cTargets < MAX_TARGETS)
	{
		stListTarget.Events[stListTarget.cTargets] = CreateEvent(NULL, FALSE, FALSE, NULL);
		stListTarget.IPAddresses[stListTarget.cTargets].S_un.S_addr = inet_addr(szIPAddress);
		stListTarget.cTargets++;

		return TRUE;
	}

	return FALSE;
}

/*
 * There appears to be a problem with the import lib for IcmpParseReplies
 * so we load it at run-time.
 */
DWORD CPingICMP2::wrapIcmpParseReplies(LPVOID ReplyBuffer, DWORD ReplySize)
{
	typedef int (WINAPI * pIPR)(LPVOID, DWORD);
	static pIPR dl_IcmpParseReplies;

	if (!dl_IcmpParseReplies)
	{
		HMODULE hICMP = LoadLibrary(TEXT("ICMP.dll"));
		dl_IcmpParseReplies = (pIPR) GetProcAddress(hICMP, "IcmpParseReplies");
	}

	if (dl_IcmpParseReplies)
		return dl_IcmpParseReplies(ReplyBuffer, ReplySize);
	else
		return 0;
}