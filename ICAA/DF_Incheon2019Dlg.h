
// DF_Incheon2019Dlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "math.h"
#include "DFData.h"
/*
#define COMINT_MAX_CHANNEL_COUNT	5			// COMIT ��Ž �ִ� ä�� ��

//LMS_ADD_20171221
#define BAND1_START_FREQ				20		// ���뿪 ���� ���ļ� ���� //LMS_ADD_20170209
#define BAND1_STOP_FREQ					500		// ���뿪 ���� ���ļ� ���� //LMS_ADD_20170209
#define BAND2_STOP_FREQ					3000	// ��뿪 ���� ���ļ� ���� //LMS_ADD_20170209
#define APPLY_AZIMUTH_OFFSET_WHEN_CREATING_IDEAL_RADICAL //��纸�������� ���� �� ������ �������� �ݿ��Ҷ� �����Ѵ�.
#define FREQ_RANGE_START_MHz            BAND1_START_FREQ  //LMS_ADD_20171222
#define FREQ_RANGE_END_MHz              8000              //LMS_ADD_20171222   
#define DF_CH_NUM                       9                 //LMS_ADD_20171222    
#define DF_AZIMUTH_NUM                  360               //LMS_ADD_20171222    
struct _stBigArray
{
	USHORT	m_usPhDfRomData[FREQ_RANGE_END_MHz-FREQ_RANGE_START_MHz+1][DF_AZIMUTH_NUM][DF_CH_NUM];     //m_usPhDfRomData[9][360][2501]; // ��纸���� ���� ROM ������[ä��][����][���ļ�] : ���� 0~360, 1�� Step, ���ļ� 500MHz~3000MHz, 1MHz Step
};
#define	PI							(float)(3.14159265)
*/

// CDF_Incheon2019Dlg ��ȭ ����
//class CDF_Incheon2019Dlg : public CDialogEx//class CDF_Incheon2019Dlg : public CDialogEx
//{
//// �����Դϴ�.
//public:
//	CDF_Incheon2019Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
//
//
//	BOOL					Init(void);
//
//
//	BOOL					IsRunning(void);
//
//
//
//// ��ȭ ���� �������Դϴ�.
//	enum { IDD = IDD_DF_INCHEON2019_DIALOG };
//
//	//LMS_ADD_20190704
//	CHAR	m_szCurDir[1024];  // ���� ���丮 ���� ����	
//	UINT	m_uiRomDataLoadStepAngle;
//
//	INT    iFreqIndex;
//	INT   	iAOA; 
//	INT   	iSimulationAOA; 
//
//	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
//
//
//// �����Դϴ�.
//protected:
//	HICON m_hIcon;
//
//	// ������ �޽��� �� �Լ�
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



extern "C" __declspec(dllexport) float DllDoCvDfAlgoOperation(INT    iSetFreqPt,      // ���ļ� �ε���
	float	*fChcalPhDiff,    // ä�κ��������� floatŸ�� 5ä�� degree
	float	*fMeasPhDiff     // ����������     floatŸ�� 5ä�� degree
	,_stBigArray *pstBigArray // ��纸�������� degree x 100 ��
	);

