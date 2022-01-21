
// ICAADlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ICAA.h"
#include "ICAADlg.h"

#include "ThreadTask\\DFTaskMngr.h"
#include "ADSBReceivedProcessMngr.h"

#include "afxdialogex.h"

#include <sstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


char g_stLogItemType[enMAXItems][20] = { "�ý���", "�α�", "����" } ;


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CICAADlg ��ȭ ����




CICAADlg::CICAADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CICAADlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iTotalNumOfLog = 1;
	m_bFlagLogShow = true;	
}

CICAADlg::~CICAADlg()
{
	
}

void CICAADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_LoglistCtrl);
	DDX_Control(pDX, IDC_BTN_RADARRD_STATUS, m_CButtonRadarRDStatus);
	DDX_Control(pDX, IDC_BTN_PDWCOL_STATUS, m_CButtonPDWColStatus);
}

BEGIN_MESSAGE_MAP(CICAADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SAVELOG, &CICAADlg::OnBnClickedBtnSavelog)
	ON_BN_CLICKED(IDC_BTN_CLEARLOG, &CICAADlg::OnBnClickedBtnClearlog)
	ON_BN_CLICKED(IDC_BUTTON1, &CICAADlg::OnBnClickedButton1)

	ON_MESSAGE(UWM_USER_LOG_MSG, OnLOGMessage )
	ON_MESSAGE(UWM_USER_STAT_MSG, OnStatus )
END_MESSAGE_MAP()


// CICAADlg �޽��� ó����

BOOL CICAADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// �ֻ�� �޴� Ʋ ����
	CRect rt;

	EnableRadarRDStatus( FALSE );
	EnablePDWColStatus( FALSE );

	m_LoglistCtrl.GetWindowRect(&rt);
	m_LoglistCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_LoglistCtrl.InsertColumn(0, TEXT("����"), LVCFMT_LEFT, (int)(rt.Width()*0.1));
	m_LoglistCtrl.InsertColumn(1, TEXT("�׸�"), LVCFMT_LEFT, (int)(rt.Width()*0.1));
	m_LoglistCtrl.InsertColumn(2, TEXT("����"), LVCFMT_LEFT, (int)(rt.Width()*0.8));

	m_DFEquipBITMngr.SetDFPoint(&m_DFTaskMngr);

	g_DlgHandle = GetSafeHwnd();

	this->SetWindowText("���̴� ��Ž ����Ʈ����");
	//CICAAMngr::GetInstance();
	CADSBReceivedProcessMngr::GetInstance();

	Log( enSYSTEM , "���α׷��� ���۵Ǿ����ϴ�." );

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

/**
 * @brief     
 * @param     UINT nID
 * @param     LPARAM lParam
 * @return    void
 * @author    ��ö��(churlhee.jo@lignex1.com)
 * @version   0.0.1
 * @date      2022/01/16 15:47:56
 * @warning   
 */
void CICAADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CICAADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CICAADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CICAADlg::OnBnClickedBtnSavelog()
{
	// ���� ����Ʈ�� ��ϵǾ� �ִ� �α׸� ������
	int iItemCnt = m_LoglistCtrl.GetItemCount();

	if(iItemCnt > 0)
	{
		stringstream ssTempSaveLog = stringstream();

		CStdioFile cSaveLogFile;
		CString cstrOneColume;
		int iStrLen = 0;

		cSaveLogFile.Open("Log.txt", CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeText);

		for(int i=0; i < iItemCnt; i++)
		{
			cstrOneColume = m_LoglistCtrl.GetItemText(i, 0);
			ssTempSaveLog << cstrOneColume << "\t";
			cstrOneColume = "";

			cstrOneColume = m_LoglistCtrl.GetItemText(i, 1);
			ssTempSaveLog << cstrOneColume << "\t";
			cstrOneColume = "";

			cstrOneColume = m_LoglistCtrl.GetItemText(i, 2);
			ssTempSaveLog << cstrOneColume << "\n";
			cstrOneColume = "";

			cSaveLogFile.WriteString(ssTempSaveLog.str().c_str());
			ssTempSaveLog.str("");
		}

		cSaveLogFile.Close();

		stringstream ssTemp = stringstream("");

		ssTemp << m_iTotalNumOfLog;

		m_LoglistCtrl.InsertItem(m_iTotalNumOfLog, ssTemp.str().c_str());
		m_LoglistCtrl.SetItem(m_iTotalNumOfLog, 1, LVIF_TEXT, TEXT("�Ϲ�"), NULL, NULL, NULL, NULL);
		m_LoglistCtrl.SetItem(m_iTotalNumOfLog, 2, LVIF_TEXT, TEXT("������ ����Ǿ����ϴ�."), NULL, NULL, NULL, NULL);

		m_iTotalNumOfLog++;

		ssTemp.str("");
	}
	else
	{
		AfxMessageBox("������ �� �ִ� �αװ� �������� �ʽ��ϴ�.");
	}
}


