// RcvFuncTCP.h: interface for the CRcvFuncTCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCVFUNCTCP_H__EB35F31F_0D4B_4A0A_8DEA_15CA350A5CB3__INCLUDED_)
#define AFX_RCVFUNCTCP_H__EB35F31F_0D4B_4A0A_8DEA_15CA350A5CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
//DEL #include "def_scpi.h"
//DEL #include "def_scpi_cmd.h"
#define UM_REQ_TIME  77778
#define UM_REP_TIME  77779

#include "def_struct.h"

#include "../library/inAdLibB/SocketFunc.h"

class CMySocket;

#define _USE_BSR_SYSTEM

//////////////////////////////////////////////////////////////////////////
#define CFG_SECTION_RCV_FUNC_NETWORK				"RCV_FUNC_NETWORK_SETTINGS"							// 섹션			: 

#define CFG_ENTRY_SCPI_IP_ADDRESS_1					"SCPI_IP_ADDRESS_1"									// 엔티티명		: 
#define CFG_DEFAULT_SCPI_IP_ADDRESS_1				"192.168.10.101"										// 엔티티값		: 
#define CFG_ENTRY_SCPI_PORT_NUMBER_1				"SCPI_PORT_NUMBER_1"								// 엔티티명		: 
#define CFG_DEFAULT_SCPI_PORT_NUMBER_1				"10001"												// 엔티티값		: 

#define CFG_ENTRY_SCPI_IP_ADDRESS_2					"SCPI_IP_ADDRESS_2"									// 엔티티명		: 
#define CFG_DEFAULT_SCPI_IP_ADDRESS_2				"192.168.10.102"										// 엔티티값		: 
#define CFG_ENTRY_SCPI_PORT_NUMBER_2				"SCPI_PORT_NUMBER_2"								// 엔티티명		: 
#define CFG_DEFAULT_SCPI_PORT_NUMBER_2				"10001"												// 엔티티값		: 

#define CFG_ENTRY_SCPI_IP_ADDRESS_3					"SCPI_IP_ADDRESS_3"									// 엔티티명		: 
#define CFG_DEFAULT_SCPI_IP_ADDRESS_3				"192.168.10.103"										// 엔티티값		: 
#define CFG_ENTRY_SCPI_PORT_NUMBER_3				"SCPI_PORT_NUMBER_3"								// 엔티티명		: 
#define CFG_DEFAULT_SCPI_PORT_NUMBER_3				"10001"												// 엔티티값		: 

#define CFG_ENTRY_SCPI_IP_ADDRESS_4					"SCPI_IP_ADDRESS_4"									// 엔티티명		: 
#define CFG_DEFAULT_SCPI_IP_ADDRESS_4				"192.168.10.104"										// 엔티티값		: 
#define CFG_ENTRY_SCPI_PORT_NUMBER_4				"SCPI_PORT_NUMBER_4"								// 엔티티명		: 
#define CFG_DEFAULT_SCPI_PORT_NUMBER_4				"10001"												// 엔티티값		: 

#define CFG_ENTRY_SCPI_IP_ADDRESS_5					"SCPI_IP_ADDRESS_5"									// 엔티티명		: 
#define CFG_DEFAULT_SCPI_IP_ADDRESS_5				"192.168.10.105"										// 엔티티값		: 
#define CFG_ENTRY_SCPI_PORT_NUMBER_5				"SCPI_PORT_NUMBER_5"								// 엔티티명		: 
#define CFG_DEFAULT_SCPI_PORT_NUMBER_5				"10001"												// 엔티티값		: 

#define CFG_ENTRY_GIGA_IP_ADDRESS_1					"GIGA_IP_ADDRESS_1"									// 엔티티명		: 
#define CFG_DEFAULT_GIGA_IP_ADDRESS_1				"192.168.11.101"										// 엔티티값		: 
#define CFG_ENTRY_GIGA_PORT_NUMBER_1				"GIGA_PORT_NUMBER_1"								// 엔티티명		: 
#define CFG_DEFAULT_GIGA_PORT_NUMBER_1				"1024"												// 엔티티값		: 

