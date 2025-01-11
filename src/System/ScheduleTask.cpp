#include "pch.h"

#include "ScheduleTask.h"

#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"NeteaseRTDataSource.h"
#include"NeteaseDayLineDataSource.h"
#include"TengxunDayLineDataSource.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"
#include"QuandlDataSource.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

#include "simdjsonGetValue.h"
#include "Thread.h"

#include <spdlog/sinks/basic_file_sink.h>

void TaskCheckWorldMarketReady() {
	if (gl_pWorldMarket->IsSystemReady()) return;
	if (gl_pFinnhubDataSource->IsUpdateSymbol()) return;
	if (gl_pFinnhubDataSource->IsUpdateForexExchange()) return;
	if (gl_pFinnhubDataSource->IsUpdateForexSymbol()) return;
	if (gl_pFinnhubDataSource->IsUpdateCryptoExchange()) return;
	if (gl_pFinnhubDataSource->IsUpdateCryptoSymbol()) return;

	const CString str = "世界市场初始化完毕";
	gl_systemMessage.PushInformationMessage(str);
	gl_pWorldMarket->SetSystemReady(true);
	gl_aTimer.at(WORLD_MARKET_CHECK_SYSTEM_READY__).cancel(); // 市场准备好后即删除此任务。
}

void CreateMarketContainer() {
	gl_vMarket.push_back(gl_pWorldMarket); // 美国股票市场
	gl_vMarket.push_back(gl_pChinaMarket); // 中国股票市场
}

void CreateDataSource() {
	// 此五个要在gl_pChinaMarket前生成
	ASSERT(gl_pChinaMarket == nullptr);
	gl_pSinaRTDataSource = make_shared<CSinaRTDataSource>();
	gl_pTengxunRTDataSource = make_shared<CTengxunRTDataSource>();
	gl_pNeteaseRTDataSource = make_shared<CNeteaseRTDataSource>();
	gl_pNeteaseDayLineDataSource = make_shared<CNeteaseDayLineDataSource>();
	gl_pTengxunDayLineDataSource = make_shared<CTengxunDayLineDataSource>();

	// 此三个要在gl_pWorldMarket前生成
	ASSERT(gl_pWorldMarket == nullptr);
	gl_pFinnhubDataSource = make_shared<CFinnhubDataSource>();
	gl_pTiingoDataSource = make_shared<CTiingoDataSource>();
	gl_pQuandlDataSource = make_shared<CQuandlDataSource>();
}

void CreateWebSocket() {
	// WebSocket要在gl_pWorldMarket之前生成
	ASSERT(gl_pWorldMarket == nullptr);
	gl_pFinnhubWebSocket = make_shared<CFinnhubWebSocket>();
	gl_pTiingoIEXWebSocket = make_shared<CTiingoIEXWebSocket>();
	gl_pTiingoCryptoWebSocket = make_shared<CTiingoCryptoWebSocket>();
	gl_pTiingoForexWebSocket = make_shared<CTiingoForexWebSocket>();
}

void CreateMarket() {
	if (gl_pChinaMarket == nullptr) gl_pChinaMarket = make_shared<CChinaMarket>();
	if (gl_pWorldMarket == nullptr) gl_pWorldMarket = make_shared<CWorldMarket>();
}

void AssignDataSourceAndWebInquiryToMarket() {
	ASSERT(gl_pChinaMarket != nullptr);
	ASSERT(gl_pWorldMarket != nullptr);

	// china market's data source 
	gl_pChinaMarket->StoreDataSource(gl_pSinaRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pTengxunRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pNeteaseRTDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pNeteaseDayLineDataSource);
	gl_pChinaMarket->StoreDataSource(gl_pTengxunDayLineDataSource);

	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case 0:	// 使用新浪实时数据服务器
		gl_systemConfiguration.UsingSinaRealtimeServer();
		break;
	case 1: // 使用网易实时服务器
		gl_systemConfiguration.UsingNeteaseRealtimeServer();
		break;
	case 2: // 使用腾讯实时数据服务器
		gl_systemConfiguration.UsingTengxunRealtimeServer();
		break;
	default: // 例外情况时默认使用新浪实时数据服务器
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0); // 改正无效的标志
		gl_systemConfiguration.UsingSinaRealtimeServer();
		break;
	}

	if (gl_systemConfiguration.GetChinaMarketDayLineServer() == 0) {
		// 使用网易日线数据服务器
		gl_pNeteaseDayLineDataSource->Enable(true);
		gl_pTengxunDayLineDataSource->Enable(false);
	}
	else {
		// 使用腾讯日线数据服务器
		gl_pNeteaseDayLineDataSource->Enable(false);
		gl_pTengxunDayLineDataSource->Enable(true);
	}

	// world market's data source
	gl_pWorldMarket->StoreDataSource(gl_pFinnhubDataSource);
	gl_pWorldMarket->StoreDataSource(gl_pTiingoDataSource);
}

