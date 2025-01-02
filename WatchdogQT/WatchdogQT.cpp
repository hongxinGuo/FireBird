#include "WatchdogQT.h"

#include <afx.h>
#include<QMessageBox>
#include<QLabel>
#include<QChronoTimer>

#include<chrono>
#include <spdlog/fmt/bundled/core.h>
using namespace std;

#include"resource.h"

#ifdef _DEBUG
#pragma comment(lib, "/D/concurrencpp.lib")
#else
#pragma comment(lib, "/R/concurrencpp.lib")
#endif

std::chrono::sys_seconds gl_tpNow; // 协调世界时（Coordinated Universal Time）

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

	setWindowIcon(QIcon(":/WatchdogQT/Watchdog.ico")); // Note 路径需要使用全称。
	//setWindowIcon(QIcon("WatchDog.ico"));

	ui.setupUi(this);

	BuildUI();

	connect(ui.actionAbout, &QAction::triggered, this, &WatchdogQT::onActionAboutTriggered);
}

WatchdogQT::~WatchdogQT() {
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
	char buffer[100];
	const auto time = gl_tpNow.time_since_epoch().count();
	localtime_s(&tmLocal, &time);
	sprintf_s(buffer, "%02d:%02d:%02d ", tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
	string s = buffer;
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
	static int s_Counter = 1; // 初始值为60次，即一分钟后执行启动FireBird任务（每十秒钟监视一次）
	if (--s_Counter < 1) {
		if (!IsFireBirdAlreadyRunning("FireBirdStockAnalysis")) {
			const UINT iReturnCode = WinExec(("C:\\FireBird\\FireBird.exe"), SW_SHOW);
			tm tmLocal;
			char buffer[100];
			const auto time = gl_tpNow.time_since_epoch().count();
			localtime_s(&tmLocal, &time);
			sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d"), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
			string s = "启动FireBird于: ";
			s += buffer;
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