#define CFG_ENTRY_GIGA_IP_ADDRESS_2					"GIGA_IP_ADDRESS_2"									// 엔티티명		: 
#define CFG_DEFAULT_GIGA_IP_ADDRESS_2				"192.168.13.101"										// 엔티티값		: 
#define CFG_ENTRY_GIGA_PORT_NUMBER_2				"GIGA_PORT_NUMBER_2"								// 엔티티명		: 
#define CFG_DEFAULT_GIGA_PORT_NUMBER_2				"1025"												// 엔티티값		: 

#define CFG_ENTRY_GIGA_IP_ADDRESS_3					"GIGA_IP_ADDRESS_3"									// 엔티티명		: 
#define CFG_DEFAULT_GIGA_IP_ADDRESS_3				"192.168.15.101"										// 엔티티값		: 
#define CFG_ENTRY_GIGA_PORT_NUMBER_3				"GIGA_PORT_NUMBER_3"								// 엔티티명		: 
#define CFG_DEFAULT_GIGA_PORT_NUMBER_3				"1026"												// 엔티티값		: 

#define CFG_ENTRY_GIGA_IP_ADDRESS_4					"GIGA_IP_ADDRESS_4"									// 엔티티명		: 
#define CFG_DEFAULT_GIGA_IP_ADDRESS_4				"192.168.17.101"										// 엔티티값		: 
#define CFG_ENTRY_GIGA_PORT_NUMBER_4				"GIGA_PORT_NUMBER_4"								// 엔티티명		: 
#define CFG_DEFAULT_GIGA_PORT_NUMBER_4				"1027"												// 엔티티값		: 

#define CFG_ENTRY_GIGA_IP_ADDRESS_5					"GIGA_IP_ADDRESS_5"									// 엔티티명		: 
#define CFG_DEFAULT_GIGA_IP_ADDRESS_5				"192.168.19.101"										// 엔티티값		: 
#define CFG_ENTRY_GIGA_PORT_NUMBER_5				"GIGA_PORT_NUMBER_5"								// 엔티티명		: 
#define CFG_DEFAULT_GIGA_PORT_NUMBER_5				"1028"												// 엔티티값		: 

//////////////////////////////////////////////////////////////////////////
#define CFG_SECTION_RCV_IQ_NETWORK					"RCV_IQ_NETWORK_SETTINGS"							// 섹션			: 

#define CFG_ENTRY_LOCAL_IQ_IP_ADDRESS_1				"LOCAL_IP_ADDRESS_1"								// 엔티티명		: 
#define CFG_DEFAULT_LOCAL_IQ_IP_ADDRESS_1			"192.168.11.100"										// 엔티티값		: 

#define CFG_ENTRY_LOCAL_IQ_IP_ADDRESS_2				"LOCAL_IP_ADDRESS_2"								// 엔티티명		: 
#define CFG_DEFAULT_LOCAL_IQ_IP_ADDRESS_2			"192.168.13.100"										// 엔티티값		: 

#define CFG_ENTRY_LOCAL_IQ_IP_ADDRESS_3				"LOCAL_IP_ADDRESS_3"								// 엔티티명		: 
#define CFG_DEFAULT_LOCAL_IQ_IP_ADDRESS_3			"192.168.15.100"										// 엔티티값		: 

#define CFG_ENTRY_LOCAL_IQ_IP_ADDRESS_4				"LOCAL_IP_ADDRESS_4"								// 엔티티명		: 
#define CFG_DEFAULT_LOCAL_IQ_IP_ADDRESS_4			"192.168.17.100"										// 엔티티값		: 

#define CFG_ENTRY_LOCAL_IQ_IP_ADDRESS_5				"LOCAL_IP_ADDRESS_5"								// 엔티티명		: 
#define CFG_DEFAULT_LOCAL_IQ_IP_ADDRESS_5			"192.168.19.100"										// 엔티티값		: 

//////////////////////////////////////////////////////////////////////////
#define CFG_SECTION_KILL_PROCESS					"KILL_PROCESS_SETTINGS"								// 섹션			: 

