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

	const CString str = "�����г���ʼ�����";
	gl_systemMessage.PushInformationMessage(str);
	gl_pWorldMarket->SetSystemReady(true);
	gl_aTimer.at(WORLD_MARKET_CHECK_SYSTEM_READY__).cancel(); // �г�׼���ú�ɾ��������
}

void CreateMarketContainer() {
	gl_vMarket.push_back(gl_pWorldMarket); // ������Ʊ�г�
	gl_vMarket.push_back(gl_pChinaMarket); // �й���Ʊ�г�
}

void CreateDataSource() {
	// �����Ҫ��gl_pChinaMarketǰ����
	ASSERT(gl_pChinaMarket == nullptr);
	gl_pSinaRTDataSource = make_shared<CSinaRTDataSource>();
	gl_pTengxunRTDataSource = make_shared<CTengxunRTDataSource>();
	gl_pNeteaseRTDataSource = make_shared<CNeteaseRTDataSource>();
	gl_pNeteaseDayLineDataSource = make_shared<CNeteaseDayLineDataSource>();
	gl_pTengxunDayLineDataSource = make_shared<CTengxunDayLineDataSource>();

	// ������Ҫ��gl_pWorldMarketǰ����
	ASSERT(gl_pWorldMarket == nullptr);
	gl_pFinnhubDataSource = make_shared<CFinnhubDataSource>();
	gl_pTiingoDataSource = make_shared<CTiingoDataSource>();
	gl_pQuandlDataSource = make_shared<CQuandlDataSource>();
}

void CreateWebSocket() {
	// WebSocketҪ��gl_pWorldMarket֮ǰ����
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
	case 0:	// ʹ������ʵʱ���ݷ�����
		gl_systemConfiguration.UsingSinaRealtimeServer();
		break;
	case 1: // ʹ������ʵʱ������
		gl_systemConfiguration.UsingNeteaseRealtimeServer();
		break;
	case 2: // ʹ����Ѷʵʱ���ݷ�����
		gl_systemConfiguration.UsingTengxunRealtimeServer();
		break;
	default: // �������ʱĬ��ʹ������ʵʱ���ݷ�����
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0); // ������Ч�ı�־
		gl_systemConfiguration.UsingSinaRealtimeServer();
		break;
	}

	if (gl_systemConfiguration.GetChinaMarketDayLineServer() == 0) {
		// ʹ�������������ݷ�����
		gl_pNeteaseDayLineDataSource->Enable(true);
		gl_pTengxunDayLineDataSource->Enable(false);
	}
	else {
		// ʹ����Ѷ�������ݷ�����
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
	CreateMarketContainer();	//�����г�����
}

void InitializeLogSystem() {
	// Create a daily logger - a new file is created every day at 2:30 am
	gl_dailyLogger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
	gl_dailyWebSocketLogger = spdlog::daily_logger_mt("dailyWebSocketLogger", "logs/dailyWebSocket.txt", 2, 30);
	gl_warnLogger = spdlog::basic_logger_mt("basic_warn_logger", "logs/warn.txt");
	gl_traceLogger = spdlog::basic_logger_mt("basic_trace_logger", "logs/trace.txt");
	gl_SoftwareDevelopingLogger = spdlog::basic_logger_mt("software_developing_logger", "logs/softwareDeveloping.txt");

	//spdlog::flush_every(chrono::seconds(600)); // ÿ10����ˢ��һ�Σ�ֻ������_mtģʽ���ɵ���־��
	gl_dailyWebSocketLogger->set_level(static_cast<spdlog::level::level_enum>(gl_systemConfiguration.GetLogLevel()));
	gl_dailyLogger->flush_on(spdlog::level::warn); // ����ȼ�����������ˢ��
	gl_dailyWebSocketLogger->flush_on(spdlog::level::warn);
	gl_warnLogger->flush_on(spdlog::level::trace);
	gl_traceLogger->enable_backtrace(20); // 20��������Ϣ

	gl_dailyLogger->info("FireBird App begin running");
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ϵͳ���ָ�ϵͳ�ĳ�ʼ̬��׼���ڶ��������������
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
/// ÿ100������ȵ����񣬸��������Ҫ����
///
///
///<summery>
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void TaskSchedulePer100ms() {
	static bool s_Processing = false;
	if (IsMarketResetting()) return;// �г�������Ҫ�ϳ�ʱ�䣬�޷����й������ʶ���ͣ���ȡ�
	if (s_Processing) {
		gl_dailyLogger->warn("TaskSchedulePer100ms()��������");
#ifdef _TRACE_SCHEDULE_TASK___
		gl_traceLogger->warn("TaskSchedulePer100ms()��������");
		gl_traceLogger->dump_backtrace(); // 
#endif
		return;
	}
	s_Processing = true;
	auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
	try {
		// ��ȡϵͳʱ�����
		gl_tpNow = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());

		ScheduleMarketTask();	// ���������Ⱥ���,�ɸ��г����Ⱥ���ִ�о�������
		//todo ������DataSource�ĵ��ȣ�Ҳ���������˴���Ŀǰ��DataSource�ĵ��ȣ���CVirtualMarket��ScheduleTask()�С�
	} catch (std::exception* e) { // �˴��ػ���ָ�룬�Ա��������ɾ��֮��
		CString str = _T("ScheduleMarketTask unhandled exception founded : ");
		str += e->what();
		gl_systemMessage.PushInformationMessage(str);
		gl_systemMessage.PushErrorMessage(str);
		delete e; // ɾ��֮����ֹ����û�д���exception���³��������˳���
	}
	catch (CException* e) {
		char buffer[1000];
		CString str = _T("ScheduleMarketTask unhandled CException founded : ");
		e->GetErrorMessage(buffer, 1);
		str += buffer;
		gl_systemMessage.PushInformationMessage(str);
		gl_systemMessage.PushErrorMessage(str);
		//gl_warnLogger->error("{}", str);
		delete e; // ɾ��֮����ֹ����û�д���exception���³��������˳���
	}
	auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
	gl_systemMessage.IncreaseScheduleTaskTime((end - start).count());
	s_Processing = false;
}

