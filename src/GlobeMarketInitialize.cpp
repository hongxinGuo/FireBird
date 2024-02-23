#include "pch.h"
#include "GlobeMarketInitialize.h"

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
	gl_vMarketPtr.push_back(gl_pWorldMarket); // 美国股票市场
	gl_vMarketPtr.push_back(gl_pChinaMarket); // 中国股票市场
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
	CreateMarketContainer();	//生成市场容器Vector
}

void ResetMarkets() {
	for (const auto& pMarket : gl_vMarketPtr) {
		if (pMarket->IsResetMarket()) {
			pMarket->SetResetMarket(false);
			pMarket->ResetMarket();
		}
	}
}

void SchedulingTask() {
	for (const auto& pVirtualMarket : gl_vMarketPtr) {
		if (pVirtualMarket->IsReadyToRun()) pVirtualMarket->SchedulingTask();
	}
}
