////////////////////////////////////////////////////////////////////////////////////
//
//
// ������ҪԤ���趨ȫ�ֳ�ʼ���������ʶ���ʹ��ϵͳ�ṩ��main()�����������Լ����塣
// ��Ҫ����һ��AddGlobalTestEnvironment()�������ã������趨ȫ�ֻ������˺������Զ��ʹ�á�
//
// ���Ի�����ʹ������ʵ��gl_pChinaMarket��gl_pWorldMarket�ȱ���������ʱ��Ҫ����״̬�ָ�ԭ״���Է�ֹ�������Ӧ�����ݿ⡣�мǡ�
//
/////////////////////////////////////////////////////////////////////////////////////

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
#error _T("��ϵͳʹ�ö��ֽ��ַ���")
#endif

#ifndef __GOOGLEMOCK__
#error _T("��ϵͳ����ʹ��GOOGLE MOCK")
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
			ASSERT(!gl_systemConfiguration.IsWorkingMode());
			time(&gl_tUTCTime);

			ASSERT_STREQ(gl_systemConfiguration.GetDefaultFileDirectoryAndName(), _T("C:\\FireBird\\SystemConfiguration.json"));
			gl_systemConfiguration.SetDefaultFileDirectory(_T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\"));
			gl_systemConfiguration.SetDefaultFileName(_T("systemConfigurationTest.json"));
			ASSERT_STREQ(gl_systemConfiguration.GetDefaultFileDirectoryAndName(), _T("C:\\Users\\hxguo\\source\\repos\\FireBird\\GoogleUnitTest\\systemConfigurationTest.json"));
			gl_systemConfiguration.LoadDB();
			gl_systemConfiguration.Update();

			// WebSocketҪ��gl_pWorldMarket֮ǰ����
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

			// ����ȫ������ָ��Ϊʵ����
			gl_pChinaMarket = make_shared<CChinaMarket>();
			gl_pWorldMarket = make_shared<CWorldMarket>();
			gl_pChinaMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pWorldMarket->ResetMarket();

			EXPECT_EQ(gl_vMarketPtr.size(), 0);
			gl_vMarketPtr.push_back(gl_pWorldMarket); // ������Ʊ�г�
			gl_vMarketPtr.push_back(gl_pChinaMarket); // �й���Ʊ�г�

			EXPECT_LE(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL); //
			gl_pMockMainFrame = make_shared<CMockMainFrame>();
			EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL) << "������MainFrameʱ��������һ���Ӿ������������˳�ʱ��Ҫɾ��֮";

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // �޸Ļ�Ծ��Ʊ��IPO״̬

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"����Ʊ���߽�����������30��ʱ��װ���Ʊ�������ݿ�ʱҪ����´����";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_GT(gl_pChinaMarket->GetTotalStock(), 4800);
			EXPECT_FALSE(gl_pChinaMarket->IsSystemReady()) << "�г�Ĭ��Ϊ��δ׼����";
			gl_pChinaMarket->SetSystemReady(true); // ����ϵͳĬ��Ϊ׼������
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

			while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
			while (!gl_pWorldMarket->IsMarketTaskEmpty()) gl_pWorldMarket->DiscardCurrentMarketTask();

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			EXPECT_FALSE(gl_pWorldMarket->IsSystemReady()) << "�г�Ĭ��Ϊ��δ׼����";
			gl_pWorldMarket->SetSystemReady(true);// ����ϵͳĬ��Ϊ׼������
			const auto pStock1 = gl_pWorldMarket->GetStock(_T("AAPL"));
			pStock1->SetUpdateCompanyProfile(true);

			// ���Ԥװ���finnhubInaccessibleExchange
			gl_finnhubInaccessibleExchange.Clear();

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// ����Ҫ���⽫����Mockȫ�ֱ�������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Explorer���ڣ��еõ����Խ����
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
				delete CMFCVisualManager::GetInstance(); // ������gl_pMockMainFrameʱ��������һ���Ӿ����������ʶ��ڴ�ɾ��֮��
			}

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				pStock->SetUpdateProfileDB(false);	// gl_pMockMainFrameʹ����������gl_pChinaMarket,�˴����ô˱�ʶ����ֹ�⹹gl_pMockMainFrameʱ�������ݿ⡣
			}
			ASSERT_THAT(gl_pChinaMarket->IsUpdateStockProfileDB(), IsFalse()) << "�˳�ʱ���뱣֤������´����";

			gl_systemConfiguration.SetExitingSystem(false);
			gl_pMockMainFrame = nullptr;
			EXPECT_TRUE(gl_systemConfiguration.IsExitingSystem()) << "MainFrame����ʱ���ô˱�ʶ";

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

			// ������finnhubInaccessibleExchange�ļ�
			gl_finnhubInaccessibleExchange.SetUpdate(false);
		}
	};
}

using namespace FireBirdTest;

// ������
Test_FinnhubWebData finnhubWebData0(0, _T("AAPL"), _T("{}"));
// ��Ȩ��������
Test_FinnhubWebData finnhubWebData1(1, _T("AAPL"), _T("{\"error\":\"You don't have access to this resource.\"}"));

int main(int argc, char* argv[]) {
	InitGoogleTest(&argc, argv);
	// gTest takes ownership of the TestEnvironment ptr - we don't delete it.
	AddGlobalTestEnvironment(new TestEnvironment);

	ASSERT(!gl_systemConfiguration.IsWorkingMode());

	return RUN_ALL_TESTS();
}
