
// DF_Incheon2019Dlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "math.h"
#include "DFData.h"
/*
#define COMINT_MAX_CHANNEL_COUNT	5			// COMIT 방탐 최대 채널 수

//LMS_ADD_20171221
#define BAND1_START_FREQ				20		// 저대역 시작 주파수 설정 //LMS_ADD_20170209
#define BAND1_STOP_FREQ					500		// 저대역 종료 주파수 설정 //LMS_ADD_20170209
#define BAND2_STOP_FREQ					3000	// 고대역 종료 주파수 설정 //LMS_ADD_20170209
#define APPLY_AZIMUTH_OFFSET_WHEN_CREATING_IDEAL_RADICAL //방사보정데이터 생성 시 방위각 오프셋을 반영할때 설정한다.
#define FREQ_RANGE_START_MHz            BAND1_START_FREQ  //LMS_ADD_20171222
#define FREQ_RANGE_END_MHz              8000              //LMS_ADD_20171222   
#define DF_CH_NUM                       9                 //LMS_ADD_20171222    
#define DF_AZIMUTH_NUM                  360               //LMS_ADD_20171222    
struct _stBigArray
{
	USHORT	m_usPhDfRomData[FREQ_RANGE_END_MHz-FREQ_RANGE_START_MHz+1][DF_AZIMUTH_NUM][DF_CH_NUM];     //m_usPhDfRomData[9][360][2501]; // 방사보정용 위상 ROM 데이터[채널][방위][주파수] : 방위 0~360, 1도 Step, 주파수 500MHz~3000MHz, 1MHz Step
};
#define	PI							(float)(3.14159265)
*/

// CDF_Incheon2019Dlg 대화 상자
//class CDF_Incheon2019Dlg : public CDialogEx//class CDF_Incheon2019Dlg : public CDialogEx
//{
//// 생성입니다.
//public:
//	CDF_Incheon2019Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
//
//
//	BOOL					Init(void);
//
//
//	BOOL					IsRunning(void);
//
//
//
//// 대화 상자 데이터입니다.
//	enum { IDD = IDD_DF_INCHEON2019_DIALOG };
//
//	//LMS_ADD_20190704
//	CHAR	m_szCurDir[1024];  // 현재 디렉토리 설정 변수	
//	UINT	m_uiRomDataLoadStepAngle;
//
//	INT    iFreqIndex;
//	INT   	iAOA; 
//	INT   	iSimulationAOA; 
//
//	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
//
//
//// 구현입니다.
//protected:
//	HICON m_hIcon;
//
//	// 생성된 메시지 맵 함수
//	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();
//
//    BOOL LoadDfCalRomDataPh();
//
//
//	DECLARE_MESSAGE_MAP()
//public:
//	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton2();
//};



extern "C" __declspec(dllexport) float DllDoCvDfAlgoOperation(INT    iSetFreqPt,      // 주파수 인덱스
	float	*fChcalPhDiff,    // 채널보정데이터 float타입 5채널 degree
	float	*fMeasPhDiff     // 수집데이터     float타입 5채널 degree
	,_stBigArray *pstBigArray // 방사보정데이터 degree x 100 값
	);