#define CFG_ENTRY_KILL_TIME_DIFF					"KILL_TIME_DIFF"									// 엔티티명		: 
#define CFG_DEFAULT_KILL_TIME_DIFF					"10"												// 엔티티값		: 
//////////////////////////////////////////////////////////////////////////
#define CFG_SECTION_RCV_FUNC_FFT					"RCV_FUNC_FFT_SETTINGS"								// 섹션			: 

#define CFG_ENTRY_WINDOW_TYPE						"WINDOW_TYPE"										// 엔티티명		: 
#define CFG_DEFAULT_WINDOW_TYPE						"0"													// 엔티티값		: 

typedef enum {
	// (TCP)
	emWM_RCVSOCKET_ACCEPT			= WM_USER+200,
	emWM_RCVSOCKET_RECEIVE,
	emWM_RCVSOCKET_WARNING,
	emWM_RCVSOCKET_CLOSE,
	// (UDP)
	emWM_RCVSOCKET_RECEIVE_UDP,
	emWM_RCVSOCKET_WARNING_UDP,
	emWM_RCVSOCKET_CLOSE_UDP,

	emWM_RCVSOCKET_RESET_CMD,

	emWM_RCVSOCKET_RECEIVE_SCANDATA,

	emCountRCVSocketEventTag,					// 주의! 열거형의 멤버수
} enumRCVSocketEventTag;

typedef unsigned short	UINT16;

//////////////////////////////////////////////////////////////////////////
#include <winsock.h>
#pragma comment(lib,"WSOCK32.LIB")

////#define	BUFFMAX						4096				// 임시 버퍼 크기
////#define	BUFFMIN						64					// 임시 버퍼 크기
//////////////////////////////////////////////////////////////////////////

#define DEF_ON			1
#define DEF_OFF			0

#define DEF_TEST_SCPI_COM				"TEST:SCPI"

class AFX_EXT_CLASS CRcvFuncTCP : public CObject  
{
public:
	CString SCPI_CommendQuery(CString strSCPICMD = DEF_TEST_SCPI_COM);
	bool SCPI_CommendWrite(CString strSCPICMD = DEF_TEST_SCPI_COM);
	CRcvFuncTCP();
	virtual ~CRcvFuncTCP();
//////////////////////////////////////////////////////////////////////////
	void			Initialize();

	void			Finish();

	void			SystemReset(UINT32 ResetNo = 0);

	void			SetHWND(HWND hwnd);

	//////////////////////////////////////////////////////////////////////////
	// Select Receiver 
	void			SelectDevice(unsigned int LogicDev);

	UINT32			GetBaseAddress();

	BOOL			SetRcvParams(StRcvParameter *RcvParameter);

	unsigned int	GetRcvParams(StRcvParameter *RcvParameter);
	unsigned int	GetRcvParams2(StRcvParameter *RcvParameter);

	unsigned int	SetFrequency(UINT64 FreqValue);
	unsigned int	m_SetFreqCnt;
	UINT64	GetFrequency();

	unsigned int	SetBandwidth(UINT32 BWIndex);

	// BANDWIDTH(0 ~ 22), ERROR(-1)
	int				GetBandwidth();

	unsigned int	SetReceiverMode(UINT32 ModeIndex);

	UINT32			GetReceiverMode();

	// OFF(0), ON(1)
	unsigned int	SetAFCMode(int AfcMode);

	unsigned int	GetAFCMode();


	// FM(0), AM(1), RESERVED(2), RESERVED(3), RESERVED(4), RESERVED(5), CW(6), USB(7), LSB(8)
	unsigned int	SetDemodMode(UINT32 DemodIndex);

	// FM(0), AM(1), RESERVED(2), RESERVED(3), RESERVED(4), RESERVED(5), CW(6), USB(7), LSB(8), ERROR(-1) 
	int				GetDemodMode();

	// AVG(0), PEAK(1), FAST(2), RMS(3)
	unsigned int	SetLevelIndicatorType(UINT32 IndicatorType);

	int				GetLevelIndicatorType();

	// 0 ~ 100
	unsigned int	SetVolume(int value);

	UINT32			GetVolume();

