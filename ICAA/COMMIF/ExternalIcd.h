#ifndef __EXTERNALICD_H__
#define __EXTERNALICD_H__

#include "ExternalICDCommon.h"

#pragma pack(1)

#define SZ_MAX_DATA_LEN						65536						// 64K
#define SZ_MAX_MSG_LEN						(SZ_MAX_DATA_LEN+20)		// 64K+20(stLHdrNFrame+stMHdr)
#define SZ_MAX_LOGMSG_LEN					(SZ_MAX_DATA_LEN-4)			// 64K-4(usTotalCount+usCurrentNumber)

#define VOICE_FRAME_COUNT					2							// 1회 전송(40ms 당)시  전송하는 프레임 개수
#define VOICE_FRAME_DATA_COMP_SIZE			70							// 프레임 당 데이터 크기(인코딩후) = 160 샘플  x 샘플 당 2바이트
#define VOICE_FRAME_DATA_UNCOMP_SIZE		640							// 프레임 당 데이터 크기(인코딩전) = 160 샘플 x 20ms x 샘플 당 2바이트

/*********************************************************************
*
*                           외부 인터페이스
*
*                              패킷 형식
*
*********************************************************************/
// 링크 헤더 중 LSize에 포함되지 않는 영역
#if defined(BIG_ENDIAN)
	typedef struct
	{
		UINT		ACID				: 4	;	// 항공기 식별 번호
		UINT		E					: 1	;	// 암호화 여부
		UINT		C					: 1	;	// 압축 여부
		UINT		A					: 1	;	// ACK 여부
		UINT		R					: 1	;	// 재전송 필요 여부
		UINT		F					: 1	;	// Frame 분할 여부
		UINT		LSize               : 23;	// 전체 크기
	} stLHdrFlag;	

	typedef union
	{
		UINT			uiData;
		stLHdrFlag		stData;
	}unLHdrFlag;
#else
	typedef struct
	{
		UINT		LSize               : 23;	// 전체 크기
		UINT		F					: 1	;	// Frame 분할 여부
		UINT		R					: 1	;	// 재전송 필요 여부
		UINT		A					: 1	;	// ACK 여부
		UINT		C					: 1	;	// 압축 여부
		UINT		E					: 1	;	// 암호화 여부
		UINT		ACID				: 4	;	// 항공기 식별 번호
	} stLHdrFlag;

	typedef union
	{
		UINT			uiData;
		stLHdrFlag		stData;
	}unLHdrFlag;
#endif	//	#if defined(BIG_ENDIAN)

// 링크 헤더 중 LSize에 포함되는 영역
typedef struct
{
	UINT		SeqNum;	// 메시지 일련 번호
} stLHdrRest;

// 링크헤더(프레임1)
#if defined(BIG_ENDIAN)	
	typedef struct
	{
		unLHdrFlag LHdrFlag;				//
		UINT	   uiSeqNum;				// 메시지 일련 번호
	} stLHdr1Frame;
#else
	typedef struct
	{
		unLHdrFlag LHdrFlag;				//
		UINT	   uiSeqNum;				// 메시지 일련 번호
	} stLHdr1Frame;
#endif	//	#if defined(BIG_ENDIAN)

// 링크헤더(프레임N)
#if defined(BIG_ENDIAN)
	typedef struct
	{
		unLHdrFlag	LHdrFlag;				//
		UINT		uiSeqNum;				// 메시지 일련 번호
		USHORT		usTotalFrameCount;		// 전체 Frame 수(Option)
		USHORT		usFrameNum;				// Frame 번호(Option)
	} stLHdrNFrame;	
#else
	typedef struct
	{
		unLHdrFlag	LHdrFlag;				//
		UINT		uiSeqNum;				// 메시지 일련 번호
		USHORT		usTotalFrameCount;		// 전체 Frame 수(Option)
		USHORT		usFrameNum;				// Frame 번호(Option)
	} stLHdrNFrame;	
#endif	//	#if defined(BIG_ENDIAN)

