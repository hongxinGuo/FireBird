////////////////////////////////////////////////////////////////////////////////////
//
//
// ������ҪԤ���趨ȫ�ֳ�ʼ���������ʶ���ʹ��ϵͳ�ṩ��main()�����������Լ����塣
// ��Ҫ����һ��AddGlobalTestEnvironment()�������ã������趨ȫ�ֻ������˺������Զ��ʹ�á�
//
// ���Ի�����ʹ������ʵ��gl_pChinaMarket��gl_pWorldMarket�ȱ���������ʱ��Ҫ����״̬�ָ�ԭ״���Է�ֹ�������Ӧ�����ݿ⡣�мǡ�
//
// Ŀǰʹ��.runsettings�ļ����ų��ⲿ���룬����ʹ��ExcludeSourceFromCodeCoverage��ģʽ����ExcludeSourceFromCodeCoverageģʽĿǰ��C20��׼���޷����롣
//
/////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GoogleMockʹ�÷�����
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
#error _T("��ϵͳʹ�ö��ֽ��ַ���")
#endif

#ifndef __GOOGLEMOCK__
#error _T("��ϵͳ����ʹ��GOOGLE MOCK")
#endif

// IXWebSocket��ҪOpenSSL����ϵͳֱ��ʹ�á�
#ifdef _DEBUG
#pragma comment(lib, "/vc/x64/MTd/libcrypto.lib")
#pragma comment(lib, "/vc/x64/MTd/libssl.lib")
#pragma comment(lib, "/D/concurrencpp.lib")
#else
#pragma comment(lib, "/vc/x64/MT/libcrypto.lib")
#pragma comment(lib, "/vc/x64/MT/libssl.lib")
#pragma comment(lib, "/R/concurrencpp.lib")
#endif

