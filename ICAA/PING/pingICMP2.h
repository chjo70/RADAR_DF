//
// Ping.h
//

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#if defined(_MSC_VER)
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

using namespace std;

//test
#define MAX_TARGETS 300
typedef struct  target
{
	struct in_addr IPAddresses[MAX_TARGETS];
	BYTE           Buffers    [MAX_TARGETS][sizeof(ICMP_ECHO_REPLY) + 32];
	HANDLE         Events     [MAX_TARGETS];
	UINT           cTargets;
	DWORD          rsult[MAX_TARGETS];
} Targets;

class CPingICMP2
{

public:
	CPingICMP2(void);
	virtual ~CPingICMP2(void);

	/**
	* 보안 장비 자체점검 결과를 상위로 전달하기 위한 윈도우 핸들
	*/
public:
	bool Ping(UINT nRetries,LPCSTR pstrHost,HWND hWnd, int iBITKind);	
	bool AsyncPing();
	BOOL new_Target(LPCSTR szIPAddress);
	DWORD wrapIcmpParseReplies(LPVOID ReplyBuffer, DWORD ReplySize);

public:
	Targets stListTarget;
};


