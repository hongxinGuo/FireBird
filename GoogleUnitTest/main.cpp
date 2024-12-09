////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
// 测试环境中使用了真实的gl_pChinaMarket、gl_pWorldMarket等变量，析构时需要将其状态恢复原状，以防止其更新相应的数据库。切记。
//
// 目前使用.runsettings文件来排除外部代码，不再使用ExcludeSourceFromCodeCoverage的模式。且ExcludeSourceFromCodeCoverage模式目前在C20标准下无法编译。
//
/////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

////#include"gtest/gtest.h"

#include"globedef.h"

#include <spdlog/sinks/basic_file_sink.h>

import FireBird.System.Message;

#include"GeneralCheck.h"

#include"ChinaMarket.h"
#include"WorldMarket.h"

import FireBird.WindowInterface.MainFrm;
#include"MockMainFrm.h"

import  FireBird.Simdjson.GetValue;

import FireBird.System.ScheduleTask;
import FireBird.System.FinnhubInaccessibleExchange;
import FireBird.Accessory.InfoReport;
import FireBird.System.ThreadStatus;
#include "TiingoDataSource.h"
import FireBird.Accessory.TimeConvert;

using namespace testing;

#ifndef _MBCS
#error _T("本系统使用多字节字符集")
#endif

#ifndef __GOOGLEMOCK__
#error _T("本系统必须使用GOOGLE MOCK")
#endif

