////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
//Note: 测试环境中使用了真实的gl_pChinaMarket、gl_pWorldMarket等变量，析构时需要将其状态恢复原状，以防止其更新相应的数据库。切记。
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

#include"GeneralCheck.h"

#include"ChinaMarket.h"
#include "dataBaseConnector.h"
#include"WorldMarket.h"

#include"simdjsonGetValue.h"

#include"Initialization.h"
#include"InaccessibleSymbol.h"
#include "TiingoDataSource.h"
#include"TimeConvert.h"

using namespace testing;

#ifdef _MBCS
#error "本系统使用UNICODE字符集"
#endif

#ifndef _UNICODE
#error "本系统使用UNICODE字符集"
#endif

#ifndef __GOOGLEMOCK__
#error "本系统必须使用GOOGLE MOCK"
#endif

#ifndef FMT_UNICODE
#error "fmt 使用unicode"
#endif

// 目前使用.runsettings文件来排除外部代码，不再使用ExcludeSourceFromCodeCoverage的模式。且ExcludeSourceFromCodeCoverage模式目前在C20标准下无法编译。

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	// 测试环境中使用了真实的gl_pChinaMarket、gl_pWorldMarket等变量，析构时需要将其状态恢复原状，以防止其更新相应的数据库。切记。
	class TestEnvironment : public Environment {
		// 全局初始化，由main()函数调用。
	public:
		TestEnvironment() = default;
		~TestEnvironment() override = default;

		void SetUp() override {
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			gl_tpNow = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());
			gl_pTimeZoneLocal = chrono::current_zone(); // 获取软件运行所在的当地时区

			CreateSimdjsonEmptyArray();

			SetMaxCurrencyLevel();

			InitializeLogSystem();

			//Warning: Resharper的UnitTest要运行程序才能找到所有的测试函数，结果这里产生副作用。工作目录为X64/Debug,如果目录中没有systemConfiguration.json文件时，
			// 程序就会生成一个新文件，导致下面的断言失败。目前先屏蔽掉这个断言
			//ASSERT_TRUE(gl_systemConfiguration.IsNeedUpdate()) << gl_systemConfiguration.GetConfigurationFileDirectoryAndName();
			//gl_systemConfiguration.NeedUpdate(false);
			gl_systemConfiguration.SetConfigurationFileDirectory(R"(C:\Users\hxguo\source\repos\FireBird\GoogleUnitTest\)");
			gl_systemConfiguration.SetDefaultFileName("systemConfigurationTest.json");
			ASSERT_EQ(gl_systemConfiguration.GetConfigurationFileDirectoryAndName(), R"(C:\Users\hxguo\source\repos\FireBird\GoogleUnitTest\systemConfigurationTest.json)");
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

			//EXPECT_TRUE(gl_dataContainerChinaStock.IsUpdateProfileDB());
			for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				if (pStock->GetDayLineEndDate() > 20251101) {
					pStock->SetDayLineEndDate(20250101);//Todo: 测试用，避免日线结束日期过晚,过一段时间后删除此行
					pStock->SetUpdateProfileDB(true);
				}
				pStock->SetUpdateDayLine(true);

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20250101) {
						pStock->SetUpdateProfileDB(false);
					}
				}
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
			}
			EXPECT_FALSE(gl_dataContainerChinaStock.IsUpdateProfileDB());

			for (size_t i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
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

			while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
			while (!gl_pWorldMarket->IsMarketTaskEmpty()) gl_pWorldMarket->DiscardCurrentMarketTask();

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			EXPECT_FALSE(gl_pWorldMarket->IsSystemReady()) << "市场默认为尚未准备好";
			gl_pWorldMarket->SetSystemReady(true);// 测试系统默认为准备好了
			const auto pStock1 = gl_dataContainerFinnhubStock.GetItem("AAPL");
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
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				EXPECT_TRUE(pStock->IsUpdateDayLine()) << pStock->GetSymbol();
			}

			gl_systemConfiguration.SetExitingSystem(true);

			for (size_t i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				EXPECT_FALSE(pStock->IsUpdateProfileDB());
				pStock->SetUpdateProfileDB(false);	//此处重置此标识，防止更新数据库。
			}
			ASSERT_THAT(gl_dataContainerChinaStock.IsUpdateProfileDB(), IsFalse()) << "退出时必须保证无需更新代码库";

			gl_systemConfiguration.SetExitingSystem(false);

			// 以下真实的数据指针需要主动赋值为nullptr
			gl_pWorldMarket = nullptr;
			gl_pChinaMarket = nullptr;
			gl_vMarket.clear();

			gl_pFinnhubWebSocket = nullptr;
			gl_pTiingoIEXWebSocket = nullptr;
			gl_pTiingoCryptoWebSocket = nullptr;
			gl_pTiingoForexWebSocket = nullptr;

			ASSERT_THAT(gl_finnhubInaccessibleExchange.IsUpdateDB(), IsFalse());
			ASSERT_THAT(gl_systemConfiguration.IsUpdateDB(), IsFalse());
		}
	};
}

using namespace FireBirdTest;

// 空数据
//Test_FinnhubWebData finnhubWebData0(0, "AAPL", R"({})");
// 无权访问数据
//Test_FinnhubWebData finnhubWebData1(1, "AAPL", R"({"error":"You don't have access to this resource."})");
// 空数据
//Test_FinnhubWebData finnhubWebData2(2, "AAPL", R"([])");

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UNICODE时，使用wWinMain作为入口函数。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
#include"windows.h"
#include"shellapi.h"

int WINAPI wWinMain(HINSTANCE HInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
	int argc = 0;
	wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argv == nullptr) {
		return -1;
	}

	GTEST_FLAG_SET(death_test_style, "fast");
	::InitGoogleTest(&argc, argv);

	//Note: 使用测试环境的数据库连接池，避免对正式环境的数据库造成影响。测试环境的数据库连接池在测试结束时会自动析构。
	InitSqlppMySQLConnectionPool("Test", "test", "stock_market_test", "localhost", 3306, 20, false); // Note:: 连接测试环境的数据库

	// gTest takes ownership of the TestEnvironment ptr - we don't delete it.
	AddGlobalTestEnvironment(new TestEnvironment);

	ASSERT(gl_systemConfiguration.IsWorkingMode()); // 预设为是实际系统。
	gl_systemConfiguration.SetWorkingMode(false); // 实际系统，测试状态为假。

	return RUN_ALL_TESTS();
}
