#pragma once
#include"pch.h"

#ifndef __GOOGLEMOCK__
#error _T("本测试系统必须使用GOOGLE MOCK")
#endif

#include"globedef.h"
#include"ThreadStatus.h"
#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"MockWorldMarket.h"
#include"MockChinaMarket.h"

#include"MockMainFrm.h"

#include"MockSinaRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockFinnhubWebInquiry.h"
#include"MockTiingoWebInquiry.h"
#include"MockQuandlWebInquiry.h"
#include"MockChinaMarket.h"

using namespace testing;
// using testing::GTEST_FLAG(brief); // 此标识已实现，但在1.10.0中尚未允许使用。

#include"WebInquirer.h"

using namespace std;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	// 构造析构时开销大的Mock类声明为全局变量，在测试系统退出时才析构,这样容易在测试信息窗口中发现故障
	CMockWorldMarketPtr gl_pMockWorldMarket;
	CMockChinaMarketPtr gl_pMockChinaMarket;
	CMockMainFrame* gl_pMockMainFrame; // 此Mock类使用真实的各市场类(gl_pChinaMarket, gl_pWorldMarket, ...)

	class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
	public:
		TestEnvironment(void) {
		}
		virtual ~TestEnvironment() {
		}

		virtual void SetUp(void) override {
			ASSERT(!gl_fNormalMode);
			gl_GlobeOption.SetDefaultFileName(_T("C:\\Users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\systemOptionTest.json"));
			gl_GlobeOption.LoadDB();

			gl_pSinaRTWebInquiry = nullptr;
			gl_pTengxunRTWebInquiry = nullptr;
			gl_pNeteaseRTWebInquiry = nullptr;
			gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
			gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
			gl_pFinnhubWebInquiry = nullptr;
			gl_pFinnhubWebInquiry = nullptr;
			gl_pTiingoWebInquiry = nullptr;
			gl_pQuandlWebInquiry = nullptr;

			// 下列网络提取器皆使用Mock类，以防止出现实际的网络读取动作。
			gl_pSinaRTWebInquiry = make_shared<CMockSinaRTWebInquiry>();
			gl_pTengxunRTWebInquiry = make_shared<CMockTengxunRTWebInquiry>();
			gl_pNeteaseRTWebInquiry = make_shared<CMockNeteaseRTWebInquiry>();
			gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
			gl_pNeteaseDayLineWebInquiry2 = make_shared<CMockNeteaseDayLineWebInquiry>();
			gl_pFinnhubWebInquiry = make_shared<CMockFinnhubWebInquiry>();
			gl_pTiingoWebInquiry = make_shared<CMockTiingoWebInquiry>();
			gl_pQuandlWebInquiry = make_shared<CMockQuandlWebInquiry>();

			// 下列全局智能指针为实际类
			gl_pChinaMarket = make_shared<CChinaMarket>();
			gl_pWorldMarket = make_shared<CWorldMarket>();
			gl_pChinaMarket->ResetMarket();
			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pWorldMarket->ResetMarket();

			EXPECT_EQ(gl_vMarketPtr.size(), 0);
			gl_vMarketPtr.push_back(gl_pWorldMarket); // 美国股票市场
			gl_vMarketPtr.push_back(gl_pChinaMarket); // 中国股票市场

			gl_pMockChinaMarket = make_shared<CMockChinaMarket>();
			gl_pMockChinaMarket->ResetMarket();
			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pMockWorldMarket = make_shared<CMockWorldMarket>(); // 在此生成，在全局TearDown才赋值nullptr.这样容易看到错误信息
			gl_pMockWorldMarket->ResetMarket();
			EXPECT_LE(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());
			EXPECT_LE(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL);//
			gl_pMockMainFrame = new CMockMainFrame;
			EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL);//

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pChinaMarket->GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(__STOCK_IPOED__); // 修改活跃股票的IPO状态

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
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(__STOCK_IPOED__); // 修改活跃股票的IPO状态

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pMockChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"当股票日线结束日期早于30日时，装入股票代码数据库时要求更新代码库";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}

			if (gl_pChinaMarket->GetMarketDate() >= 20210531) { // 目前测试数据库的日线结束日期为20210430
				if (gl_pChinaMarket->GetDayOfWeek() == 1) { // 如果是星期一
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

			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();

			gl_pWorldMarket->GetStock(_T("AAPL"))->SetProfileUpdated(false);

			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// 这里要故意将几个Mock全局变量设置为nullptr，这样就能够在测试输出窗口（不是Test Expxplorer窗口）中得到测试结果。
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}

			gl_fExitingSystem = true;
			// 重置以下指针，以测试是否存在没有配对的Mock。
			gl_pMockChinaMarket = nullptr;
			gl_pMockWorldMarket = nullptr;

			if (CMFCVisualManager::GetInstance() != NULL) {
				delete CMFCVisualManager::GetInstance(); // 在生成MainFrame时，会生成一个视觉管理器。故而在此删除之。
			}

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				pStock->SetUpdateProfileDB(false); // gl_pMockMainFrame使用了真正的gl_pChinaMarket,此处重置此标识，防止解构gl_pMockMainFrame时更新数据库。
			}
			ASSERT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse()) << "退出时必须保证无需更新代码库";

			gl_fExitingSystem = false;
			delete gl_pMockMainFrame;
			EXPECT_TRUE(gl_fExitingSystem) << "MainFrame析构时设置此标识";

			gl_pSinaRTWebInquiry = nullptr;
			gl_pTengxunRTWebInquiry = nullptr;
			gl_pNeteaseRTWebInquiry = nullptr;
			gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
			gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
			gl_pFinnhubWebInquiry = nullptr;
			gl_pFinnhubWebInquiry = nullptr;
			gl_pTiingoWebInquiry = nullptr;
			gl_pQuandlWebInquiry = nullptr;

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			while (gl_ThreadStatus.IsWebInquiringThreadRunning()) Sleep(1);
			gl_pWorldMarket = nullptr;
			gl_pChinaMarket = nullptr;
			gl_vMarketPtr.clear();
		}
	};
}