	// Cal Filter(0), Operation Filter(1)
	unsigned int	SetCalFilterState(UINT32 mode);

	UINT32			GetCalFilterState();

	// AUTO(0), MANUAL(1)
	unsigned int	SetAttenMode(UINT32 AttenMode);

	unsigned int	GetAttenMode();

	// 0 ~ 40dB
	unsigned int	SetAttenValue(float AttenVal);

	float	GetAttenValue();

	// OFF(0), ON(1)
	unsigned int	SetSquelchMode(UINT32 SquelchMode);

	unsigned int	GetSquelchMode();

	//	파라미터 1개, Set, Get 가능
	//	1이면, 채널로 전달된 각 임계레벨을 적용
	//	그 외, 하나의 임계레벨 적용(OUTPut:SQUelch:THReshold 또는 SENSe:BASIc 로 설정된 것)
	BOOL			SetSquelchControl(int SquelchControl);
	unsigned int	GetSquelchControl();


	// -15 ~ 130 dBuV in 1dB increments.
	unsigned int	SetSquelchValue(int SquelchVal);

	int				GetSquelchValue();

	// AUTO(0), MANUAL(1)
	BOOL			SetGainControlMode(int GainMode);

	unsigned int	GetGainControlMode();

	// -15 ~ 130 dBuV in 1dB increments.
	unsigned int	SetGainControlValue(int GainValue);

	int				GetGainControlValue();

	// ?
	unsigned int	SetTemperatureAtten(float fAttenValue);

	float			GetTemperatureAtten();

	// NORMAL(0), LOW NOISE(1), LOW DISTORTION(2)
	unsigned int	SetPreSelectMode(UINT32 PreSelectValue);

	int				GetPreSelectMode();

	// Master(0), Slave_1(1), Slave_2(2), Slave_3(3), Slave_4(4)
	BOOL			SyncModeSelect(UINT32 SYNC_Mode);

	// Path : FPDP(0), VXI(1), FPDP_VXI(2)
	unsigned int	SetIQDataPath(UINT32 Path);

	// RESERVED(0), RESERVED(1), RESERVED(2), BIT32_FLAG(3), RESERVED(4)
	unsigned int	SetIQDataFormat(UINT32 Mode);

	UINT32			GetIQDataFormat();

	// FPDP Path IQ DATA Output Disable
	unsigned int	SetSyncReadyFPDP(UINT32 Mode);

	unsigned int	SetSyncStartFPDP(UINT32 Mode);

	unsigned int	SetSyncReadyStartFPDP();

	// VXI Path IQ DATA Output Disable
	unsigned int	SetSyncReadyVXI(UINT32 Mode);

	// VXI Path IQ DATA Output Enable
	unsigned int	SetSyncStartVXI(UINT32 Mode);

	// mode : OFF(0), ON(1)
	bool			SetBiteMode(UINT uiMode, unsigned int uiFrequency = 0);
	UINT			GetBiteMode();

	UINT32			GetDeviceStatus(char *Status);
	UINT32			GetDeviceStatus(char *Status, unsigned int uiFrequency);
	UINT32			SetDeviceStatus(unsigned int uiFrequency);
	// ClearChannelMemory(0, 0, 4999)
	BOOL			ClearChannelMemory(UINT32 iFuncCode, UINT32 iChNumSrc, UINT32 iChNumDest);

	BOOL			SetMemoryTable(StMemoryTable *stMemTable);


	BOOL			SetScanDataLength(UINT32 iScanDataLength);
	UINT32			GetScanDataLength();

	BOOL			SetScanDwellIlligal(BOOL bIlligal);
	BOOL			GetScanDwellIlligal();


	// Frequency [Start, Stop, Step] (Hz)
	BOOL			SetFScanParams(UINT32 StartFreq, UINT32 StopFreq, UINT32 StepFreq);

	BOOL			SetFScanDwellTime(UINT32 iDwellTime, UINT32 iDwellMode);
	UINT32			GetFScanDwellTime();

	BOOL			SetFScanCycleCount(UINT32 iCycleCount, UINT32 iCycleMode);
	UINT32			GetFScanCycleCount();

