#pragma once

#ifndef __GOOGLEMOCK__
#error _T("本测试系统必须使用GOOGLE MOCK")
#endif

#include"FinnhubInaccessibleExchange.h"
#include"ThreadStatus.h"

#include"GeneralCheck.h"
#include"TimeConvert.h"

#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include "TengxunDayLineDataSource.h"
#include"NeteaseRTDataSource.h"
#include"NeteaseDayLineDataSource.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"MockWorldMarket.h"
#include"MockChinaMarket.h"

#include"MockMainFrm.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"
#include"QuandlDataSource.h"

using namespace testing;

#include<memory>

namespace FireBirdTest {
	// 构造析构时开销大的Mock类声明为全局变量，在测试系统退出时才析构,这样容易在测试信息窗口中发现故障
	CMockWorldMarketPtr gl_pMockWorldMarket;
	CMockChinaMarketPtr gl_pMockChinaMarket;
	CMockMainFramePtr gl_pMockMainFrame; // 此Mock类使用真实的各市场类(gl_pChinaMarket, gl_pWorldMarket, ...)

	class TestEnvironment : public Environment {
		// 全局初始化，由main()函数调用。
	public:
		TestEnvironment(void) = default;

		~TestEnvironment() override = default;

		void SetUp(void) override {
			ASSERT(!gl_systemStatus.IsWorkingMode());

			ASSERT_STREQ(gl_systemConfiguration.GetDefaultFileDirectoryAndName(),
			             _T("C:\\FireBird\\SystemConfiguration.json"));
			gl_systemConfiguration.SetDefaultFileDirectory(
				_T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));
			gl_systemConfiguration.SetDefaultFileName(_T("systemConfigurationTest.json"));
			ASSERT_STREQ(gl_systemConfiguration.GetDefaultFileDirectoryAndName(),
			             _T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\systemConfigurationTest.json"));
			gl_systemConfiguration.LoadDB();
			gl_systemConfiguration.Update();

			gl_pSinaRTDataSource = make_shared<CSinaRTDataSource>();
			gl_pTengxunRTDataSource = make_shared<CTengxunRTDataSource>();
			gl_pTengxunDayLineDataSource = make_shared<CTengxunDayLineDataSource>();
			gl_pNeteaseRTDataSource = make_shared<CNeteaseRTDataSource>();
			gl_pNeteaseDayLineDataSource = make_shared<CNeteaseDayLineDataSource>();

			gl_pFinnhubDataSource = make_shared<CFinnhubDataSource>();
			gl_pTiingoDataSource = make_shared<CTiingoDataSource>();
			gl_pQuandlDataSource = make_shared<CQuandlDataSource>();

			// 下列全局智能指针为实际类
			gl_pChinaMarket = make_shared<CChinaMarket>();
			gl_pWorldMarket = make_shared<CWorldMarket>();
			gl_pChinaMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pWorldMarket->ResetMarket();

			EXPECT_EQ(gl_vMarketPtr.size(), 0);
			gl_vMarketPtr.push_back(gl_pWorldMarket); // 美国股票市场
			gl_vMarketPtr.push_back(gl_pChinaMarket); // 中国股票市场

			gl_pMockChinaMarket = make_shared<CMockChinaMarket>();
			gl_pMockChinaMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pMockWorldMarket = make_shared<CMockWorldMarket>(); // 在此生成，在全局TearDown才赋值nullptr.这样容易看到错误信息
			gl_pMockWorldMarket->ResetMarket();
			EXPECT_LE(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());
			EXPECT_LE(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL); //
			gl_pMockMainFrame = make_shared<CMockMainFrame>();
			EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL) << "在生成MainFrame时，会生成一个视觉管理器。在退出时需要删除之";

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pChinaMarket->GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // 修改活跃股票的IPO状态

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"当股票日线结束日期早于30日时，装入股票代码数据库时要求更新代码库";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}

			for (int i = 0; i < gl_pMockChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pMockChinaMarket->GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // 修改活跃股票的IPO状态

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pMockChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"当股票日线结束日期早于30日时，装入股票代码数据库时要求更新代码库";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}

			if (gl_pChinaMarket->GetMarketDate() >= 20210531) {
				// 目前测试数据库的日线结束日期为20210430
				if (gl_pChinaMarket->GetDayOfWeek() == 1) {
					// 如果是星期一
					EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate()) << "每星期一重新检查摘牌股票";
					EXPECT_TRUE(gl_pMockChinaMarket->TooManyStockDayLineNeedUpdate()) << "每星期一重新检查摘牌股票";
				}
				else {
					EXPECT_FALSE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate()) << "所有的活跃股票都被视为摘牌了";
					EXPECT_FALSE(gl_pMockChinaMarket->TooManyStockDayLineNeedUpdate()) << "所有的活跃股票都被视为摘牌了";
				}
				gl_pChinaMarket->SetStockDayLineNeedUpdate(gl_pChinaMarket->GetTotalStock()); // 伪装此标识
				gl_pMockChinaMarket->SetStockDayLineNeedUpdate(gl_pChinaMarket->GetTotalStock()); // 伪装此标识
			}
			else {
				EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());
				EXPECT_TRUE(gl_pMockChinaMarket->TooManyStockDayLineNeedUpdate());
			}

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());
			EXPECT_GT(gl_pChinaMarket->GetTotalStock(), 4800);
			EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());
			gl_pChinaMarket->SetSystemReady(true);
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			auto pStock1 = gl_pWorldMarket->GetStock(_T("AAPL"));
			pStock1->SetUpdateCompanyProfile(true);

			// 清空预装入的finnhubInaccessibleExchange
			gl_finnhubInaccessibleExchange.Clear();

			GeneralCheck();
		}

		void TearDown(void) override {
			// 这里要故意将几个Mock全局变量设置为nullptr，这样就能够在测试输出窗口（不是Test Expxplorer窗口）中得到测试结果。
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}

			gl_systemStatus.SetExitingSystem(true);
			// 重置以下指针，以测试是否存在没有配对的Mock。
			gl_pMockChinaMarket = nullptr;
			gl_pMockWorldMarket = nullptr;

			if (CMFCVisualManager::GetInstance() != nullptr) {
				delete CMFCVisualManager::GetInstance(); // 在生成MainFrame时，会生成一个视觉管理器。故而在此删除之。
			}

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				pStock->SetUpdateProfileDB(false);
				// gl_pMockMainFrame使用了真正的gl_pChinaMarket,此处重置此标识，防止解构gl_pMockMainFrame时更新数据库。
			}
			ASSERT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse()) << "退出时必须保证无需更新代码库";

			gl_systemStatus.SetExitingSystem(false);
			gl_pMockMainFrame = nullptr;
			EXPECT_TRUE(gl_systemStatus.IsExitingSystem()) << "MainFrame析构时设置此标识";

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			while (gl_ThreadStatus.IsWebInquiringThreadRunning()) Sleep(1);
			gl_pWorldMarket = nullptr;
			gl_pChinaMarket = nullptr;
			gl_vMarketPtr.clear();

			// 不更新finnhubInaccessibleExchange文件
			gl_finnhubInaccessibleExchange.SetUpdate(false);
		}
	};
}
