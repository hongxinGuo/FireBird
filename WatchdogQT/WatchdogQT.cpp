#include "WatchdogQT.h"

#include <afx.h>
#include<QMessageBox>
#include<QLabel>
#include<QChronoTimer>

#include<chrono>
#include <spdlog/spdlog.h>
using namespace std;

//#include"resource.h"

std::chrono::sys_seconds gl_tpNow; // 协调世界时（Coordinated Universal Time）

/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(msg == WM_COPYDATA) {
		COPYDATASTRUCT* pCds = (COPYDATASTRUCT*)lParam;
		if(pCds->cbData == 0) {
			string sTime = (char*)pCds->lpData;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int registerWindow() {
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "ReceiverWindow";

	if(!RegisterClass(&wc)) {
		// Error
		return 1;
	}

	HWND hWnd = CreateWindow(
		"ReceiverWindow",
		"ReceiverWindow",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

	if(hWnd == NULL) {
		// Error
		return 1;
	}

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	getchar();
	return 0;
}
*/
WatchdogQT::WatchdogQT(QWidget* parent) : QMainWindow(parent) {
	gl_tpNow = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now()); // 程序运行的第一步即要获取当前时间。以防止出现时间为零的故障。

	// 设置每秒一次的计时器
	auto* timer = new QChronoTimer(1s, this);
	connect(timer, &QChronoTimer::timeout, this, &WatchdogQT::Update);
	timer->start();
	// 设置每十秒一次的计时器
	auto* timer2 = new QChronoTimer(10s, this);
	connect(timer2, &QChronoTimer::timeout, this, &WatchdogQT::UpdatePer10Second);
	timer2->start();

	setWindowState(Qt::WindowMinimized); // watchdog初始时最小化

	setWindowIcon(QIcon(":/WatchdogQT/Watchdog.ico")); // Note 路径需要使用全称。
	//setWindowIcon(QIcon("WatchDog.ico"));

	ui.setupUi(this);

	BuildUI();

	connect(ui.actionAbout, &QAction::triggered, this, &WatchdogQT::onActionAboutTriggered);
}

WatchdogQT::~WatchdogQT() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当FireBird主动退出时，它向WatchdogQT发送WM_COPYDATA消息，告知准备退出。
//
// QT nativeEvent()函数无法接收WParam和LParam.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WatchdogQT::nativeEvent(const QByteArray& eventType, void* message, qintptr* result) {
	tm tmLocal;
	long long time;
	string s;
	MSG* msg = static_cast<MSG*>(message);
	switch (msg->message) {
	case WM_COPYDATA:
		time = gl_tpNow.time_since_epoch().count();
		localtime_s(&tmLocal, &time);
		s = fmt::format("FireBird于: {:04d}年{:02d}月{:02d}日 {:02d}:{:02d}:{:02d} 主动关闭", tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		m_listOutput.push_back(s);
		break;
	default:
		return false;
	}
	return false;
}

void WatchdogQT::BuildUI() {
	// 创建状态栏 
	labTime = new QLabel(this);
	labTime->setMinimumWidth(50);
	labTime->setText("00:00:00");
	ui.statusBar->addPermanentWidget(labTime);
}

void WatchdogQT::Update() const {
	gl_tpNow = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());
	tm tmLocal;
	const auto time = gl_tpNow.time_since_epoch().count();
	localtime_s(&tmLocal, &time);
	string s = fmt::format("{:02d}:{:02d}:{:02d}", tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	labTime->setText(s.c_str());

	string sTotal;
	for (auto s1 : m_listOutput) {
		sTotal += s1;
		sTotal += _T("\n");
	}
	ui.textBrowser->setText(sTotal.c_str());
}

bool IsFireBirdAlreadyRunning(const string& strProgramToken) {
	const HANDLE hMutex = CreateMutex(nullptr, false, strProgramToken.c_str()); // 采用创建系统命名互斥对象的方式来实现只运行单一实例
	bool bAlreadyRunning = false;
	if (hMutex) {
		if (ERROR_ALREADY_EXISTS == ::GetLastError()) {
			bAlreadyRunning = true;
		}
	}
	::CloseHandle(hMutex); // 无论成功与否，都要关闭此系统互斥变量。因系统互斥变量是计数的，当该变量已经存在时，生成之的实际行为是计数加一。计数为零时则自动删除。
	return bAlreadyRunning;
}

void WatchdogQT::UpdatePer10Second() {
	static int s_Counter = 6; // 初始值为6次，即一分钟后执行启动FireBird任务（每十秒钟监视一次）
	if (--s_Counter < 1) {
		if (!IsFireBirdAlreadyRunning("FireBirdStockAnalysis")) {
			const UINT iReturnCode = WinExec(("C:\\FireBird\\FireBird.exe"), SW_SHOW);
			tm tmLocal;
			const auto time = gl_tpNow.time_since_epoch().count();
			localtime_s(&tmLocal, &time);
			const string s = fmt::format("启动FireBird于: {:04d}年{:02d}月{:02d}日 {:02d}:{:02d}:{:02d}", tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
			if (m_listOutput.size() > 100) m_listOutput.pop_front();
			m_listOutput.push_back(s);
			ASSERT(iReturnCode > 31);
		}
		s_Counter = 0;
	}
}

void WatchdogQT::onActionAboutTriggered() {
	QString text = tr("FireBird Watchdog\n"
		"Version 0.01\n"
		"2023 Copyright FireBird Inc.\n"
		"All right reserved\n\n");

	QMessageBox::about(this, tr("Watchdog"), text);
}
