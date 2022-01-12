#include  <afxwin.h>
//-----------------------------/
#ifndef __DEF_STRUCT__
#define __DEF_STRUCT__
//------------------------------/
typedef struct {
	UINT32	TxRx_StatusReg;
	UINT32	DestAddr;
	UINT32	SourceAddr;
	UINT32	OBJECT;
	UINT32	Comm_Act;
	UINT32	Status;
	UINT32	Header_End;
	UINT32	Body_length;
	UINT32	Version;
	UINT32	FpdpFpga;
	UINT32	CpuFpga;
	UINT32	CpuApp;
	UINT32	DspFpga;
	UINT32	DspApp;
} VERSION_PACKET;
//---------------------------------/
/***
typedef struct {
	UINT32  TxRx_StatusReg;
	UINT32  DestAddr;
	UINT32  SourceAddr;
	UINT32  OBJECT;
	UINT32  Comm_Act;
	UINT32  Status;
	UINT32  Header_End;
	UINT32  Body_length;
	UINT32  Command;
} CONNECTION_PACKET; 
***/ 
//---------------------------------/
/***
typedef struct {
	UINT32  TxRx_StatusReg;
	UINT32  DestAddr;
	UINT32  SourceAddr;
	UINT32  OBJECT;
	UINT32  Comm_Act;
	UINT32  Status;
	UINT32  Header_End;
	UINT32  Body_length;
	UINT32  Command;
} CLOSE_PACKET;
***/
//---------------------------------/
typedef struct {
	DWORD	TxRx_StatusReg;
	DWORD	DestAddr;
	DWORD	SourceAddr;
	DWORD	OBJECT;
	DWORD	Comm_Act;
	DWORD	Status;
	DWORD	Header_End;
	DWORD	Body_length;
	UINT32	Command;
} COMMAND_PACKET;
typedef struct {
	DWORD	TxRx_StatusReg;
	DWORD	DestAddr;
	DWORD	SourceAddr;
	DWORD	OBJECT;
	DWORD	Comm_Act;
	DWORD	Status;
	DWORD	Header_End;
	DWORD	Body_length;
	UINT32	Command_1;
	UINT32	Command_2;
	UINT32	Command_3;
} THREE_COMMAND_PACKET;
//-------------------------------/
typedef struct
{
    DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
    UINT32 Command_1;
	UINT32 Command_2;
} TWO_COMMAND_PACKET;
//-------------------------------/
/***
typedef struct {
    DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
    double Command;
} DCOMMAND_PACKET;
***/
//--------------------------------/
/***
typedef struct {
	UINT32	TxRx_StatusReg;
	UINT32	DestAddr;
	UINT32	SourceAddr;
	UINT32	OBJECT;
	UINT32	Comm_Act;
	UINT32	Status;
	UINT32	Header_End;
	UINT32	Body_length;
    UINT32	Command;
} GENERAL_PACKET;
***/
//-------------------------------/
typedef struct {
	DWORD	ScanMode;
	DWORD	Frequency;
	DWORD	Demodulation_Mode;
	DWORD	Bandwidth;
	DWORD	Squelch_Control;
	INT32	Squelch_Threshold;
	DWORD	AFC_Control;
	DWORD	RF_ATT_Mode;
	DWORD	RF_ATT_Value;	
	DWORD	PreSelect_Mode;
	DWORD	GainControl_Mode;
	DWORD	GainControl_Value;
	DWORD	LevelIndicator_Mode;
	DWORD	Volume;
} BASIC_PARAM;
//--------------------------------/
typedef struct {
	DWORD	TxRx_StatusReg;
	DWORD	DestAddr;
	DWORD	SourceAddr;
	DWORD	OBJECT;
	DWORD	Comm_Act;
	DWORD	Status;
	DWORD	Header_End;
	DWORD	Body_length;
	DWORD	StartFreq;
	DWORD	StopFreq;
	DWORD	StepFreq;
} FSCAN_FREQ_PACKET;
//---------------------------------/
typedef struct {
	DWORD	StartFreq;
	DWORD	StopFreq;
	DWORD	StepFreq;
} FSCAN_FREQ_PARAM;
//--------------------------------/
typedef struct {
	DWORD	nFscanFrequencyStart;
	DWORD	nFscanFrequencyStop;
	DWORD	nFscanFrequencyStep;
}StFScanParameter;
//--------------------------------/
typedef struct {
	DWORD	ChannelNumber;
	DWORD	ChannelContent;
	DWORD	ChannelState;
	DWORD	FreqValue;
	DWORD	DemodSelect;
	DWORD	BandwidthValue;
	DWORD	SquelchState;
	DWORD	SquelchValue;
	DWORD	AntenaNumber;
	DWORD	AFC_State;
	DWORD	Atten_Select;
	DWORD	Atten_Value;
} MEM_MSCAN_PARAM;
//---------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
	DWORD FunctionCode;
	DWORD ChannelStart;
	DWORD ChannelStop;
} CLEAR_MEM_PACKET;