	BOOL			SetMScanDwellTime(UINT32 iDwellTime, UINT32 iDwellMode);

	BOOL			SetMScanCycleCount(UINT32 iCycleCount, UINT32 iCycleMode);
	UINT32			GetMScanCycleCount();

	BOOL			GetRcvLevel_dBm(float *pfLevel);

	BOOL			GetRcvLevel_dBuV(float *pfLevel);

	void			ReportFSCAN(HWND hwnd, DWORD EventID, unsigned int DevID);

	CString			GetPSCanPARAmeter();
	UINT32			SetPSCanPARAmeter(CString strPSCanPARAmeter);

	CString			GetPSCanCOUNt();
	UINT32			SetPSCanCOUNt(CString strPSCanCOUNt);

	BOOL			StartFScan();

	BOOL			StopFScan();

	//////////////////////////////////////////////////////////////////////////
	unsigned int	SetAGCAtten1(double AGCAttenValue);
	double			GetAGCAtten1();
	unsigned int	SetAGCAtten2(double AGCAttenValue);
	double			GetAGCAtten2();

	unsigned int	SetANTControlMode(int iAntMode);
	int				GetANTControlMode();

	unsigned int	SetChannelControl(int iChannel);
	int				GetChannelControl();

	//////////////////////////////////////////////////////////////////////////	

	BOOL 			VxiReadMemory(UINT32 Addr, UINT16 DataWidth, UINT32 * pBuffer);

	UINT32			VxiReadMemory(UINT32 Addr, UINT16 DataWidth);
	
	BOOL			VxiWriteMemory(UINT32 Addr, UINT16 DataWidth, UINT32 DataValue);
	
	BOOL			VxiReadBlock(UINT32 srcAddr, void * pBuffer, UINT32 Length, UINT16 DataWidth);

	//////////////////////////////////////////////////////////////////////////
	// jhmoon [2010/2/19] 추가

	// Signal Path: 0(Operation) 1(Calibration)
	unsigned int	SetSignalPath(int PathIndex);

	UINT32			GetSignalPath();

	// Audio Select
	BOOL			SetAudioSelect(int AudioMode);

	unsigned int	GetAudioSelect();

	// Audio Balance
	unsigned int	SetAudioBalance(UINT32 AudioBalance);

	unsigned int	GetAudioBalance();

	// Boaed Offset
	unsigned int	SetBoardOffset(int OffsetNormal, int OffsetLNoise, int OffsetLDistortion, int OffsetHF);

	unsigned int	GetBoardOffset(int &m_uBDOffsetNOR, int &m_uBDOffsetLN, int &m_uBDOffsetLD, int &m_uBDOffsetHF);

	// Boaed Offset - 3
	unsigned int	SetBoardOffset(int OffsetNormal, int OffsetLNoise, int OffsetLDistortion);

	unsigned int	GetBoardOffset(int &m_uBDOffsetNOR, int &m_uBDOffsetLN, int &m_uBDOffsetLD);

	unsigned int	GetLogDet(int &IFLog100, int &RFLog100);

	// Gain Adjust ATT
	unsigned int	SetGainAdjustAtt(UINT32 GainAdjustAtt);

	unsigned int	GetGainAdjustAtt();

	//SetPRSBMode
	//	0 : TLM, 1 : FDM, 2 : DFMU, 3 : DFMD
	unsigned int	SetPRSBMode(UINT32 PRSBMode);
	int				GetPRSBMode();

	unsigned int	SetCalibrationInfo(int iType = 10);

	//ModuleSelect
	unsigned int	SetModuleSelect(UINT32 ModuleSelect);
	int				GetModuleSelect();


	// RX_IFATT_SELECT
	unsigned int	SetIFAttenMode(UINT32 AttenMode);
	
	unsigned int	GetIFAttenMode();

	// RX_IFATT_VALUE
	unsigned int	SetIFAttenValue(float AttenVal);

	float	GetIFAttenValue();

	// SYS ATT_VALUE
	unsigned int	SetSYSAttenValue(float AttenVal);

	float	GetSYSAttenValue();