void InitializeMarkets() {
	CreateWebSocket();
	CreateDataSource();
	CreateMarket();
	CreateMarketContainer();	//生成市场容器
}

void InitializeLogSystem() {
	// Create a daily logger - a new file is created every day at 2:30 am
	gl_dailyLogger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
	gl_dailyWebSocketLogger = spdlog::daily_logger_mt("dailyWebSocketLogger", "logs/dailyWebSocket.txt", 2, 30);
	gl_warnLogger = spdlog::basic_logger_mt("basic_warn_logger", "logs/warn.txt");
	gl_traceLogger = spdlog::basic_logger_mt("basic_trace_logger", "logs/trace.txt");
	gl_SoftwareDevelopingLogger = spdlog::basic_logger_mt("software_developing_logger", "logs/softwareDeveloping.txt");

	//spdlog::flush_every(chrono::seconds(600)); // 每10分钟刷新一次（只能用于_mt模式生成的日志）
	gl_dailyWebSocketLogger->set_level(static_cast<spdlog::level::level_enum>(gl_systemConfiguration.GetLogLevel()));
	gl_dailyLogger->flush_on(spdlog::level::warn); // 警告等级及以上立刻刷新
	gl_dailyWebSocketLogger->flush_on(spdlog::level::warn);
	gl_warnLogger->flush_on(spdlog::level::trace);
	gl_traceLogger->enable_backtrace(20); // 20个回溯消息

	gl_dailyLogger->info("FireBird App begin running");
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// 重置系统（恢复系统的初始态，准备第二天继续工作）。
//
////////////////////////////////////////////////////////////////////////////////////////////
void ResetMarkets() {
	for (const auto& pMarket : gl_vMarket) {
		pMarket->ResetMarket();
	}
}

bool IsMarketResetting() {
	for (const auto& pMarket : gl_vMarket) {
		if (pMarket->IsResetting()) return true;
	}
	return false;
}

void ScheduleMarketTask() {
	for (const auto& pVirtualMarket : gl_vMarket) {
		pVirtualMarket->ScheduleTask();
	}
}

void SetMaxCurrencyLevel() {
	if (gl_runtime.thread_pool_executor()->max_concurrency_level() >= 8) gl_concurrency_level = 4;
	else gl_concurrency_level = 3;
}

////////////////////////////////////////////////////////////////////////////////////////////
///
///<summery>
///
/// 每100毫秒调度的任务，负责各种主要任务
///
///
///<summery>
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void TaskSchedulePer100ms() {
	static bool s_Processing = false;
	if (IsMarketResetting()) return;// 市场重启需要较长时间，无法并行工作，故而暂停调度。
	if (s_Processing) {
		gl_dailyLogger->warn("TaskSchedulePer100ms()发生重入");
#ifdef _TRACE_SCHEDULE_TASK___
		gl_traceLogger->warn("TaskSchedulePer100ms()发生重入");
		gl_traceLogger->dump_backtrace(); // 
#endif
		return;
	}
	s_Processing = true;
	auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
	try {
		// 获取系统时间戳。
		gl_tpNow = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());

		ScheduleMarketTask();	// 调用主调度函数,由各市场调度函数执行具体任务
		//todo 其他各DataSource的调度，也考虑移至此处。目前各DataSource的调度，在CVirtualMarket的ScheduleTask()中。
	} catch (std::exception* e) { // 此处截获本体指针，以备处理完后删除之。
		CString str = _T("ScheduleMarketTask unhandled exception founded : ");
		str += e->what();
		gl_systemMessage.PushInformationMessage(str);
		gl_systemMessage.PushErrorMessage(str);
		delete e; // 删除之，防止由于没有处理exception导致程序意外退出。
	}
	catch (CException* e) {
		char buffer[1000];
		CString str = _T("ScheduleMarketTask unhandled CException founded : ");
		e->GetErrorMessage(buffer, 1);
		str += buffer;
		gl_systemMessage.PushInformationMessage(str);
		gl_systemMessage.PushErrorMessage(str);
		//gl_warnLogger->error("{}", str);
		delete e; // 删除之，防止由于没有处理exception导致程序意外退出。
	}
	auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
	gl_systemMessage.IncreaseScheduleTaskTime((end - start).count());
	s_Processing = false;
}

