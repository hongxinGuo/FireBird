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

void CreateMarketContainer() {
	gl_vMarketPtr.push_back(gl_pWorldMarket); // ������Ʊ�г�
	gl_vMarketPtr.push_back(gl_pChinaMarket); // �й���Ʊ�г�
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
	CreateMarketContainer();	//�����г�����Vector
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ϵͳ���ָ�ϵͳ�ĳ�ʼ̬��׼���ڶ��������������
//
////////////////////////////////////////////////////////////////////////////////////////////
void ResetMarkets() {
	for (const auto& pMarket : gl_vMarketPtr) {
		if (pMarket->IsResetMarket()) {
			pMarket->SetResetMarket(false);
			pMarket->ResetMarket();
		}
	}
}

void ScheduleMarketTask() {
	for (const auto& pVirtualMarket : gl_vMarketPtr) {
		if (pVirtualMarket->IsReadyToRun()) pVirtualMarket->ScheduleTask();
	}
}

void ScheduleTask() {
	try {
		ResetMarkets(); // ����ϵͳ�ڴ˴�ִ�У����׵��ø����ú���
		ScheduleMarketTask();	// ���������Ⱥ���,�ɸ��г����Ⱥ���ִ�о�������
		// ������DataSource�ĵ��ȣ�Ҳ���������˴���
	}
	catch (std::exception* e) { // �˴��ػ���ָ�룬�Ա��������ɾ��֮��
		CString str = _T("ScheduleMarketTask unhandled exception founded : ");
		str += e->what();
		gl_systemMessage.PushInformationMessage(str);
		gl_systemMessage.PushErrorMessage(str);
		delete e; // ɾ��֮����ֹ����û�д���exception���³��������˳���
	}
	catch (CException* e) {
		const CString str = _T("ScheduleMarketTask unhandled exception founded : ");
		gl_systemMessage.PushInformationMessage(str);
		gl_systemMessage.PushErrorMessage(str);
		delete e; // ɾ��֮����ֹ����û�д���exception���³��������˳���
	}
}