//---------------------------------/
typedef struct
{
	DWORD FunctionCode;
	DWORD ChannelStart;
	DWORD ChannelStop;
} CLEAR_MEM_PARAM;
//---------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
	DWORD ModuleIndex;
	DWORD Count;
	DWORD Valid;
	DWORD MinLimit;
	DWORD MaxLimit;
	DWORD Value;
	DWORD ModuleName;
	DWORD pcName;
} TESTPOINT_PACKET;
//-------------------------------------/
typedef struct
{
	DWORD ModuleIndex;
	DWORD Count;
	DWORD Valid;
	DWORD MinLimit;
	DWORD MaxLimit;
	DWORD Value;
	DWORD ModuleName;
	DWORD pcName;
}TESTPOINT_PARAM;
//--------------------------------------/
typedef struct
{
	DWORD ModuleIndex;
	DWORD Count;
	DWORD Valid;
	DWORD MinLimit;
	DWORD MaxLimit;
	DWORD Value;
	DWORD ModuleName;
	DWORD pcName;
}StTestPoint;
//-------------------------------------/
typedef struct
{
	DWORD eReceiverModeSelect;
	DWORD nFreqReceiverVal;
	DWORD eDemodSelect;
	DWORD nlfBandwidthVal;
	DWORD bSquelchState;
	INT32 nThresholdVal;
	DWORD bAfcState;
	DWORD bAttenuationManual;
	DWORD nAttenuationVal;
	DWORD eRfMode;            //PRSB_Mode
	DWORD eGainControlSelect;
	INT32 nGainVal;
	DWORD eLevelIndicatorSelect;
	DWORD nVolumeVal;
} StRcvParameter;
//-----------------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;	
	StRcvParameter ReceiverParam;
	////BASIC_PARAM ReceiverParam;
} BASIC_PACKET;
//---------------------------------/
typedef struct {
	UINT32	TxRx_StatusReg;
	UINT32	DestAddr;
	UINT32	SourceAddr;
	UINT32	OBJECT;
	UINT32	Comm_Act;
	UINT32	Status;
	UINT32	Header_End;
	UINT32	Body_length;
	UINT32	ScanMode;
	UINT32	Frequency;		
	UINT32	Demodulation_Mode;	
	UINT32	Bandwidth;
	UINT32	Squelch_Control;
	INT32	Squelch_Threshold;
	UINT32	AFC_Control;
	UINT32	RF_ATT_Mode;
	UINT32	RF_ATT_Value;
	UINT32	PreSelect_Mode;		//PRSB Mode
	UINT32	GainControl_Mode;	//gain ctrl
	UINT32	GainControl_Value;	//gain val
	UINT32	LevelIndicator;		//level mode
	UINT32	AudioVolume;		//volum value	
	UINT32	Command;	
} BASIC_PARAMETER;
//--------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 ScanMode;
	UINT32 Frequency;		
	UINT32 Demodulation_Mode;	
	UINT32 Bandwidth;
	UINT32 Squelch_Control;
	INT32  Squelch_Threshold;
	UINT32 AFC_Control;
	UINT32 RF_ATT_Mode;
	UINT32 RF_ATT_Value;
    UINT32 PreSelect_Mode;		//PRSB Mode
	UINT32 GainControl_Mode;	//gain ctrl
	UINT32 GainControl_Value;	//gain val
	UINT32 LevelIndicator;		//level mode
	UINT32 AudioVolume;			//volum value	
	UINT32 Command;
} RES_BASIC_PARAMETER;
//-------------------------------------------/
typedef struct
{
	DWORD nChannelNumberDestination;
	DWORD bChannelContent;
	DWORD bChannelState;
	DWORD nFreqReceiverVal;
	DWORD eDemodSelect;
	DWORD nlfBandwidthVal;
	DWORD bSquelchState;
	INT32 nThresholdVal;
	DWORD nAntennaNumber;
	DWORD bAfcState;
	DWORD bAttenuationManual;
	INT32 bAttenuationVal;
	INT32 ChannelPower;
} StMemoryTable;
//------------------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
	StMemoryTable MemTable;  
} MEM_MSCAN_PACKET;
//----------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
    UINT32 ChannelIndex;
	UINT32 FreqValue;
	UINT32 LevelValue;
} SCAN_RESULT_PACKET;
//-------------------------------/
// happyim 2009.03.02 MSCAN
typedef struct
{
	UINT32 fFlag;
    UINT32 ChannelIndex;
	UINT32 FreqValue;
	UINT32 LevelValue;
} MSCAN_RESULT_PACKET;
//-------------------------------/
// HAPPYIM [2009/11/4] 복조수신기 폴링방식[블럭단위]
typedef struct	
{
	UINT32 fFlag;
	UINT32 BlockEndFlag;		// 스캔완료 유/무
    UINT32 ChannelIndex;		// 채널시작인덱스
	UINT32 ChannelCount;		// 유효채널갯수
	UINT32 LevelValue[100];		// 최대블럭갯수[100]
} MSCAN_RESULT_PACKET_BLOCK;
//-------------------------------/
typedef struct 
{
	DWORD Index;
	DWORD StartFreq;
	DWORD StopFreq;
} StSuppressFreq;
//-------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
	StSuppressFreq stSPFreq;
} SUPPRESS_FREQ_PACKET;
//--------------------------------/
typedef struct {
	UINT32	TxRx_StatusReg;
	UINT32	DestAddr;
	UINT32	SourceAddr;
	UINT32	OBJECT;
	UINT32	Comm_Act;
	UINT32	Status;
	UINT32	Header_End;
	UINT32	Body_length;
	UINT32	ScanChannelNumber;
	UINT32	ScanFrequency;
    UINT32	Command;
} CURRENT_MSCAN_PACKET;
//--------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;	
	UINT32 ScanStatus;	
	UINT32 ScanFrequency;	
    UINT32 Command;
} CURRENT_FSCAN_PACKET;
//------------------------------/
/***
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	INT32 Command;
} RES_GENERAL_PACKET;
***/
//--------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 Level;
	UINT32 FreqOffset;
	UINT32 FreqDeviation;
	UINT32 Mod_Depth;
	UINT32 Command;
} RES_MEASURE_OPERATION_MODE_PACKET;
//--------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 Start_Frequency;
	UINT32 Stop_Frequency;
	UINT32 Step_Frequency;
	UINT32 Command;
} FSCAN_PACKET;
//-------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 Start_Frequency;
	UINT32 Stop_Frequency;
	UINT32 Step_Frequency;
	UINT32 Command;
} RES_FSCAN_PACKET;
//-------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 ChannelNumber;
	UINT32 ChannelContent;
	UINT32 ChannelState;
	UINT32 Frequency;
	UINT32 DemodulationMode;
	UINT32 Bandwidth;
	UINT32 Squelch;
	UINT32 SquelchThreshold;
	UINT32 AntenaNumber;
    UINT32 AFC_Control;
	//UINT32 GainControl_Mode;
	//UINT32 GainControl_Value;
	//UINT32 PreSelect_Mode;
	UINT32 RF_ATT_Mode;
	UINT32 RF_ATT_Value;
	UINT32 ChannelPower;
	UINT32 Command;
} MSCAN_PACKET;
//-------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 ChannelNumber;
	UINT32 Frequency;
	UINT32 Bandwidth;	
	UINT32 DemodulationMode;
	UINT32 AudioControl;
	UINT32 Squelch;
	UINT32 SquelchThreshold;
    UINT32 AFC_Control;
	UINT32 GainControl_Mode;
	UINT32 GainControl_Value;
	UINT32 PreSelect_Mode;
	UINT32 RF_ATT_Mode;
	UINT32 RF_ATT_Value;
	UINT32 Command;
} RES_MSCAN_PACKET;
//-------------------------------/

