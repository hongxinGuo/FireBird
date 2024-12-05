// Watchdog.cpp : Defines the class behaviors for the application.
//

#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "resource.h"

import Watchdog.MainFrm;
import Watchdog.ViewTree;
import Watchdog.Document;
import Watchdog.View;
import Watchdog.ChildFrm;
import Watchdog.App;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWatchdogApp

BEGIN_MESSAGE_MAP(CWatchdogApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CWatchdogApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

// CWatchdogApp construction

CWatchdogApp::CWatchdogApp() noexcept {
	m_bHiColorIcons = TRUE;

	m_nAppLook = 0;
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("FireBird.Watchdog.Version.0.01"));

	// Place all significant initialization in InitInstance
}

// The one and only CWatchdogApp object

CWatchdogApp theApp;

HANDLE gl_hMutex = nullptr;

bool IsFireBirdWatchdogAlreadyRunning(const CString& strProgramToken) {
	gl_hMutex = ::CreateMutex(nullptr, false, strProgramToken); // 采用创建系统命名互斥对象的方式来实现只运行单一实例
	bool bAlreadyRunning = false;
	if (gl_hMutex) {
		if (ERROR_ALREADY_EXISTS == ::GetLastError()) {
			bAlreadyRunning = true;
		}
	}
	return bAlreadyRunning;
}

// CWatchdogApp initialization

BOOL CWatchdogApp::InitInstance() {
	if (IsFireBirdWatchdogAlreadyRunning(_T("FireBirdWatchdog"))) {
		MessageBox(nullptr,
		           "Only one instance can run!",
		           "FireBird Watchdog Warning:",
		           MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit()) {
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("FireBird Watchdog"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
	                                             RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate = new CMultiDocTemplate(IDR_WatchdogTYPE,
	                                                        RUNTIME_CLASS(CWatchdogDoc),
	                                                        RUNTIME_CLASS(CWatchdogChildFrame), // custom MDI child frame
	                                                        RUNTIME_CLASS(CWatchdogView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CWatchdogMainFrame* pMainFrame = new CWatchdogMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME)) {
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// The main window has been initialized, so show and update it
	//pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->ShowWindow(SW_MINIMIZE);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CWatchdogApp::ExitInstance() {
	AfxOleTerm(FALSE);

	if (gl_hMutex != nullptr) ::CloseHandle(gl_hMutex);

	return CWinAppEx::ExitInstance();
}

// CWatchdogApp message handlers

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx {
public:
	CAboutDlg() noexcept;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CWatchdogApp::OnAppAbout() {
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CWatchdogApp customization load/save methods

void CWatchdogApp::PreLoadState() {
	CString strName;
	BOOL bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CWatchdogApp::LoadCustomState() {}

void CWatchdogApp::SaveCustomState() {}

// CWatchdogApp message handlers