	unsigned int	SetIFAttenDefaultNormal(float fValue);
	float			GetIFAttenDefaultNormal();
	unsigned int	SetIFAttenDefaultLowNoise(float fValue);
	float			GetIFAttenDefaultLowNoise();

	// [8/8/2013 Josuk]
	unsigned int	SetIFAttenDefaultNormal(UINT32 uValue);
	unsigned int	SetIFAttenDefaultLowNoise(UINT32 uValue);


	//수신기 기본 상태로 복귀
	unsigned int	SetDefaultStatus();

	//
	unsigned int	SetRFInputAUTO(int RFAutoManual);
	unsigned int	GetRFInputAUTO();

	unsigned int	SetOutChannelSelect(int Chindex);
	unsigned int	GetOutChannelSelect();

	// RF_INPUT_PATH
	unsigned int	SetRFInputPath(int RFInputPath);

	unsigned int	GetRFInputPath();


	//////////////////////////////////////////////////////////////////////////
	//
	unsigned int	GetHFAmpPathMode();
	BOOL			SetHFAmpPathMode(UINT32 HFAmpPathMode);

	//
	unsigned int	GetHFAmpPath();
	BOOL			SetHFAmpPath(UINT32 HFAmpPath);

	//
	unsigned int	GetHFIFPath();
	BOOL			SetHFIFPath(UINT32 HFIFPath);

	//
	unsigned int	GetHFRFAttenMode();
	BOOL			SetHFRFAttenMode(UINT32 HFRFAttenMode);

	//
	float			GetHFRFAtten();
	BOOL			SetHFRFAtten(float HFRFAtten);
	BOOL			SetHFRFAtten(UINT32 HFRFAtten);

	//////////////////////////////////////////////////////////////////////////
	//
	unsigned int	GetHFFilterCtrlMode();
	BOOL			SetHFFilterCtrlMode(UINT32 HFFilterCtrlMode);

	//
	unsigned int	GetHFHPFVal();
	BOOL			SetHFHPFVal(UINT32 HFHPFVal);

	//
	unsigned int	GetHFLPFVal();
	BOOL			SetHFLPFVal(UINT32 HFLPFVal);

	unsigned int	GetHFBPAVal();
	BOOL			SetHFBPAVal(UINT32 HFLPFVal);

	//////////////////////////////////////////////////////////////////////////
	unsigned int	GetPowerAvgCnt();
	BOOL			SetPowerAvgCnt(UINT32 PowerAvgCnt);

	unsigned int		GetLanDataMode();
	BOOL			SetLanDataMode(UINT32 iLanDataMode);
	unsigned int	GetLanDataNFFT();
	BOOL			SetLanDataNFFT(UINT32 iLanDataNFFT);
	CString			GetDEMInfo();
	BOOL			SetResetFIFO(UINT32 iResetFIFO);

	unsigned int	GetFFTWindowType();
	BOOL			SetFFTWindowType(UINT32 iFFTWindowType);

	unsigned int	GetFrequencyOffset();

	BOOL			SetJigIOTest(CString strMessage);

	CString			GetBoardID();
	BOOL			SetBoardID(CString strBoardID);
	//////////////////////////////////////////////////////////////////////////
	// 추가 2012 02 14
	unsigned int GetDEM_BFO();
	BOOL SetDEM_BFO(UINT32 iDEM_BFO);

	unsigned int GetHITCNT_VUHF(UINT32 &iResult1, UINT32 &iResult2);
	BOOL SetHITCNT_VUHF(UINT32 iHITCNT_VUHF1, UINT32 iHITCNT_VUHF2);

	unsigned int GetHITCNT_HF(UINT32 &iResult1, UINT32 &iResult2);
	BOOL SetHITCNT_HF(UINT32 iHITCNT_HF1, UINT32 iHITCNT_HF2);

	unsigned int GetTHRESHOLD_VUHF(UINT32 &iResult1, UINT32 &iResult2);
	BOOL SetTHRESHOLD_VUHF(UINT32 iTHRESHOLD_VUHF1, UINT32 iTHRESHOLD_VUHF2);