void CICAADlg::Log( enENUM_ITEM enItemType , char *pszContents )
{
	STR_LOGMESSAGE stMsg;

	stMsg.enItemType = enItemType;
	strcpy( stMsg.szContents, pszContents );

	Log( & stMsg );
}

void CICAADlg::Log( STR_LOGMESSAGE *pMsg )
{
	int nItemNum = m_LoglistCtrl.GetItemCount();

	char szBuffer[100];

	sprintf( szBuffer, "%d" , m_iTotalNumOfLog );
	m_LoglistCtrl.InsertItem( nItemNum, szBuffer );

	sprintf( szBuffer, "%s" , g_stLogItemType[ pMsg->enItemType ] );
	m_LoglistCtrl.SetItemText( nItemNum, 1, szBuffer );
	m_LoglistCtrl.SetItemText( nItemNum, 2, pMsg->szContents );

	m_LoglistCtrl.SendMessage( WM_VSCROLL, SB_BOTTOM );

	++ m_iTotalNumOfLog;

}

void CICAADlg::OnBnClickedBtnClearlog()
{
	int iResult = AfxMessageBox("�α׸� ������ ����ðڽ��ϱ�?", MB_OKCANCEL);

	if(iResult == 1)
	{
		m_bFlagLogShow = false;
		m_iTotalNumOfLog = 0;
		m_LoglistCtrl.DeleteAllItems();
		m_bFlagLogShow = true;
	}
	else
	{

	}
	
}


void CICAADlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_DFTaskMngr.StartChannelCorrect(1);
	//m_DFTaskMngr.SetFreqToRadarUnit(901000);
	//m_DFTaskMngr.AlgrismInit();
}


LRESULT CICAADlg::OnLOGMessage( WPARAM wParam, LPARAM lParam )
{
	// �޽����� �޾Ƽ� ó���ϴ� �Լ�
	enENUM_ITEM enItemType = (enENUM_ITEM ) wParam;
	char *pszContents = (char *) lParam;

	TRACE( "\n" );
	TRACE( pszContents );
	Log( enItemType, pszContents );

	//AfxMessageBox(msg->GetString());
	return 0;
}

void CICAADlg::EnableRadarRDStatus( BOOL bEnable )
{
	if( bEnable == TRUE ) {
		m_CButtonRadarRDStatus.SetWindowTextA( "���̴� �м� ����" );
		m_CButtonRadarRDStatus.SetFaceColor(RGB(0,0,255),true);
		m_CButtonRadarRDStatus.SetTextColor(RGB(255,0,0)); //���ڻ� ����
	}
	else {
		m_CButtonRadarRDStatus.SetWindowTextA( "���̴� �м� �̿���" );
		m_CButtonRadarRDStatus.SetFaceColor(RGB(255,0,0),true);
		m_CButtonRadarRDStatus.SetTextColor(RGB(255,0,0)); //���ڻ� ����
	}

}

void CICAADlg::EnablePDWColStatus( BOOL bEnable )
{
	if( bEnable == TRUE ) {
		m_CButtonPDWColStatus.SetWindowTextA( "PDW������ ����" );
		m_CButtonPDWColStatus.SetFaceColor(RGB(0,0,255),true);
		m_CButtonPDWColStatus.SetTextColor(RGB(255,0,0)); //���ڻ� ����
	}
	else {
		m_CButtonPDWColStatus.SetWindowTextA( "PDW������ �̿���" );
		m_CButtonPDWColStatus.SetFaceColor(RGB(255,0,0),true);
		m_CButtonPDWColStatus.SetTextColor(RGB(255,0,0)); //���ڻ� ����
	}

}

LRESULT CICAADlg::OnStatus( WPARAM wParam, LPARAM lParam )
{
	// �޽����� �޾Ƽ� ó���ϴ� �Լ�
	enENUM_STATUS_UNIT enStatusUnit = (enENUM_STATUS_UNIT ) wParam;
	BOOL bEnable = (BOOL ) lParam;

	switch( enStatusUnit ) {
		case enRADARRD :
			EnableRadarRDStatus( bEnable );
			break;
		case enPDWCOL :
			EnablePDWColStatus( bEnable );
			break;

		default:
			break;
	}

	return 0;
}