////////////////////////////////////////////////////////////////////////////////////////////
///
///<summery>
///
/// 每秒调度的任务，负责完成各种辅助任务（每100毫秒调度的任务是主要任务）
///
///
///<summery>
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void TaskSchedulePerSecond() {
	try {
		gl_pSinaRTDataSource->CalcTotalBytePerSecond(); // 计算每秒读取的数据量
		gl_systemMessage.CalcScheduleTaskTimePerSecond(); // 计算每秒调度所需的时间
	} catch (std::exception* e) {	// 此处截获本体指针，以备处理完后删除之。
		CString str1 = "TaskSchedulePerSecond unhandled exception founded : ";
		str1 += e->what();
		gl_systemMessage.PushErrorMessage(str1);
		gl_warnLogger->error("{}", str1.GetBuffer());
		delete e; // 删除之，防止由于没有处理exception导致程序意外退出。
	}
	catch (CException* e) {	// 此处截获本体指针，以备处理完后删除之。
		char buffer[1000];
		CString str = _T("TaskSchedulePerSecond unhandled CException founded : ");
		e->GetErrorMessage(buffer, 1);
		str += buffer;
		gl_systemMessage.PushErrorMessage(str);
		gl_warnLogger->error("{}", str.GetBuffer());
		delete e; // 删除之，防止由于没有处理exception导致程序意外退出。
	}
}

void TaskExitSystem() {
	// 向主窗口发送关闭窗口系统消息，通知框架窗口执行关闭任务。
	// 由于系统需要顺序关闭各项任务，故而不允许直接退出系统。
	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_SYSCOMMAND, SC_CLOSE, 0);
}

void SystemInitialization() {
	// 系统初始化开始
	CreateSimdjsonEmptyArray();

	SetMaxCurrencyLevel();

	// 初始化各market dataSource WebSocket
	InitializeMarkets();
	AssignDataSourceAndWebInquiryToMarket();
	ResetMarkets(); // 要预先重置一次

	// 设置工作线程最大数量
	gl_systemConfiguration.SetThreadPoolExecutorCurrencyLevel(gl_runtime.thread_pool_executor()->max_concurrency_level());
	gl_systemConfiguration.SetThreadExecutorCurrencyLevel(gl_runtime.thread_executor()->max_concurrency_level());
	gl_systemConfiguration.SetBackgroundExecutorCurrencyLevel(gl_runtime.background_executor()->max_concurrency_level());

	TRACE("Start scheduling task\n");
	// 设置100毫秒每次的工作线程调度，用于完成系统各项定时任务。
	gl_aTimer.at(GENERAL_TASK_PER_100MS__) = gl_runtime.timer_queue()->make_timer(
		1000ms,
		100ms,
		gl_runtime.thread_executor(), // 此为主调度任务，任务繁杂，故而使用独立的工作线程来调度任务
		::TaskSchedulePer100ms);

	// 设置每秒执行一次的辅助工作线程调度，用于执行各项辅助工作。
	gl_aTimer.at(GENERAL_TASK_PER_SECOND__) = gl_runtime.timer_queue()->make_timer(
		1000ms,
		1000ms,
		gl_runtime.thread_executor(), // 此为辅助调度任务
		::TaskSchedulePerSecond);
}