// OPCODE
	#if defined(BIG_ENDIAN)
	typedef union
	{
		UCHAR				ucData;
		struct
		{
			UCHAR   Encrytion       : 1 ;		// 압축여부
			UCHAR   Class			: 4	;		// 메시지 우선순위
			CHAR   O				: 4	;		// 운용자 구분			
		}BIT_FIELD;
	}  unCO;
#else
	typedef union
	{
		UCHAR				ucData;
		struct
		{
			UCHAR   O				: 4	;		// 운용자 구분
			UCHAR   Class			: 3	;		// 메시지 우선순위
			UCHAR   Encrytion       : 1 ;		// 압축여부
		}BIT_FIELD;
	} unCO; 
#endif	//	#if defined(BIG_ENDIAN)

#if defined(BIG_ENDIAN)
	typedef struct
	{
		UCHAR   Src;						// 송신장비
		UCHAR   Dest;						// 수신장비
		UCHAR   CmdCode;					// 수행 명령
		unCO	CO;
	} stOpcode;	
#else
	typedef struct
	{
		unCO	CO;
		UCHAR   CmdCode;					// 수행 명령
		UCHAR   Dest;						// 수신장비
		UCHAR   Src;						// 송신장비		
	} stOpcode;
#endif	//	#if defined(BIG_ENDIAN)

// 임무 헤더
typedef struct
{
	stOpcode	Opcode;				// Opcode
	USHORT		usMSize;			// 데이터 영역 크기
	UCHAR		ucSourcdeOprID;		// SOURCE 운용자 구분필드
	UCHAR		ucDestinationOprID; // Destination 운용자 구분필드
	//USHORT		usOperatorID;		// 운용자 식별 ID
} stMHdr;

// 링크프레임(프레임N)
typedef struct
{
	stLHdrNFrame	LHdr;					// 링크 헤더
	stMHdr			MHdr;					// 임무 헤더
	char			Data[SZ_MAX_DATA_LEN];	// 데이터
} stMsgNFrame;

typedef union
{
	stMsgNFrame  Msg;
	char		 Data[SZ_MAX_MSG_LEN];
} unMsgNFrame;

// 링크프레임(프레임1)
typedef struct
{
	stLHdr1Frame	LHdr;					// 링크 헤더
	stMHdr			MHdr;					// 임무 헤더
	char			Data[SZ_MAX_DATA_LEN];	// 데이터
} stMsg1Frame;

typedef union
{
	stMsg1Frame  Msg;
	char		 Data[SZ_MAX_MSG_LEN];
} unMsg1Frame;

// 임무프레임
typedef struct
{
	stMHdr		MHdr;					// 임무 헤더
	char		Data[SZ_MAX_DATA_LEN];	// 데이터
} stMsgMission;

typedef union
{
	stMsgMission  Msg;
	char		  Data[SZ_MAX_MSG_LEN];
} unMsgMission;

/*********************************************************************
*
*                           외부 인터페이스
*
*                              OPCODE
*
*********************************************************************/
// GMEI->AIE
#define EOPCODE_GMEI_ReqInit						0x01		// 초기화 요구
#define EOPCODE_GMEI_AckInitResult					0x02		// 초기화 결과 응답
#define EOPCODE_GMEI_ReqPBIT						0x03		// 초기자체점검 요구
#define EOPCODE_GMEI_AckPBITResult					0x04		// 초기자체점검 결과 응답
#define EOPCODE_GMEI_ReqCBIT						0x05		// 연속자체점검 요구
#define EOPCODE_GMEI_ReqIBIT						0x06		// 운용자 자체점검 요구
#define EOPCODE_GMEI_AckIBITResult					0x07		// 운용자 자체점검 결과 응답
#define EOPCODE_GMEI_ReqFISINTPathCheck				0x08		// FISINT 경로점검 요구
#define EOPCODE_GMEI_AckFISINTPathCheckResult		0x09		// FISINT 경로점검 결과 응답
#define EOPCODE_GMEI_ReqPowerControl				0x12	    // 임무장비전원제어요구	
#define EOPCODE_GMEI_AckPowerControlResult			0x13		// 임무장비전원제어결과 응답	
#define EOPCODE_GMEI_AckPowerStatus					0x14		// 임무장비전원상태변경 응답	
#define EOPCODE_GMEI_LogDataRequest					0x30		// 저장데이터 요구
#define EOPCODE_GMEI_UpwardVoice					0x40		// 상향음성	

