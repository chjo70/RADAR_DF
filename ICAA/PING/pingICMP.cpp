//
// THIS CODE IS BASED ON THE CODE FROM 
// THE BOOK WINSOCK 2.0 BY LEWIS NAPPER...
// 
//

#include "stdafx.h"
#include "pingICMP.h"

bool CPingICMP::Ping(UINT nRetries,LPCSTR pstrHost,HWND hWnd, int iBITKind)
{
	int nSequence = 0;
	int nMessageSize = 32;	//The message size that the ICMP echo request should carry with it
	int nTimeOut = 100;	//Request time out for echo request (in milliseconds)
	int nCount = nRetries;	//Number of times the request has to be send

	bool bPingResut = false;

	char *pszRemoteIP = NULL, *pSendBuffer = NULL, *pszRemoteHost = NULL;

	pszRemoteHost = (char*)pstrHost;	

	if (ResolveIP (pszRemoteHost, &pszRemoteIP) == false)
	{
		//cerr << endl << "Unable to resolve hostname" << endl;
		TRACE("Unable to resolve hostname \n");
		//bPingResut = true;
	}
	//cout << "Pinging " << pszRemoteHost << " [" << pszRemoteIP << "] with " << nMessageSize << " bytes of data." << endl << endl;
	ICMPheader sendHdr;

	SOCKET sock;
	sock = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);	//Create a raw socket which will use ICMP

	SOCKADDR_IN dest;	//Dest address to send the ICMP request
	dest.sin_addr.S_un.S_addr = inet_addr (pszRemoteIP);
	dest.sin_family = AF_INET;
	dest.sin_port = rand ();	//Pick a random port

	int nResult = 0;

	fd_set fdRead;
	SYSTEMTIME timeSend, timeRecv;
	int nTotalRoundTripTime = 0, nMaxRoundTripTime = 0, nMinRoundTripTime = -1, nRoundTripTime = 0;
	int nPacketsSent = 0, nPacketsReceived = 0;

	timeval timeInterval = {0, 0};
	timeInterval.tv_usec = nTimeOut * 1000;
	
	sendHdr.nId = htons (rand ());	//Set the transaction Id

	while (nPacketsSent < nCount)
	{
		//Create the message buffer, which is big enough to store the header and the message data
		pSendBuffer = new char [sizeof (ICMPheader) + nMessageSize];

		sendHdr.byCode = 0;	//Zero for ICMP echo and reply messages
		sendHdr.nSequence = htons (nSequence++);
		sendHdr.byType = 8;	//Eight for ICMP echo message
		sendHdr.nChecksum = 0;	//Checksum is calculated later on

		memcpy_s (pSendBuffer, sizeof (ICMPheader), &sendHdr, sizeof (ICMPheader));	//Copy the message header in the buffer
		memset (pSendBuffer + sizeof (ICMPheader), 'x', nMessageSize);	//Fill the message with some arbitary value

		//Calculate checksum over ICMP header and message data
		sendHdr.nChecksum = htons (CalcChecksum (pSendBuffer, sizeof (ICMPheader) + nMessageSize));	

		//Copy the message header back into the buffer
		memcpy_s (pSendBuffer, sizeof (ICMPheader), &sendHdr, sizeof (ICMPheader));

		nResult = sendto (sock, pSendBuffer, sizeof (ICMPheader) + nMessageSize, 0, (SOCKADDR *)&dest, sizeof (SOCKADDR_IN));
		
		//Save the time at which the ICMP echo message was sent
		::GetSystemTime (&timeSend);

		++nPacketsSent;

		if (nResult == SOCKET_ERROR)
		{
			TRACE("An error occured in sendto operation \n");
			//cerr << endl << "An error occured in sendto operation: " << "WSAGetLastError () = " << WSAGetLastError () << endl;
			UnInitialize ();
			delete []pSendBuffer;
			//bPingResut = true;
		}
		
		FD_ZERO (&fdRead);
		FD_SET (sock, &fdRead);

		if ((nResult = select (0, &fdRead, NULL, NULL, &timeInterval))
			== SOCKET_ERROR)
		{
			/*cerr << endl << "An error occured in select operation: " << "WSAGetLastError () = " << 
			WSAGetLastError () << endl;*/
			TRACE("An error occured in select operation \n");

			delete []pSendBuffer;
			//bPingResut = true;
		}

		if (nResult > 0 && FD_ISSET (sock, &fdRead))
		{
			//Allocate a large buffer to store the response
			char *pRecvBuffer = new char [1500];

			if ((nResult = recvfrom (sock, pRecvBuffer, 1500, 0, 0, 0)) 
				== SOCKET_ERROR)
			{
				/*cerr << endl << "An error occured in recvfrom operation: " << "WSAGetLastError () = " << 
					WSAGetLastError () << endl;*/

				TRACE("An error occured in recvfrom operation \n");
				UnInitialize ();
				delete []pSendBuffer;
				delete []pRecvBuffer;
				//bPingResut = true;
			}
			
			//Get the time at which response is received
			::GetSystemTime (&timeRecv);

			//We got a response so we construct the ICMP header and message out of it
			ICMPheader recvHdr;
			char *pICMPbuffer = NULL;

			//The response includes the IP header as well, so we move 20 bytes ahead to read the ICMP header
			pICMPbuffer = pRecvBuffer + sizeof(IPheader);

			//ICMP message length is calculated by subtracting the IP header size from the 
			//total bytes received
			int nICMPMsgLen = nResult - sizeof(IPheader);

			//Construct the ICMP header
			memcpy_s (&recvHdr, sizeof (recvHdr), pICMPbuffer, sizeof (recvHdr));

			//Construct the IP header from the response
			IPheader ipHdr;
			memcpy_s (&ipHdr, sizeof (ipHdr), pRecvBuffer, sizeof (ipHdr));

			recvHdr.nId = recvHdr.nId;
			recvHdr.nSequence = recvHdr.nSequence;
			recvHdr.nChecksum = ntohs (recvHdr.nChecksum);

			//Check if the response is an echo reply, transaction ID and sequence number are same
			//as for the request, and that the checksum is correct
			if (recvHdr.byType == 0 &&
				recvHdr.nId == sendHdr.nId &&
				recvHdr.nSequence == sendHdr.nSequence &&
				ValidateChecksum (pICMPbuffer, nICMPMsgLen)  && 
				memcmp (pSendBuffer + sizeof(ICMPheader), pRecvBuffer + sizeof (ICMPheader) + sizeof(IPheader), 
					nResult - sizeof (ICMPheader) - sizeof(IPheader)) == 0)
			{
				//All's OK
				int nSec = timeRecv.wSecond - timeSend.wSecond;
				if (nSec < 0)
				{
					nSec = nSec + 60;
				}

				int nMilliSec = abs (timeRecv.wMilliseconds - timeSend.wMilliseconds);

				int nRoundTripTime = 0;
				nRoundTripTime = abs (nSec * 1000 - nMilliSec);

				/*cout << "Reply from " << pszRemoteIP << ": bytes = " << nResult - sizeof (ICMPheader) - sizeof (IPheader) 
				<< ", time = " << nRoundTripTime << "ms, TTL = " << (int)ipHdr.byTtl << endl;*/

				nTotalRoundTripTime = nTotalRoundTripTime + nRoundTripTime;

				if (nMinRoundTripTime == -1)
				{
					nMinRoundTripTime = nRoundTripTime;
					nMaxRoundTripTime = nRoundTripTime;
				}
				else if (nRoundTripTime < nMinRoundTripTime)
				{
					nMinRoundTripTime = nRoundTripTime;
				}
				else if (nRoundTripTime > nMaxRoundTripTime)
				{
					nMaxRoundTripTime = nRoundTripTime;
				}

				++nPacketsReceived;
			}
			else
			{
				//TRACE("The echo reply is not correct! \n");
				//bPingResut = true;
				//cout << "The echo reply is not correct!" << endl;
			}

			delete []pRecvBuffer;
		}
		else
		{
			//TRACE("Request timed out. \n");
			//bPingResut = true;
			//cout << "Request timed out." << endl;
		}

		delete []pSendBuffer;
	}

	/*cout << endl << "Ping statistics for " << pszRemoteIP << ":" << endl << '\t' << "Packets: Sent = " << nPacketsSent << ", Received = " << 
	nPacketsReceived << ", Lost = " << (nPacketsSent - nPacketsReceived) << " (" << 
	((nPacketsSent - nPacketsReceived)/(float)nPacketsSent) * 100 << "% loss)" << endl << '\t';*/

	TRACE("Ping statistics for %s : Packets: Sent =  %d, Received = %d,  Lost = %d (%f  loss)\t\n", pszRemoteIP, nPacketsSent, nPacketsReceived, (nPacketsSent - nPacketsReceived)
		, ((nPacketsSent - nPacketsReceived)/(float)nPacketsSent) * 100 );

	if(nPacketsReceived == 0)
		bPingResut = true;

	if (nPacketsReceived > 0)
	{
		/*cout << "\rApproximate round trip times in milli-seconds:" << endl << '\t' << "Minimum = " << nMinRoundTripTime << 
		"ms, Maximum = " << nMaxRoundTripTime << "ms, Average = " << nTotalRoundTripTime / (float)nPacketsReceived << "ms" << endl;*/

		//TRACE("Approximate round trip times in milli-seconds: Minimum = %d, Maximum = %d,  Average = %d \t\n", nMinRoundTripTime, nMaxRoundTripTime,nTotalRoundTripTime / (float)nPacketsReceived);
	}

	cout << '\r' << endl;

	/*if (UnInitialize () == false)
	{
	return false;
	}*/

	return bPingResut;
}