//////////////////////////////////////////////////////////////////////////
/**
//-------------------------------/
typedef struct {
	DWORD	ScanMode;
	DWORD	Frequency;
	DWORD	Demodulation_Mode;
	DWORD	Bandwidth;
	DWORD	Squelch_Control;
	INT32	Squelch_Threshold;
	DWORD	AFC_Control;
	DWORD	RF_ATT_Mode;
	DWORD	RF_ATT_Value;	
	DWORD	PreSelect_Mode;
	DWORD	GainControl_Mode;
	DWORD	GainControl_Value;
	DWORD	LevelIndicator_Mode;
	DWORD	Volume;
} BASIC_PARAM_RCV;
//--------------------------------/
typedef struct {
	DWORD	TxRx_StatusReg;
	DWORD	DestAddr;
	DWORD	SourceAddr;
	DWORD	OBJECT;
	DWORD	Comm_Act;
	DWORD	Status;
	DWORD	Header_End;
	DWORD	Body_length;
	DWORD	StartFreq;
	DWORD	StopFreq;
	DWORD	StepFreq;
} FSCAN_FREQ_PACKET_RCV;
//---------------------------------/
typedef struct {
	DWORD	StartFreq;
	DWORD	StopFreq;
	DWORD	StepFreq;
} FSCAN_FREQ_PARAM_RCV;
//--------------------------------/
typedef struct {
	DWORD	nFscanFrequencyStart;
	DWORD	nFscanFrequencyStop;
	DWORD	nFscanFrequencyStep;
}StFScanParameter_RCV;
//--------------------------------/
typedef struct {
	DWORD	ChannelNumber;
	DWORD	ChannelContent;
	DWORD	ChannelState;
	DWORD	FreqValue;
	DWORD	DemodSelect;
	DWORD	BandwidthValue;
	DWORD	SquelchState;
	DWORD	SquelchValue;
	DWORD	AntenaNumber;
	DWORD	AFC_State;
	DWORD	Atten_Select;
	DWORD	Atten_Value;
} MEM_MSCAN_PARAM_RCV;
//---------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
	DWORD FunctionCode;
	DWORD ChannelStart;
	DWORD ChannelStop;
} CLEAR_MEM_PACKET_RCV;

//---------------------------------/
typedef struct
{
	DWORD FunctionCode;
	DWORD ChannelStart;
	DWORD ChannelStop;
} CLEAR_MEM_PARAM_RCV;
//---------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
	DWORD ModuleIndex;
	DWORD Count;
	DWORD Valid;
	DWORD MinLimit;
	DWORD MaxLimit;
	DWORD Value;
	DWORD ModuleName;
	DWORD pcName;
} TESTPOINT_PACKET_RCV;
//-------------------------------------/
typedef struct
{
	DWORD ModuleIndex;
	DWORD Count;
	DWORD Valid;
	DWORD MinLimit;
	DWORD MaxLimit;
	DWORD Value;
	DWORD ModuleName;
	DWORD pcName;
}TESTPOINT_PARAM_RCV;
//--------------------------------------/
typedef struct
{
	DWORD ModuleIndex;
	DWORD Count;
	DWORD Valid;
	DWORD MinLimit;
	DWORD MaxLimit;
	DWORD Value;
	DWORD ModuleName;
	DWORD pcName;
}StTestPoint_RCV;
//-------------------------------------/

//-----------------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;	
	StRcvParameter ReceiverParam;
	////BASIC_PARAM ReceiverParam;
} BASIC_PACKET_RCV;
//---------------------------------/
typedef struct {
	UINT32	TxRx_StatusReg;
	UINT32	DestAddr;
	UINT32	SourceAddr;
	UINT32	OBJECT;
	UINT32	Comm_Act;
	UINT32	Status;
	UINT32	Header_End;
	UINT32	Body_length;
	UINT32	ScanMode;
	UINT32	Frequency;		
	UINT32	Demodulation_Mode;	
	UINT32	Bandwidth;
	UINT32	Squelch_Control;
	INT32	Squelch_Threshold;
	UINT32	AFC_Control;
	UINT32	RF_ATT_Mode;
	UINT32	RF_ATT_Value;
	UINT32	PreSelect_Mode;		//PRSB Mode
	UINT32	GainControl_Mode;	//gain ctrl
	UINT32	GainControl_Value;	//gain val
	UINT32	LevelIndicator;		//level mode
	UINT32	AudioVolume;		//volum value	
	UINT32	Command;	
} BASIC_PARAMETER_RCV;
//--------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 ScanMode;
	UINT32 Frequency;		
	UINT32 Demodulation_Mode;	
	UINT32 Bandwidth;
	UINT32 Squelch_Control;
	INT32  Squelch_Threshold;
	UINT32 AFC_Control;
	UINT32 RF_ATT_Mode;
	UINT32 RF_ATT_Value;
    UINT32 PreSelect_Mode;		//PRSB Mode
	UINT32 GainControl_Mode;	//gain ctrl
	UINT32 GainControl_Value;	//gain val
	UINT32 LevelIndicator;		//level mode
	UINT32 AudioVolume;			//volum value	
	UINT32 Command;
} RES_BASIC_PARAMETER_RCV;
//-------------------------------------------/
typedef struct
{
	DWORD nChannelNumberDestination;
	DWORD bChannelContent;
	DWORD bChannelState;
	DWORD nFreqReceiverVal;
	DWORD eDemodSelect;
	DWORD nlfBandwidthVal;
	DWORD bSquelchState;
	INT32 nThresholdVal;
	DWORD nAntennaNumber;
	DWORD bAfcState;
	DWORD bAttenuationManual;
	INT32 bAttenuationVal;
	INT32 ChannelPower;
} StMemoryTable_RCV;
//------------------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
	StMemoryTable_RCV MemTable;  
} MEM_MSCAN_PACKET_RCV;
//----------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
    UINT32 ChannelIndex;
	UINT32 FreqValue;
	UINT32 LevelValue;
} SCAN_RESULT_PACKET_RCV;
//-------------------------------/
// happyim 2009.03.02 MSCAN
typedef struct
{
	UINT32 fFlag;
    UINT32 ChannelIndex;
	UINT32 FreqValue;
	UINT32 LevelValue;
} MSCAN_RESULT_PACKET_RCV;
//-------------------------------/
// HAPPYIM [2009/11/4] 복조수신기 폴링방식[블럭단위]
typedef struct	
{
	UINT32 fFlag;
	UINT32 BlockEndFlag;		// 스캔완료 유/무
    UINT32 ChannelIndex;		// 채널시작인덱스
	UINT32 ChannelCount;		// 유효채널갯수
	UINT32 LevelValue[100];		// 최대블럭갯수[100]
} MSCAN_RESULT_PACKET_BLOCK_RCV;
//-------------------------------/
typedef struct 
{
	DWORD Index;
	DWORD StartFreq;
	DWORD StopFreq;
} StSuppressFreq_RCV;
//-------------------------------/
typedef struct
{
	DWORD TxRx_StatusReg;
	DWORD DestAddr;
	DWORD SourceAddr;
	DWORD OBJECT;
	DWORD Comm_Act;
	DWORD Status;
	DWORD Header_End;
	DWORD Body_length;
	StSuppressFreq_RCV stSPFreq;
} SUPPRESS_FREQ_PACKET_RCV;
//--------------------------------/
typedef struct {
	UINT32	TxRx_StatusReg;
	UINT32	DestAddr;
	UINT32	SourceAddr;
	UINT32	OBJECT;
	UINT32	Comm_Act;
	UINT32	Status;
	UINT32	Header_End;
	UINT32	Body_length;
	UINT32	ScanChannelNumber;
	UINT32	ScanFrequency;
    UINT32	Command;
} CURRENT_MSCAN_PACKET_RCV;
//--------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;	
	UINT32 ScanStatus;	
	UINT32 ScanFrequency;	
    UINT32 Command;
} CURRENT_FSCAN_PACKET_RCV;
//------------------------------/
/***
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	INT32 Command;
} RES_GENERAL_PACKET;
***/
//--------------------------------/
/*
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 Level;
	UINT32 FreqOffset;
	UINT32 FreqDeviation;
	UINT32 Mod_Depth;
	UINT32 Command;
} RES_MEASURE_OPERATION_MODE_PACKET_RCV;
//--------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 Start_Frequency;
	UINT32 Stop_Frequency;
	UINT32 Step_Frequency;
	UINT32 Command;
} FSCAN_PACKET_RCV;
//-------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 Start_Frequency;
	UINT32 Stop_Frequency;
	UINT32 Step_Frequency;
	UINT32 Command;
} RES_FSCAN_PACKET_RCV;
//-------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 ChannelNumber;
	UINT32 ChannelContent;
	UINT32 ChannelState;
	UINT32 Frequency;
	UINT32 DemodulationMode;
	UINT32 Bandwidth;
	UINT32 Squelch;
	UINT32 SquelchThreshold;
	UINT32 AntenaNumber;
    UINT32 AFC_Control;
	//UINT32 GainControl_Mode;
	//UINT32 GainControl_Value;
	//UINT32 PreSelect_Mode;
	UINT32 RF_ATT_Mode;
	UINT32 RF_ATT_Value;
	UINT32 ChannelPower;
	UINT32 Command;
} MSCAN_PACKET_RCV;
//-------------------------------/
typedef struct
{
	UINT32 TxRx_StatusReg;
	UINT32 DestAddr;
	UINT32 SourceAddr;
	UINT32 OBJECT;
	UINT32 Comm_Act;
	UINT32 Status;
	UINT32 Header_End;
	UINT32 Body_length;
	UINT32 ChannelNumber;
	UINT32 Frequency;
	UINT32 Bandwidth;	
	UINT32 DemodulationMode;
	UINT32 AudioControl;
	UINT32 Squelch;
	UINT32 SquelchThreshold;
    UINT32 AFC_Control;
	UINT32 GainControl_Mode;
	UINT32 GainControl_Value;
	UINT32 PreSelect_Mode;
	UINT32 RF_ATT_Mode;
	UINT32 RF_ATT_Value;
	UINT32 Command;
} RES_MSCAN_PACKET_RCV;
//-------------------------------/
/**/

#endif