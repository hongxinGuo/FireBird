////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
// 测试环境中使用了真实的gl_pChinaMarket、gl_pWorldMarket等变量，析构时需要将其状态恢复原状，以防止其更新相应的数据库。切记。
//
/////////////////////////////////////////////////////////////////////////////////////

// GoogleMock使用方法：
//
//   ON_CALL(mock_object, method_name(matchers...))
//     .With(multi_argument_matcher)  // Can be used at most once
//     .WillByDefault(action);        // Required
//
// where With() is optional and WillByDefault() must appear exactly
// once.
//
// EXPECT_CALL(mock_object, method_name(matchers...))
//   .With(multi_argument_matcher)  // Can be used at most once
//   .Times(cardinality)            // Can be used at most once
//   .InSequence(sequences...)      // Can be used any number of times
//   .After(expectations...)        // Can be used any number of times
//   .WillOnce(action)              // Can be used any number of times
//   .WillRepeatedly(action)        // Can be used at most once
//   .RetiresOnSaturation();        // Can be used at most once
//
// where all clauses are optional and WillOnce() can be repeated.
//
#include"pch.h"

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

#include"WorldMarket.h"

#include"MockMainFrm.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"
#include"QuandlDataSource.h"

using namespace testing;

#ifndef _MBCS
#error _T("本系统使用多字节字符集")
#endif

#ifndef __GOOGLEMOCK__
#error _T("本系统必须使用GOOGLE MOCK")
#endif

// 目前使用.runsettings文件来排除外部代码，不再使用ExcludeSourceFromCodeCoverage的模式。且ExcludeSourceFromCodeCoverage模式目前在C20标准下无法编译。

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	// 构造析构时开销大的Mock类声明为全局变量，在测试系统退出时才析构,这样容易在测试信息窗口中发现故障
	CMockMainFramePtr gl_pMockMainFrame; // 此Mock类使用真实的各市场类(gl_pChinaMarket, gl_pWorldMarket, ...)

	// 测试环境中使用了真实的gl_pChinaMarket、gl_pWorldMarket等变量，析构时需要将其状态恢复原状，以防止其更新相应的数据库。切记。
	class TestEnvironment : public Environment {
		// 全局初始化，由main()函数调用。
	public:
		TestEnvironment() = default;

		~TestEnvironment() override = default;

		void SetUp() override {
			ASSERT(!gl_systemConfiguration.IsWorkingMode());
			time(&gl_tUTCTime);

			ASSERT_STREQ(gl_systemConfiguration.GetDefaultFileDirectoryAndName(), _T("C:\\FireBird\\SystemConfiguration.json"));
			gl_systemConfiguration.SetDefaultFileDirectory(_T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));
			gl_systemConfiguration.SetDefaultFileName(_T("systemConfigurationTest.json"));
			ASSERT_STREQ(gl_systemConfiguration.GetDefaultFileDirectoryAndName(), _T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\systemConfigurationTest.json"));
			gl_systemConfiguration.LoadDB();
			gl_systemConfiguration.Update();

			// WebSocket要在gl_pWorldMarket之前生成
			gl_pFinnhubWebSocket = make_shared<CFinnhubWebSocket>();
			gl_pTiingoIEXWebSocket = make_shared<CTiingoIEXWebSocket>();
			gl_pTiingoCryptoWebSocket = make_shared<CTiingoCryptoWebSocket>();
			gl_pTiingoForexWebSocket = make_shared<CTiingoForexWebSocket>();

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

			EXPECT_LE(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL); //
			gl_pMockMainFrame = make_shared<CMockMainFrame>();
			EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL) << "在生成MainFrame时，会生成一个视觉管理器。在退出时需要删除之";

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // 修改活跃股票的IPO状态

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"当股票日线结束日期早于30日时，装入股票代码数据库时要求更新代码库";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_GT(gl_pChinaMarket->GetTotalStock(), 4800);
			EXPECT_FALSE(gl_pChinaMarket->IsSystemReady()) << "市场默认为尚未准备好";
			gl_pChinaMarket->SetSystemReady(true); // 测试系统默认为准备好了
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
			while (!gl_pWorldMarket->IsMarketTaskEmpty()) gl_pWorldMarket->DiscardCurrentMarketTask();

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			EXPECT_FALSE(gl_pWorldMarket->IsSystemReady()) << "市场默认为尚未准备好";
			gl_pWorldMarket->SetSystemReady(true);// 测试系统默认为准备好了
			const auto pStock1 = gl_pWorldMarket->GetStock(_T("AAPL"));
			pStock1->SetUpdateCompanyProfile(true);

			// 清空预装入的finnhubInaccessibleExchange
			gl_finnhubInaccessibleExchange.Clear();

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// 这里要故意将几个Mock全局变量设置为nullptr，这样就能够在测试输出窗口（不是Test Explorer窗口）中得到测试结果。
			SCOPED_TRACE("");
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}

			gl_systemConfiguration.SetExitingSystem(true);

			if (CMFCVisualManager::GetInstance() != nullptr) {
				delete CMFCVisualManager::GetInstance(); // 在生成gl_pMockMainFrame时，会生成一个视觉管理器。故而在此删除之。
			}

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				pStock->SetUpdateProfileDB(false);	// gl_pMockMainFrame使用了真正的gl_pChinaMarket,此处重置此标识，防止解构gl_pMockMainFrame时更新数据库。
			}
			ASSERT_THAT(gl_pChinaMarket->IsUpdateStockProfileDB(), IsFalse()) << "退出时必须保证无需更新代码库";

			gl_systemConfiguration.SetExitingSystem(false);
			gl_pMockMainFrame = nullptr;
			EXPECT_TRUE(gl_systemConfiguration.IsExitingSystem()) << "MainFrame析构时设置此标识";

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			while (gl_ThreadStatus.IsWebInquiringThreadRunning()) Sleep(1);

			gl_pWorldMarket = nullptr;
			gl_pChinaMarket = nullptr;
			gl_vMarketPtr.clear();

			gl_pFinnhubWebSocket = nullptr;
			gl_pTiingoIEXWebSocket = nullptr;
			gl_pTiingoCryptoWebSocket = nullptr;
			gl_pTiingoForexWebSocket = nullptr;

			// 不更新finnhubInaccessibleExchange文件
			gl_finnhubInaccessibleExchange.SetUpdate(false);
		}
	};
}

using namespace FireBirdTest;

// 空数据
Test_FinnhubWebData finnhubWebData0(0, _T("AAPL"), _T("{}"));
// 无权访问数据
Test_FinnhubWebData finnhubWebData1(1, _T("AAPL"), _T("{\"error\":\"You don't have access to this resource.\"}"));

int main(int argc, char* argv[]) {
	InitGoogleTest(&argc, argv);
	// gTest takes ownership of the TestEnvironment ptr - we don't delete it.
	AddGlobalTestEnvironment(new TestEnvironment);

	ASSERT(!gl_systemConfiguration.IsWorkingMode());

	return RUN_ALL_TESTS();
}
