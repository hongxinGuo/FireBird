// FireBird.cpp: 定义应用程序的类行为。
//
#include"pch.h"

#include "FireBird.h"  

#include "MainFrm.h"

#include "FireBirdDoc.h"
#include "FireBirdView.h" 

#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"

#ifndef _MBCS
#error _T("本系统使用多字节字符集")
#endif

// CFireBirdApp

BEGIN_MESSAGE_MAP(CFireBirdApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CFireBirdApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

// CFireBirdApp 构造

CFireBirdApp::CFireBirdApp() {
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	SetAppID(_T("FireBird.AppID.0.30"));

	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CFireBirdApp 对象
CFireBirdApp theApp;

bool IsFireBirdAlreadyRunning(const CString& strProgramToken) {
	gl_hFireBirdMutex = ::CreateMutex(nullptr, false, strProgramToken); // 采用创建系统命名互斥对象的方式来实现只运行单一实例
	bool bAlreadyRunning = false;
	if (gl_hFireBirdMutex != nullptr) {
		if (ERROR_ALREADY_EXISTS == ::GetLastError()) {
			bAlreadyRunning = true;
		}
	}
	return bAlreadyRunning;
}

// CFireBirdApp 初始化
BOOL CFireBirdApp::InitInstance() {
	if (IsFireBirdAlreadyRunning(_T("FireBirdStockAnalysis"))) {
		MessageBox(nullptr,
			"Only one instance can run!",
			"FireBird Warning:",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Create a daily logger - a new file is created every day at 2:30 am
	gl_dailyLogger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
	gl_dailyWebSocketLogger = spdlog::daily_logger_mt("dailyWebSocketLogger", "logs/dailyWebSocket.txt", 2, 30);
	gl_dailyWebSocketLogger->set_level(static_cast<spdlog::level::level_enum>(gl_systemConfiguration.GetLogLevel()));
	//spdlog::flush_every(chrono::seconds(600)); // 每10分钟刷新一次（只能用于_mt模式生成的日志）
	gl_dailyLogger->flush_on(spdlog::level::warn); // 警告等级及以上立刻刷新
	gl_dailyWebSocketLogger->flush_on(spdlog::level::warn);

	gl_traceLogger = spdlog::basic_logger_mt("basic_trace_logger", "logs/trace.txt");
	gl_traceLogger->enable_backtrace(20); // 20个回溯消息

	gl_dailyLogger->info("FireBird App begin running");

	ASSERT(gl_systemConfiguration.IsWorkingMode()); // 确保此标识初始态为实际状态

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	EnableTaskbarInteraction(FALSE);

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	SetRegistryKey(_T("FireBird Stock Analysis"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CSingleDocTemplate* pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFireBirdDoc),
		RUNTIME_CLASS(CMainFrame), // main SDI frame window
		RUNTIME_CLASS(CFireBirdView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。  如果用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CFireBirdApp::ExitInstance() {

	gl_dailyLogger->info("FireBird App exit"); 

	// Under VisualStudio, this must be called before main finishes to work around a known VS issue
	spdlog::drop_all();

	//无论跟踪到了什么都在退出时存储之。
	gl_traceLogger->dump_backtrace(); 

	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CFireBirdApp 消息处理程序

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CFireBirdApp::OnAppAbout() {
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CFireBirdApp 自定义加载/保存方法

void CFireBirdApp::PreLoadState() {
	CString strName;
	const BOOL bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CFireBirdApp::LoadCustomState() {
}

void CFireBirdApp::SaveCustomState() {
}

// CFireBirdApp 消息处理程序