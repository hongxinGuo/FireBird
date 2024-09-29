#include"pch.h"

#include "TengxunDayLineDataSource.h"
#include "TengxunRTDataSource.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include "GeneralCheck.h"

#include "FinnhubInaccessibleExchange.h"
#include "NeteaseDayLineDataSource.h"
#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	void SystemMessageCheck() {
		EXPECT_THAT(gl_systemMessage.InformationSize(), 0) << gl_systemMessage.PopInformationMessage();
		EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_THAT(gl_systemMessage.DayLineInfoSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 0) << gl_systemMessage.PopErrorMessage();
	}

	void EnvironmentCheck() {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_FALSE(gl_systemConfiguration.IsExitingSystem());
		time_t tt;
		time(&tt);
		EXPECT_TRUE((gl_tUTCTime + 3600) > tt) << gl_tUTCTime;
	}

	void SystemConfigurationCheck() {
		EXPECT_FALSE(gl_systemConfiguration.IsNeedUpdate()) << "���������ϵͳ����";
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsNeedUpdate()) << "��������½��뽻��������";
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseDayLineServer());
		if (gl_UpdateChinaMarketDB.try_acquire()) {
			gl_UpdateChinaMarketDB.release();
		}
		else {
			EXPECT_TRUE(false) << "gl_UpdateChinaMarketDB����ס��";
		}
		if (gl_UpdateWorldMarketDB.try_acquire()) {
			gl_UpdateWorldMarketDB.release();
		}
		else {
			EXPECT_TRUE(false) << "gl_UpdateWorldMarketDB����ס��";
		}

		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountName(), _T("FireBird"));
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountPassword(), _T("firebird"));
		EXPECT_EQ(gl_systemConfiguration.GetBackgroundThreadPermittedNumber(), 8);

		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0) << "Ĭ��ʹ������ʵʱ���ݷ�����";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketDayLineServer(), 0) << "Ĭ��ʹ�������������ݷ�����";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime(), 200) << "Ĭ�ϲ�ѯʱ��Ϊ200����";
		EXPECT_EQ(gl_systemConfiguration.GetSavingChinaMarketStockDayLineThread(), 4) << "Ĭ�ϲ�ѯ��Ʊ��ʷ���ݹ����߳���Ϊ4";
		EXPECT_EQ(gl_systemConfiguration.GetNumberOfRTDataSource(), 4) << "�����ļ��е���ֵ";
		EXPECT_EQ(gl_systemConfiguration.GetSinaRTDataInquiryPerTime(), 850) << "�����ļ��е���ֵ";
		EXPECT_EQ(gl_systemConfiguration.GetNeteaseRTDataInquiryPerTime(), 900) << "�����ļ��е���ֵ";
		EXPECT_EQ(gl_systemConfiguration.GetTengxunRTDataInquiryPerTime(), 900) << "�����ļ��е���ֵ";

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100) << "Ĭ��ÿ�β�ѯʱ��Ϊ1100����";
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "Ĭ��ÿСʱ��ѯ�������Ϊ400";
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "Ĭ��ÿСʱ��ѯ�������Ϊ100";

		EXPECT_EQ(gl_systemConfiguration.GetInsideTransactionUpdateRate(), 30);
		EXPECT_EQ(gl_systemConfiguration.GetStockProfileUpdateRate(), 365);
		EXPECT_EQ(gl_systemConfiguration.GetStockPeerUpdateRate(), 90);
		EXPECT_EQ(gl_systemConfiguration.GetStockBasicFinancialUpdateRate(), 45);
	}

	void ChinaMarketCheck() {
		if (gl_pChinaMarket != nullptr) {
			ASSERT_FALSE(gl_dataContainerChinaStock.IsUpdateProfileDB()) << "��������¹�Ʊ�����";
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();

			EXPECT_FALSE(gl_pChinaMarket->IsUpdateChosenStockDB());

			EXPECT_TRUE(gl_pChinaMarket->IsUsingSinaRTDataReceiver());
			EXPECT_TRUE(gl_pChinaMarket->IsUsingTengxunRTDataReceiver());
			EXPECT_FALSE(gl_pChinaMarket->IsUsingNeteaseRTDataReceiver());

			EXPECT_FALSE(gl_pChinaMarket->IsCalculateChosen10RS());
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
			EXPECT_TRUE(gl_pChinaMarket->IsCheckingActiveStock());
			EXPECT_FALSE(gl_systemConfiguration.IsFastInquiringRTData()) << "DEBUGģʽʱĬ��Ϊ��";
			EXPECT_FALSE(gl_pChinaMarket->IsMarketOpened());
			EXPECT_FALSE(gl_pChinaMarket->IsRTDataSetCleared());
			EXPECT_TRUE(gl_pChinaMarket->IsSavingTempData());
			EXPECT_FALSE(gl_pChinaMarket->IsUpdateOptionDB());
			EXPECT_FALSE(gl_pChinaMarket->IsWebBusy());
			EXPECT_FALSE(gl_pChinaMarket->IsDayLineNeedProcess());
			EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());
			EXPECT_FALSE(gl_pChinaMarket->IsResetting());
			EXPECT_THAT(gl_pChinaMarket->IsCalculatingDayLineRS(), IsFalse());
			EXPECT_THAT(gl_pChinaMarket->IsCalculatingWeekLineRS(), IsFalse());
			EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());

			EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5040);
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_EQ(gl_dataContainerChinaStock.Size(), 5040) << "�������ݿ�װ�صĹ�Ʊ��";

			EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

			EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());

			EXPECT_EQ(gl_dataContainerChinaStock.GetStock(0)->GetDayLineEndDate(), 20210430) << "��һ����Ʊ���Ϻ�ָ�����Ľ�������Ϊ20210430";
			EXPECT_TRUE(gl_dataContainerChinaStock.GetStock(0)->IsIPOed()) << "��һ����Ʊ���Ϻ�ָ����ΪIPOed��255��";
			const auto pStock = gl_dataContainerChinaStock.GetStock("000001.SS");
			ASSERT_TRUE(pStock->IsIPOed());

			EXPECT_TRUE(gl_qChinaMarketRTData.size_approx() == 0);

			//EXPECT_FALSE(gl_pChinaMarket->Is);
			//EXPECT_FALSE(gl_pChinaMarket->);
			//EXPECT_FALSE(gl_pChinaMarket->);
			//EXPECT_FALSE(gl_pChinaMarket->);
		}
	}

	void WorldMarketCheck() {
		if (gl_pWorldMarket != nullptr) {
			ASSERT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB()) << "��������¹�Ʊ�����";
			const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(_T("AAPL"));
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			ASSERT_FALSE(pStock->IsUpdateProfileDB()) << "��������¹�Ʊ�����";
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateDayLine());

			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15) << "���װ����15��";
			EXPECT_EQ(gl_dataContainerFinnhubForexExchange.Size(), 11) << "���װ����11��";

			EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());

			EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty()) << gl_pWorldMarket->GetMarketTask()->GetType();

			EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847) << "Ĭ��״̬�����ݿ�����Ϊ4847(ȫ���Ϻ���Ʊ��С����������Ʊ)";

			//	for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			//		const auto p_stock = gl_dataContainerFinnhubStock.GetStock(l);
			//		EXPECT_TRUE(p_stock->IsUpdateBasicFinancial());
			//		EXPECT_EQ(p_stock->GetBasicFinancialUpdateDate(), 19800101);
			//		EXPECT_FALSE(p_stock->IsUpdateProfileDB());
			//	}
		}
	}

	void WebInquirerCheck() {
		EXPECT_TRUE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pTengxunRTDataSource->IsEnable());

		EXPECT_THAT(gl_pFinnhubDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pTiingoDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pSinaRTDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pNeteaseDayLineDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pNeteaseRTDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pTengxunRTDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pTengxunDayLineDataSource->InquiryQueueSize(), 0);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_FALSE(gl_pSinaRTDataSource->IsWebError());
		EXPECT_FALSE(gl_pNeteaseDayLineDataSource->IsWebError());
		EXPECT_FALSE(gl_pSinaRTDataSource->IsWebError());
		EXPECT_FALSE(gl_pNeteaseRTDataSource->IsWebError());
		EXPECT_FALSE(gl_pTengxunRTDataSource->IsWebError());
		EXPECT_FALSE(gl_pTengxunDayLineDataSource->IsWebError());

		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCountryList());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockProfile());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexExchange());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoSymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexSymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateSymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdatePeer());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCompanyNews());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateInsiderSentiment());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateInsiderTransaction());

		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateStockSymbol());
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());

		EXPECT_EQ(gl_pTiingoDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pFinnhubDataSource->InquiryQueueSize(), 0);

		EXPECT_THAT(gl_pTiingoCryptoWebSocket->DataSize(), 0);
		EXPECT_THAT(gl_pTiingoForexWebSocket->DataSize(), 0);
		EXPECT_THAT(gl_pTiingoIEXWebSocket->DataSize(), 0);
	}

	void DataContainerCheck() {
		CWebRTDataPtr pRTData;
		CDayLineWebDataPtr pData;
		EXPECT_FALSE(gl_qChinaMarketRTData.try_dequeue(pRTData));
		EXPECT_FALSE(gl_qDayLine.try_dequeue(pData));
	}

	void WebSocketCheck() {
		EXPECT_FALSE(gl_pFinnhubWebSocket->IsError());
		EXPECT_FALSE(gl_pTiingoCryptoWebSocket->IsError());
		EXPECT_FALSE(gl_pTiingoForexWebSocket->IsError());
		EXPECT_FALSE(gl_pTiingoIEXWebSocket->IsError());
		//EXPECT_FALSE(gl_systemConfiguration.IsUsingFinnhubWebSocket());
		//EXPECT_FALSE(gl_systemConfiguration.IsUsingTiingoCryptoWebSocket());
		//EXPECT_FALSE(gl_systemConfiguration.IsUsingTiingoIEXWebSocket());
		//EXPECT_FALSE(gl_systemConfiguration.IsUsingTiingoForexWebSocket());
	}

	void GeneralCheck() {
		SCOPED_TRACE("");

		EnvironmentCheck();
		SystemConfigurationCheck();
		SystemMessageCheck();
		ChinaMarketCheck();
		WorldMarketCheck();
		WebInquirerCheck();
		DataContainerCheck();
		WebSocketCheck();
	}
}