	unsigned int GetTHRESHOLD_HF(UINT32 &iResult1, UINT32 &iResult2);
	BOOL SetTHRESHOLD_HF(UINT32 iTHRESHOLD_HF1, UINT32 iTHRESHOLD_HF2);

	unsigned int GetTEST_CMD1();
	BOOL SetTEST_CMD1(UINT32 iTEST_CMD1);
	//////////////////////////////////////////////////////////////////////////
	unsigned int GetTEST_RESULT();


	bool			GetVxiInitialized();

	BOOL			SetPort(int iPort);
	int				GetPort();

	BOOL			SetIPAddress(CString strIP);
	CString			GetIPAddress();

	BOOL			SetPort_REAL(int iPort);
	int				GetPort_REAL();
	
	BOOL			SetIPAddress_REAL(CString strIP);
	CString			GetIPAddress_REAL();

	BOOL			SetGIGAPort(int iPort);
	int				GetGIGAPort();

	
	BOOL			SetGIGAIPAddress(CString strIP);
	CString			GetGIGAIPAddress();

	BOOL			SetGIGATargetIPAddress(CString strIP);
	CString			GetGIGATargetIPAddress();

	BOOL			SetGIGAPort_REAL(int iPort);
	int				GetGIGAPort_REAL();
	
	BOOL			SetGIGAIPAddress_REAL(CString strIP);
	CString			GetGIGAIPAddress_REAL();

	//////////////////////////////////////////////////////////////////////////
	// NHFTS
	unsigned int 	SetPacketInitiate(UINT32 iStatus);
	int				GetPacketInitiate();
	//////////////////////////////////////////////////////////////////////////

	BOOL			SetSCPICommand(CString strSCPICommand);
	CString			GetSCPICommand(CString strSCPICommand);

	BOOL			SetStartSCAN();

	SOCKET			GetSocket()
	{
		return m_sSocket;
	}


	CString			m_strModulePath;
	CString			m_strCommand;
	CString			m_strReceive;
	SOCKET			m_sSocket;
	int				m_bStartSCAN;

	int				m_iPort;
	CString			m_strIP;

	int				m_iPortGIGA;
	CString			m_strIPGIGA;

	int				m_iPort_TEMP;
	CString			m_strIP_TEMP;
	
	int				m_iPortGIGA_TEMP;
	CString			m_strIPGIGA_TEMP;

	BOOL			m_Socket_Fail;
	CString			m_strResult;
	BOOL			m_bGetIQ_Stream;

//////////////////////////////////////////////////////////////////////////
//Socket
	//-----------------------------------------------------------------------------
	// 소켓이 '연결'상태이면 강제 연결 종료
	//-----------------------------------------------------------------------------

	HWND				m_hWnd;
	int					m_nSocketID;

	CMySocket*			m_pTCPClientSock;

	CSocketFunc			m_SocketFunc;
	CSockClient*		m_pTcpClientSock_DF;

	BOOL				m_bReceiveWait;
	long				m_nKillDiff;

//////////////////////////////////////////////////////////////////////////
	// 추가 2010-07-19
	unsigned int	GetMScanHoldTime();
	BOOL			SetMScanHoldTime(UINT32 iHoldTimeMscan, UINT32 iHoldTimeMode);
	unsigned int	GetFScanHoldTime();
	BOOL			SetFScanHoldTime(UINT32 iHoldTimeMscan, UINT32 iHoldTimeMode);

	void			ProfileSave();
	void			ProfileLoad();

	BOOL			vxi_read_block(UINT32 srcAddr, UINT32 dstAddr, UINT32 Length, UINT32 DataWidth){return FALSE;};
	BOOL			vxi_read_memory(UINT32 Addr, UINT32 DataWidth, UINT32 *DataValue){return FALSE;};
	BOOL			vxi_write_memory(UINT32 Addr, UINT32 DataWidth, UINT32 DataValue){return FALSE;};

//////////////////////////////////////////////////////////////////////////

};

#endif // !defined(AFX_RCVFUNCTCP_H__EB35F31F_0D4B_4A0A_8DEA_15CA350A5CB3__INCLUDED_)
