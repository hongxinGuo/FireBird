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
#include "HighPerformanceCounter.h"
#include"WorldMarket.h"

#include "simdjsonGetValue.h"
#include "Thread.h"

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
	CHighPerformanceCounter counter;
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
	counter.start();
	try {
		ScheduleMarketTask();	// ���������Ⱥ���,�ɸ��г����Ⱥ���ִ�о�������
		// not implemented ������DataSource�ĵ��ȣ�Ҳ���������˴���Ŀǰ��DataSource�ĵ��ȣ���CVirtualMarket��ScheduleTask()�С�
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
		delete e; // ɾ��֮����ֹ����û�д���exception���³��������˳���
	}
	counter.stop();
	gl_systemMessage.IncreaseScheduleTaskTime(counter.GetElapsedMicroSecond());
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
	} catch (std::exception* e) { // �˴��ػ���ָ�룬�Ա��������ɾ��֮��
		CString str = _T("TaskSchedulePerSecond unhandled exception founded : ");
		str += e->what();
		gl_systemMessage.PushErrorMessage(str);
		delete e; // ɾ��֮����ֹ����û�д���exception���³��������˳���
	}
	catch (CException* e) {
		char buffer[1000];
		CString str = _T("TaskSchedulePerSecond unhandled CException founded : ");
		e->GetErrorMessage(buffer, 1);
		str += buffer;
		gl_systemMessage.PushErrorMessage(str);
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

	// ����ϵͳ��ʾ�߶ȺͿ��
	gl_systemConfiguration.SetSystemDisplayRect(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
	gl_systemConfiguration.SetCurrentWindowRect(GetSystemMetrics(SM_CXMAXIMIZED), GetSystemMetrics(SM_CYMAXIMIZED));

	// ��ʼ����market dataSource WebSocket
	InitializeMarkets();
	AssignDataSourceAndWebInquiryToMarket();
	ResetMarkets(); // ҪԤ������һ��
}