// Ŀǰʹ��.runsettings�ļ����ų��ⲿ���룬����ʹ��ExcludeSourceFromCodeCoverage��ģʽ����ExcludeSourceFromCodeCoverageģʽĿǰ��C20��׼���޷����롣

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	// ��������ʱ�������Mock������Ϊȫ�ֱ������ڲ���ϵͳ�˳�ʱ������,���������ڲ�����Ϣ�����з��ֹ���
	CMockMainFramePtr gl_pMockMainFrame; // ��Mock��ʹ����ʵ�ĸ��г���(gl_pChinaMarket, gl_pWorldMarket, ...)

	// ���Ի�����ʹ������ʵ��gl_pChinaMarket��gl_pWorldMarket�ȱ���������ʱ��Ҫ����״̬�ָ�ԭ״���Է�ֹ�������Ӧ�����ݿ⡣�мǡ�
	class TestEnvironment : public Environment {
		// ȫ�ֳ�ʼ������main()�������á�
	public:
		TestEnvironment() = default;
		~TestEnvironment() override = default;

		void SetUp() override {
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			gl_tpNow = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());

			CreateSimdjsonEmptyArray();

			SetMaxCurrencyLevel();

			InitializeSpdlog();

			//Bug Resharper��UnitTestҪ���г�������ҵ����еĲ��Ժ��������������������á�����Ŀ¼ΪX64/Debug,���Ŀ¼��û��systemConfiguration.json�ļ�ʱ��
			// ����ͻ�����һ�����ļ�����������Ķ���ʧ�ܡ�Ŀǰ�����ε��������
			//ASSERT_TRUE(gl_systemConfiguration.IsNeedUpdate()) << gl_systemConfiguration.GetConfigurationFileDirectoryAndName();
			//gl_systemConfiguration.NeedUpdate(false);
			gl_systemConfiguration.SetConfigurationFileDirectory(_T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));
			gl_systemConfiguration.SetDefaultFileName(_T("systemConfigurationTest.json"));
			ASSERT_STREQ(gl_systemConfiguration.GetConfigurationFileDirectoryAndName(), _T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\systemConfigurationTest.json"));
			ASSERT_TRUE(gl_systemConfiguration.LoadDB()) << "ʹ��GoogleUnitTestĿ¼�е������ļ�";
			gl_finnhubInaccessibleExchange.LoadDB(); // ���¼��أ�ʹ�ò���Ŀ¼�е�json�ļ�
			gl_finnhubInaccessibleExchange.Update();

			InitializeMarkets();
			gl_pChinaMarket->ResetMarket();
			gl_pWorldMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			EXPECT_LE(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL); //
			gl_pMockMainFrame = make_shared<CMockMainFrame>();
			EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL) << "������MainFrameʱ��������һ���Ӿ������������˳�ʱ��Ҫɾ��֮";

			EXPECT_TRUE(gl_dataContainerChinaStock.IsUpdateProfileDB());
			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				pStock->SetUpdateDayLine(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // �޸Ļ�Ծ��Ʊ��IPO״̬

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"����Ʊ���߽�����������30��ʱ��װ���Ʊ�������ݿ�ʱҪ����´����";
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
			EXPECT_FALSE(gl_pChinaMarket->IsSystemReady()) << "�г�Ĭ��Ϊ��δ׼����";
			gl_pChinaMarket->SetSystemReady(true); // ����ϵͳĬ��Ϊ׼������
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
			while (!gl_pWorldMarket->IsMarketTaskEmpty()) gl_pWorldMarket->DiscardCurrentMarketTask();

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			EXPECT_FALSE(gl_pWorldMarket->IsSystemReady()) << "�г�Ĭ��Ϊ��δ׼����";
			gl_pWorldMarket->SetSystemReady(true);// ����ϵͳĬ��Ϊ׼������
			const auto pStock1 = gl_dataContainerFinnhubStock.GetStock(_T("AAPL"));
			pStock1->SetUpdateCompanyProfile(true);

			// ���Ԥװ���finnhubInaccessibleExchange
			gl_finnhubInaccessibleExchange.Clear();

			gl_pTiingoDataSource->SetUpdateIEXTopOfBook(false); // Ĭ��Ϊ�����˻�

			gl_systemConfiguration.SetUpdateDB(false);
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// ����Ҫ���⽫����Mockȫ�ֱ�������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Explorer���ڣ��еõ����Խ����
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
				delete CMFCVisualManager::GetInstance(); // ������gl_pMockMainFrameʱ��������һ���Ӿ����������ʶ��ڴ�ɾ��֮��
			}

			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				const auto pStock = gl_dataContainerChinaStock.GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				pStock->SetUpdateProfileDB(false);	// gl_pMockMainFrameʹ����������gl_pChinaMarket,�˴����ô˱�ʶ����ֹ�⹹gl_pMockMainFrameʱ�������ݿ⡣
			}
			ASSERT_THAT(gl_dataContainerChinaStock.IsUpdateProfileDB(), IsFalse()) << "�˳�ʱ���뱣֤������´����";

			gl_systemConfiguration.SetExitingSystem(false);
			gl_pMockMainFrame = nullptr;
			EXPECT_TRUE(gl_systemConfiguration.IsExitingSystem()) << "MainFrame����ʱ���ô˱�ʶ";

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);

			// ������ʵ������ָ����Ҫ������ֵΪnullptr
			gl_pWorldMarket = nullptr;
			gl_pChinaMarket = nullptr;
			gl_vMarket.clear();

			gl_pFinnhubWebSocket = nullptr;
			gl_pTiingoIEXWebSocket = nullptr;
			gl_pTiingoCryptoWebSocket = nullptr;
			gl_pTiingoForexWebSocket = nullptr;

			// ������finnhubInaccessibleExchange�ļ�
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);
			// ������systemConfiguration�ļ�
			gl_systemConfiguration.SetUpdateDB(false);
		}
	};
}

using namespace FireBirdTest;

// ������
Test_FinnhubWebData finnhubWebData0(0, _T("AAPL"), _T("{}"));
// ��Ȩ��������
Test_FinnhubWebData finnhubWebData1(1, _T("AAPL"), _T("{\"error\":\"You don't have access to this resource.\"}"));
// ������
Test_FinnhubWebData finnhubWebData2(2, _T("AAPL"), _T("[]"));

int main(int argc, char* argv[]) {
	InitGoogleTest(&argc, argv);
	// gTest takes ownership of the TestEnvironment ptr - we don't delete it.
	AddGlobalTestEnvironment(new TestEnvironment);

	ASSERT(gl_systemConfiguration.IsWorkingMode()); // Ԥ��Ϊ��ʵ��ϵͳ��
	gl_systemConfiguration.SetWorkingMode(false); // ʵ��ϵͳ������״̬Ϊ�١�

	GTEST_FLAG_SET(death_test_style, "fast");

	return RUN_ALL_TESTS();
}
