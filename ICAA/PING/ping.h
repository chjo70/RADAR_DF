//
// Ping.h
//

#pragma pack(1)

#define ICMP_ECHOREPLY	0
#define ICMP_ECHOREQ	8

/** 
* SDD식별자:D-CM-SFD-ACIF
*
* (1) 기능
*  - 보안장비 점검을 위해 사용
*  - 통신상태 확인
*
* (2) 설계결정사항
*  - 해당 사항 없음
*/
class CPing
{
public:
	
	/**
	* 보안 장비 자체점검 결과를 상위로 전달하기 위한 윈도우 핸들
	*/
	HWND m_hWnd;
	
	bool Ping(UINT nRetries,LPCSTR pstrHost,HWND hWnd, int iBITKind);	
	int  WaitForEchoReply(SOCKET s);	
	int		SendEchoRequest(SOCKET, LPSOCKADDR_IN);	
	DWORD	RecvEchoReply(SOCKET, LPSOCKADDR_IN, u_char *);	
	u_short in_cksum(u_short *addr, int len);

	bool SendPing(LPCSTR pstrHost);
protected:	
	void WSAError(LPCSTR pstrFrom);

};

// IP Header -- RFC 791
typedef struct tagIPHDR
{
	u_char  VIHL;			// Version and IHL
	u_char	TOS;			// Type Of Service
	short	TotLen;			// Total Length
	short	ID;				// Identification
	short	FlagOff;		// Flags and Fragment Offset
	u_char	TTL;			// Time To Live
	u_char	Protocol;		// Protocol
	u_short	Checksum;		// Checksum
	struct	in_addr iaSrc;	// Internet Address - Source
	struct	in_addr iaDst;	// Internet Address - Destination
}IPHDR, *PIPHDR;


// ICMP Header - RFC 792
typedef struct tagICMPHDR
{
	u_char	Type;			// Type
	u_char	Code;			// Code
	u_short	Checksum;		// Checksum
	u_short	ID;				// Identification
	u_short	Seq;			// Sequence
	char	Data;			// Data
}ICMPHDR, *PICMPHDR;


#define REQ_DATASIZE 32		// Echo Request Data size

// ICMP Echo Request
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;


// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;

/** 
* SDD 식별자 : D-CM-SFD-ACIF
*
* (1) 기능
*  - 보안장비 점검을 위해 사용
*  - IP 정보를 관리
*
* (2) 설계결정사항
*  - 해당 사항 없음
*/
class CIPData : public CObject
{
public:
	CIPData() {};
	~CIPData() {};

public:
	/**
	* 보안장비 IP
	*/
	CString m_strIP;
	/**
	* 보안장비명
	*/
	CString m_strName;
};

#pragma pack()