// AIE->GMEI
#define EOPCODE_GMEI_AckInit						0x01		// 초기화 요구 응답
#define EOPCODE_GMEI_SndInitResult					0x02		// 초기화 결과
#define EOPCODE_GMEI_AckPBIT						0x03		// 초기자체점검 요구 응답
#define EOPCODE_GMEI_SndPBITResult					0x04		// 초기자체점검 결과
#define EOPCODE_GMEI_SndCBITResult					0x05		// 연속자체점검 결과
#define EOPCODE_GMEI_AckIBIT						0x06		// 운용자 자체점검 요구응답
#define EOPCODE_GMEI_SndIBITResult					0x07		// 운용자 자체점검 결과
#define EOPCODE_GMEI_AckFISINTPathCheck				0x08		// FISINT 경로점검 요구 응답
#define EOPCODE_GMEI_SndFISINTPathCheckResult		0x09		// FISINT 경로점검 결과
#define EOPCODE_GMEI_SndZeroizeResult				0x11		// 자료삭제 알림
#define EOPCODE_GMEI_AckPowerControl				0x12		// 임무장비전원제어요구응답
#define EOPCODE_GMEI_SndPowerControlResult			0x13		// 임무장비전원제어결과
#define EOPCODE_GMEI_SndPowerStatus					0x14		// 임무장비전원상태변경
#define EOPCODE_GMEI_SndNavigationInfo				0x20		// 항법정보전송
#define EOPCODE_GMEI_SndTimeInfo					0x21		// 시간정보전송
#define EOPCODE_GMEI_LogData						0x30		// 저장데이터 전송
#define EOPCODE_GMEI_CreatedDataSize				0x31		// 생성데이터량 전송
#define EOPCODE_GMEI_DownwardVoice					0x41		// 하향음성
#define EOPCODE_AIE_GMEI_WOWState					0x50		// WOW상태


/*********************************************************************
*
*                           외부 인터페이스
*
*                             필드값 정의
*
*********************************************************************/
// 송/수신 장비 코드
#define EQUIP_AIR_GND								0x80	// 항공/지상 구분
#define EQUIP_AIR_ACE								0x20	// 항공 COMINT 장비(ACE)
#define EQUIP_AIR_AEE								0x10	// 항공 ELINT 장비(AEE)
#define EQUIP_AIR_AFE								0x08	// 항공 FISINT 장비(AFE)
#define EQUIP_AIR_PDE								0x04	// 화염탐지장비(PDE)
#define EQUIP_AIR_AIE								0x02	// 항공연동장비(AIE)
#define EQUIP_AIR_ADE								0x01	// 항공데이터링크장비(ADE)
#define EQUIP_GND_GMEO								0x08	// 지상임무장비-운용(GMEO)
#define EQUIP_GND_GMEI								0x04	// 지상임무장비-연동(GMEI)
#define EQUIP_GND_GMED								0x02	// 지상임무장비-DL(GMED)
#define EQUIP_GND_GDE								0x01	// 지상데이터링크장비(GDE)
#define EQUIP_BROADSTING							0x00	// 공통(Broadsting)

#define EQUIP_INTERNAL_AIPU							0x21	// 항공임무연동장치
#define EQUIP_INTERNAL_PDU							0x22	// 전원분배장치
#define EQUIP_INTERNAL_IFBD_CTRL					0x23	// 통합연동판 - 제어
#define EQUIP_INTERNAL_IFBD_IDPU					0x24	// 통합연동판 - 암호장비연동
#define EQUIP_INTERNAL_SBC_HEARTBEAT				0x25	// SBC간 이중화
#define EQUIP_INTERNAL_SBC_LOG						0x26	// SBC간 저장

// 항공기 ID
#define AIRCRAFT_ID_1								0x01
#define AIRCRAFT_ID_2								0x02

