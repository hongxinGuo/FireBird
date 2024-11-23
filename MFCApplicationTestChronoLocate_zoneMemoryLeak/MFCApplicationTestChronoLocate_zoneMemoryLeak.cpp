
// MFCApplicationTestChronoLocate_zoneMemoryLeak.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MFCApplicationTestChronoLocate_zoneMemoryLeak.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplicationTestChronoLocatezoneMemoryLeakApp

BEGIN_MESSAGE_MAP(CMFCApplicationTestChronoLocatezoneMemoryLeakApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMFCApplicationTestChronoLocatezoneMemoryLeakApp::OnAppAbout)
END_MESSAGE_MAP()


// CMFCApplicationTestChronoLocatezoneMemoryLeakApp construction

CMFCApplicationTestChronoLocatezoneMemoryLeakApp::CMFCApplicationTestChronoLocatezoneMemoryLeakApp() noexcept
{

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MFCApplicationTestChronoLocatezoneMemoryLeak.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMFCApplicationTestChronoLocatezoneMemoryLeakApp object

CMFCApplicationTestChronoLocatezoneMemoryLeakApp theApp;


// CMFCApplicationTestChronoLocatezoneMemoryLeakApp initialization

BOOL CMFCApplicationTestChronoLocatezoneMemoryLeakApp::InitInstance()
{
	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);





	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CMFCApplicationTestChronoLocatezoneMemoryLeakApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CMFCApplicationTestChronoLocatezoneMemoryLeakApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CMFCApplicationTestChronoLocatezoneMemoryLeakApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMFCApplicationTestChronoLocatezoneMemoryLeakApp message handlers