// IXWebSocket需要OpenSSL，本系统直接使用。
#ifdef _DEBUG
#pragma comment(lib, "/vc/x64/MTd/libcrypto.lib")
#pragma comment(lib, "/vc/x64/MTd/libssl.lib")
#pragma comment(lib, "/D/concurrencpp.lib")
#else
#pragma comment(lib, "/vc/x64/MT/libcrypto.lib")
#pragma comment(lib, "/vc/x64/MT/libssl.lib")
#pragma comment(lib, "/R/concurrencpp.lib")
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
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			gl_tpNow = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());

			CreateSimdjsonEmptyArray();

			SetMaxCurrencyLevel();

			InitializeSpdlog();

			//Bug Resharper的UnitTest要运行程序才能找到所有的测试函数，结果这里产生副作用。工作目录为X64/Debug,如果目录中没有systemConfiguration.json文件时，
			// 程序就会生成一个新文件，导致下面的断言失败。目前先屏蔽掉这个断言
			//ASSERT_TRUE(gl_systemConfiguration.IsNeedUpdate()) << gl_systemConfiguration.GetConfigurationFileDirectoryAndName();
			//gl_systemConfiguration.NeedUpdate(false);
			gl_systemConfiguration.SetConfigurationFileDirectory(_T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));
			gl_systemConfiguration.SetDefaultFileName(_T("systemConfigurationTest.json"));
			ASSERT_STREQ(gl_systemConfiguration.GetConfigurationFileDirectoryAndName(), _T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\systemConfigurationTest.json"));
			ASSERT_TRUE(gl_systemConfiguration.LoadDB()) << "使用GoogleUnitTest目录中的配置文件";
			gl_finnhubInaccessibleExchange.LoadDB(); // 重新加载，使用测试目录中的json文件
			gl_finnhubInaccessibleExchange.Update();

			InitializeMarkets();
			gl_pChinaMarket->ResetMarket();
			gl_pWorldMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			EXPECT_LE(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL); //
			gl_pMockMainFrame = make_shared<CMockMainFrame>();
			EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL) << "在生成MainFrame时，会生成一个视觉管理器。在退出时需要删除之";

			EXPECT_TRUE(gl_dataContainerChinaStock.IsUpdateProfileDB());
			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				pStock->SetUpdateDayLine(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // 修改活跃股票的IPO状态

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"当股票日线结束日期早于30日时，装入股票代码数据库时要求更新代码库";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}
			EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateProfileDB());

			for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
				auto pStock = gl_dataContainerTiingoStock.GetStock(i);
				pStock->SetUpdateProfileDB(false);
				pStock->SetUpdateDayLineDB(false);
				pStock->SetUpdateDayLine(true);
			}
			EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());

			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_GT(gl_dataContainerChinaStock.Size(), 4800);
			EXPECT_FALSE(gl_pChinaMarket->IsSystemReady()) << "市场默认为尚未准备好";
			gl_pChinaMarket->SetSystemReady(true); // 测试系统默认为准备好了
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
			while (!gl_pWorldMarket->IsMarketTaskEmpty()) gl_pWorldMarket->DiscardCurrentMarketTask();

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			EXPECT_FALSE(gl_pWorldMarket->IsSystemReady()) << "市场默认为尚未准备好";
			gl_pWorldMarket->SetSystemReady(true);// 测试系统默认为准备好了
			const auto pStock1 = gl_dataContainerFinnhubStock.GetStock(_T("AAPL"));
			pStock1->SetUpdateCompanyProfile(true);

			// 清空预装入的finnhubInaccessibleExchange
			gl_finnhubInaccessibleExchange.Clear();

			gl_pTiingoDataSource->SetUpdateIEXTopOfBook(false); // 默认为付费账户

			gl_systemConfiguration.SetUpdateDB(false);
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// 这里要故意将几个Mock全局变量设置为nullptr，这样就能够在测试输出窗口（不是Test Explorer窗口）中得到测试结果。
			SCOPED_TRACE("");
			GeneralCheck();

			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				EXPECT_TRUE(pStock->IsUpdateDayLine()) << pStock->GetSymbol();
			}

			gl_systemConfiguration.SetExitingSystem(true);

			if (CMFCVisualManager::GetInstance() != nullptr) {
				delete CMFCVisualManager::GetInstance(); // 在生成gl_pMockMainFrame时，会生成一个视觉管理器。故而在此删除之。
			}

			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				pStock->SetUpdateProfileDB(false);	// gl_pMockMainFrame使用了真正的gl_pChinaMarket,此处重置此标识，防止解构gl_pMockMainFrame时更新数据库。
			}
			ASSERT_THAT(gl_dataContainerChinaStock.IsUpdateProfileDB(), IsFalse()) << "退出时必须保证无需更新代码库";

			gl_systemConfiguration.SetExitingSystem(false);
			gl_pMockMainFrame = nullptr;
			EXPECT_TRUE(gl_systemConfiguration.IsExitingSystem()) << "MainFrame析构时设置此标识";

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);

			// 以下真实的数据指针需要主动赋值为nullptr
			gl_pWorldMarket = nullptr;
			gl_pChinaMarket = nullptr;
			gl_vMarket.clear();

			gl_pFinnhubWebSocket = nullptr;
			gl_pTiingoIEXWebSocket = nullptr;
			gl_pTiingoCryptoWebSocket = nullptr;
			gl_pTiingoForexWebSocket = nullptr;

			// 不更新finnhubInaccessibleExchange文件
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);
			// 不更新systemConfiguration文件
			gl_systemConfiguration.SetUpdateDB(false);
		}
	};
}

using namespace FireBirdTest;

// 空数据
Test_FinnhubWebData finnhubWebData0(0, _T("AAPL"), _T("{}"));
// 无权访问数据
Test_FinnhubWebData finnhubWebData1(1, _T("AAPL"), _T("{\"error\":\"You don't have access to this resource.\"}"));
// 空数据
Test_FinnhubWebData finnhubWebData2(2, _T("AAPL"), _T("[]"));

int main(int argc, char* argv[]) {
	InitGoogleTest(&argc, argv);
	// gTest takes ownership of the TestEnvironment ptr - we don't delete it.
	AddGlobalTestEnvironment(new TestEnvironment);

	ASSERT(gl_systemConfiguration.IsWorkingMode()); // 预设为是实际系统。
	gl_systemConfiguration.SetWorkingMode(false); // 实际系统，测试状态为假。

	GTEST_FLAG_SET(death_test_style, "fast");

	return RUN_ALL_TESTS();
}