////////////////////////////////////////////////////////////////////////////////////////////
///
///<summery>
///
/// ÿ����ȵ����񣬸�����ɸ��ָ�������ÿ100������ȵ���������Ҫ����
///
///
///<summery>
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void TaskSchedulePerSecond() {
	try {
		gl_pSinaRTDataSource->CalcTotalBytePerSecond(); // ����ÿ���ȡ��������
		gl_systemMessage.CalcScheduleTaskTimePerSecond(); // ����ÿ����������ʱ��
	} catch (std::exception* e) {	// �˴��ػ���ָ�룬�Ա��������ɾ��֮��
		CString str1 = "TaskSchedulePerSecond unhandled exception founded : ";
		str1 += e->what();
		gl_systemMessage.PushErrorMessage(str1);
		gl_warnLogger->error("{}", str1.GetBuffer());
		delete e; // ɾ��֮����ֹ����û�д���exception���³��������˳���
	}
	catch (CException* e) {	// �˴��ػ���ָ�룬�Ա��������ɾ��֮��
		char buffer[1000];
		CString str = _T("TaskSchedulePerSecond unhandled CException founded : ");
		e->GetErrorMessage(buffer, 1);
		str += buffer;
		gl_systemMessage.PushErrorMessage(str);
		gl_warnLogger->error("{}", str.GetBuffer());
		delete e; // ɾ��֮����ֹ����û�д���exception���³��������˳���
	}
}

void TaskExitSystem() {
	// �������ڷ��͹رմ���ϵͳ��Ϣ��֪ͨ��ܴ���ִ�йر�����
	// ����ϵͳ��Ҫ˳��رո������񣬹ʶ�������ֱ���˳�ϵͳ��
	PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), WM_SYSCOMMAND, SC_CLOSE, 0);
}

void SystemInitialization() {
	// ϵͳ��ʼ����ʼ
	CreateSimdjsonEmptyArray();

	SetMaxCurrencyLevel();

	// ��ʼ����market dataSource WebSocket
	InitializeMarkets();
	AssignDataSourceAndWebInquiryToMarket();
	ResetMarkets(); // ҪԤ������һ��

	// ���ù����߳��������
	gl_systemConfiguration.SetThreadPoolExecutorCurrencyLevel(gl_runtime.thread_pool_executor()->max_concurrency_level());
	gl_systemConfiguration.SetThreadExecutorCurrencyLevel(gl_runtime.thread_executor()->max_concurrency_level());
	gl_systemConfiguration.SetBackgroundExecutorCurrencyLevel(gl_runtime.background_executor()->max_concurrency_level());

	TRACE("Start scheduling task\n");
	// ����100����ÿ�εĹ����̵߳��ȣ��������ϵͳ���ʱ����
	gl_aTimer.at(GENERAL_TASK_PER_100MS__) = gl_runtime.timer_queue()->make_timer(
		1000ms,
		100ms,
		gl_runtime.thread_executor(), // ��Ϊ���������������ӣ��ʶ�ʹ�ö����Ĺ����߳�����������
		::TaskSchedulePer100ms);

	// ����ÿ��ִ��һ�εĸ��������̵߳��ȣ�����ִ�и����������
	gl_aTimer.at(GENERAL_TASK_PER_SECOND__) = gl_runtime.timer_queue()->make_timer(
		1000ms,
		1000ms,
		gl_runtime.thread_executor(), // ��Ϊ������������
		::TaskSchedulePerSecond);
}