// 클래스
#define CLASS_1										1		// 클래스 1 : 제어결과, 분석결과
#define CLASS_2										2		// 클래스 2 : Telemetry, 화염
#define CLASS_3										3		// 클래스 3 : 음성, 복조 결과
#define CLASS_4										4		// 클래스 4 : 요청 데이터
#define CLASS_5										5		// 클래스 5 : 수집데이터(스펙트럼)
#define CLASS_COUNT									5		// 클래스 개수

// 운용자 구분
#define OPR_NA										0		// 운용자구분 : NA
#define OPR_TST										1		// 운용자구분 : TST
#define OPR_DLO										2		// 운용자구분 : DLO
#define OPR_ETC										3		// 운용자구분 : 기타(muphiri 추가 정의 필요)

// 운용자 ID
#define OPRID_DEFAULT								0		// 운용자ID : 기본

#define FLAG_ENCRY_YES								0x0		// 암호화 여부 : 데이터영역 암호화
#define FLAG_ENCRY_NO								0x1		// 암호화 여부 : 데이터영역 비암호화

#define FLAG_COMP_YES								0x0		// 압축 여부 : 데이터영역 압축
#define FLAG_COMP_NO								0x1		// 압축 여부 : 데이터영역 비압축

#define FLAG_ACK_NO									0x0		// ACK 여부 : 메시지
#define FLAG_ACK_YES								0x1		// ACK 여부 : 메시지에 대한 ACK

#define FLAG_RETRY_YES								0x0		// 재전송 필요 여부 : 재전송 필요
#define FLAG_RETRY_NO								0x1		// 재전송 필요 여부 : 재전송 불필요

#define FLAG_FRAME_YES								0x0		// Frame 분할 여부 : Frame 분할
#define FLAG_FRAME_NO								0x1		// Frame 분할 여부 : Frame 미분할

#define DLSTAT_LINK_OK_EQUIP_OK						0xFFFF	// 데이터링크 상태:링크 정상, 장비 정상
#define DLSTAT_LINK_OK_EQUIP_FAIL					0xFF00	// 데이터링크 상태:링크 정상, 장비 이상
#define DLSTAT_LINK_FAIL_EQUIP_OK					0x00FF	// 데이터링크 상태:링크 단절, 장비 정상
#define DLSTAT_LINK_FAIL_EQUIP_FAIL					0x0000	// 데이터링크 상태:링크 단절, 장비 이상

#define EQUIP_STATE_NOT_VALID						0x0000	// Not Valid(지상으로부터 요청을 받지 않아 데이터필드 유효하지 않음)
#define EQUIP_STATE_ING								0x0001	// 진행중(지상으로 전송시 진행중은 응답없음으로 보내야 함)
#define EQUIP_STATE_NOT_RESPOND						0x0002	// 응답없음(데이터필드유효하지 않음)
#define EQUIP_STATE_COMPLETE						0x0003	// 완료

#define ZEROIZE_STATE_ING							0x0000	// 진행중
#define ZEROIZE_STATE_COMPLETE						0x0001	// 완료

#define BIT_RESULT_FAIL								0		// 자체점검결과 정상
#define BIT_RESULT_OK								1		// 자체점검결과 비정상

#define FISINT_PATH_OK								0x00	// C/E 경로 정상
#define FISINT_PATH_COK_EFAIL						0x0F	// C경로 정상/E경로 비정상
#define FISINT_PATH_CFAIL_EOK						0xF0	// C경로 비정상/E경로 정상
#define FISINT_PATH_CFAIL_EFAIL						0xFF	// CE 경로 비정상

#define MSG_STATUS_ING								0		// 진행중
#define MSG_STATUS_COMPLETE							1		// 완료

#define MSG_STATUS_AEE_NOTDOING				0		// 미수행
#define MSG_STATUS_AEE_ING					1		// 진행중
#define MSG_STATUS_AEE_COMPLETE				2		// 완료

#define RETRY_NO_OPCODE_GR_OPERATOR_AUDIO		0x04824000

#define	MAX_RETRY_CNT				3
#define RETRY_INTERVAL_TIME			(3000)

#pragma pack()

#endif // #ifndef __EXTERNALICD_H__