unsigned short CPingICMP::CalcChecksum (char *pBuffer, int nLen)
{
	unsigned short nWord;
	unsigned int nSum = 0;
	int i;
    
	//Make 16 bit words out of every two adjacent 8 bit words in the packet
	//and add them up
	for (i = 0; i < nLen; i = i + 2)
	{
		nWord =((pBuffer [i] << 8)& 0xFF00) + (pBuffer [i + 1] & 0xFF);
		nSum = nSum + (unsigned int)nWord;	
	}
	
	//Take only 16 bits out of the 32 bit sum and add up the carries
	while (nSum >> 16)
	{
		nSum = (nSum & 0xFFFF) + (nSum >> 16);
	}

	//One's complement the result
	nSum = ~nSum;
	
	return ((unsigned short) nSum);
}

bool CPingICMP::ValidateChecksum (char *pBuffer, int nLen)
{
	unsigned short nWord;
	unsigned int nSum = 0;
	int i;
    
	//Make 16 bit words out of every two adjacent 8 bit words in the packet
	//and add them up
	for (i = 0; i < nLen; i = i + 2)
	{
		nWord =((pBuffer [i] << 8)& 0xFF00) + (pBuffer [i + 1] & 0xFF);
		nSum = nSum + (unsigned int)nWord;	
	}
	
	//Take only 16 bits out of the 32 bit sum and add up the carries
	while (nSum >> 16)
	{
		nSum = (nSum & 0xFFFF) + (nSum >> 16);
	}

	//To validate the checksum on the received message we don't complement the sum
	//of one's complement
	//One's complement the result
	//nSum = ~nSum;
	
	//The sum of one's complement should be 0xFFFF
	return ((unsigned short)nSum == 0xFFFF);
}

