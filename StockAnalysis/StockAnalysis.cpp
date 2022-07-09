// StockAnalysis.cpp: 定义应用程序的类行为。
//
// 可以使用Virtual leak detector(vld)查找内存泄漏。
// 由于vld官方源码支持只到vs2015，故而需要手工修改源代码，才能使用VS2022编译。
// 且dbghelp.dll需要使用目录C:\Program Files\Microsoft Visual Studio\2022\Exterprice\Common7\IDE\CommonExtensions\Microsoft\TestWindow\VsTest\X64中的那个版本
// 需要将vld\VS2022文件夹中的文件dbghelp.dll Microsoft.DTfW.DHL.manifest vld_x64.dll三个文件拷贝至执行文件夹中
//
//#include"vld.h" // 需要检测内存泄漏时包括此头文件

#include"pch.h"

#include"ThreadStatus.h"

#include "StockAnalysis.h"
#include "MainFrm.h"

#include "StockAnalysisDoc.h"
#include "StockAnalysisView.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

#include <ixwebsocket/IXNetSystem.h>

#ifdef _DEBUG
#ifdef _OPENSSL_USE_DLL
//#pragma comment(lib, "/vc/libcrypto64MTd.lib")
//#pragma comment(lib, "/vc/libssl64MTd.lib")
#else
//#pragma comment(lib, "/vc/static/libcrypto64MTd.lib")
//#pragma comment(lib, "/vc/static/libssl64MTd.lib")
#endif
#else
#ifdef _OPENSSL_USE_DLL
//#pragma comment(lib, "/vc/static/libcrypto64MT.lib")
//#pragma comment(lib, "/vc/static/libssl64MT.lib")
#else
//#pragma comment(lib, "/vc/static/libcrypto64MT.lib")
//#pragma comment(lib, "/vc/static/libssl64MT.lib")
#endif
#endif

#ifndef _MBCS
#error _T("本系统使用多字节字符集")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStockAnalysisApp

BEGIN_MESSAGE_MAP(CStockAnalysisApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CStockAnalysisApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

// CStockAnalysisApp 构造

CStockAnalysisApp::CStockAnalysisApp() {
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("FiriBird.StockAnalysis.AppID.0.25"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CStockAnalysisApp 对象
CStockAnalysisApp theApp;

// CStockAnalysisApp 初始化

BOOL CStockAnalysisApp::InitInstance() {
#ifndef DEBUG
	// 非调试状态下只允许运行一个实例
	if (FindWindow(nullptr, "StockAnalysis1 - StockAnalysis")) {
		//找顶层窗口。这种方法不太稳妥，当程序运行后可能会改变顶层窗口的名称，导致判断失误
		MessageBox(nullptr,
			"Only one instance can run!",
			"FireBird Stock Analysis Warnning:",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
#endif // DEBUG
	ASSERT(!gl_systemStatus.IsWorkingMode()); // 确保此标识初始态为测试状态
	gl_systemStatus.SetWorkingMode(true); // 实际系统，测试状态为假。

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	EnableTaskbarInteraction(FALSE);

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("FireBird"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CStockAnalysisDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CStockAnalysisView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CStockAnalysisApp::ExitInstance() {
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CStockAnalysisApp 消息处理程序

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
void CStockAnalysisApp::OnAppAbout() {
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CStockAnalysisApp 自定义加载/保存方法

void CStockAnalysisApp::PreLoadState() {
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CStockAnalysisApp::LoadCustomState() {
}

void CStockAnalysisApp::SaveCustomState() {
}

// CStockAnalysisApp 消息处理程序