bool CPingICMP::Initialize ()
{
WSADATA wsaData;

	if (WSAStartup (MAKEWORD (2, 2), &wsaData) == SOCKET_ERROR)
	{
		TRACE("An error occured in WSAStartup operation \n");
		//cerr << endl << "An error occured in WSAStartup operation: " << "WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}

	SYSTEMTIME time;
	::GetSystemTime (&time);

	//Seed the random number generator with current millisecond value
	srand (time.wMilliseconds);

	return true;
}

bool CPingICMP::UnInitialize ()
{
//Cleanup
	if (WSACleanup () == SOCKET_ERROR)
	{
		TRACE("An error occured in WSACleanup operation \n");
		//cerr << endl << "An error occured in WSACleanup operation: WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}

	return true;
}

bool CPingICMP::ResolveIP (char *pszRemoteHost, char **pszIPAddress)
{
	hostent *pHostent = gethostbyname (pszRemoteHost);
	if (pHostent == NULL)
	{
		TRACE("An error occured in gethostbyname operation \n");
		//cerr << endl << "An error occured in gethostbyname operation: WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}

	in_addr in;
	memcpy_s (&in, sizeof (in_addr), pHostent->h_addr_list [0], sizeof (in_addr));
	*pszIPAddress = inet_ntoa (in);

	return true;
}
