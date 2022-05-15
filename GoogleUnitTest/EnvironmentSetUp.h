#pragma once
#include"pch.h"

#ifndef __GOOGLEMOCK__
#error _T("������ϵͳ����ʹ��GOOGLE MOCK")
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
// using testing::GTEST_FLAG(brief); // �˱�ʶ��ʵ�֣�����1.10.0����δ����ʹ�á�

#include"WebInquirer.h"

using namespace std;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	// ��������ʱ�������Mock������Ϊȫ�ֱ������ڲ���ϵͳ�˳�ʱ������,���������ڲ�����Ϣ�����з��ֹ���
	CMockWorldMarketPtr gl_pMockWorldMarket;
	CMockChinaMarketPtr gl_pMockChinaMarket;
	CMockMainFrame* gl_pMockMainFrame; // ��Mock��ʹ����ʵ�ĸ��г���(gl_pChinaMarket, gl_pWorldMarket, ...)

	class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
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
			gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
			gl_pNeteaseDayLineWebInquiry2 = nullptr; // ����������ʷ����
			gl_pFinnhubWebInquiry = nullptr;
			gl_pFinnhubWebInquiry = nullptr;
			gl_pTiingoWebInquiry = nullptr;
			gl_pQuandlWebInquiry = nullptr;

			// ����������ȡ����ʹ��Mock�࣬�Է�ֹ����ʵ�ʵ������ȡ������
			gl_pSinaRTWebInquiry = make_shared<CMockSinaRTWebInquiry>();
			gl_pTengxunRTWebInquiry = make_shared<CMockTengxunRTWebInquiry>();
			gl_pNeteaseRTWebInquiry = make_shared<CMockNeteaseRTWebInquiry>();
			gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
			gl_pNeteaseDayLineWebInquiry2 = make_shared<CMockNeteaseDayLineWebInquiry>();
			gl_pFinnhubWebInquiry = make_shared<CMockFinnhubWebInquiry>();
			gl_pTiingoWebInquiry = make_shared<CMockTiingoWebInquiry>();
			gl_pQuandlWebInquiry = make_shared<CMockQuandlWebInquiry>();

			// ����ȫ������ָ��Ϊʵ����
			gl_pChinaMarket = make_shared<CChinaMarket>();
			gl_pWorldMarket = make_shared<CWorldMarket>();
			gl_pChinaMarket->ResetMarket();
			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pWorldMarket->ResetMarket();

			EXPECT_EQ(gl_vMarketPtr.size(), 0);
			gl_vMarketPtr.push_back(gl_pWorldMarket); // ������Ʊ�г�
			gl_vMarketPtr.push_back(gl_pChinaMarket); // �й���Ʊ�г�

			gl_pMockChinaMarket = make_shared<CMockChinaMarket>();
			gl_pMockChinaMarket->ResetMarket();
			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pMockWorldMarket = make_shared<CMockWorldMarket>(); // �ڴ����ɣ���ȫ��TearDown�Ÿ�ֵnullptr.�������׿���������Ϣ
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
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(__STOCK_IPOED__); // �޸Ļ�Ծ��Ʊ��IPO״̬

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"����Ʊ���߽�����������30��ʱ��װ���Ʊ�������ݿ�ʱҪ����´����";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}

			for (int i = 0; i < gl_pMockChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pMockChinaMarket->GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(__STOCK_IPOED__); // �޸Ļ�Ծ��Ʊ��IPO״̬

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pMockChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"����Ʊ���߽�����������30��ʱ��װ���Ʊ�������ݿ�ʱҪ����´����";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}

			if (gl_pChinaMarket->GetMarketDate() >= 20210531) { // Ŀǰ�������ݿ�����߽�������Ϊ20210430
				if (gl_pChinaMarket->GetDayOfWeek() == 1) { // ���������һ
					EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate()) << "ÿ����һ���¼��ժ�ƹ�Ʊ";
					EXPECT_TRUE(gl_pMockChinaMarket->TooManyStockDayLineNeedUpdate()) << "ÿ����һ���¼��ժ�ƹ�Ʊ";
				}
				else {
					EXPECT_FALSE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate()) << "���еĻ�Ծ��Ʊ������Ϊժ����";
					EXPECT_FALSE(gl_pMockChinaMarket->TooManyStockDayLineNeedUpdate()) << "���еĻ�Ծ��Ʊ������Ϊժ����";
				}
				gl_pChinaMarket->SetStockDayLineNeedUpdate(gl_pChinaMarket->GetTotalStock()); // αװ�˱�ʶ
				gl_pMockChinaMarket->SetStockDayLineNeedUpdate(gl_pChinaMarket->GetTotalStock()); // αװ�˱�ʶ
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
			// ����Ҫ���⽫����Mockȫ�ֱ�������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Expxplorer���ڣ��еõ����Խ����
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}

			gl_fExitingSystem = true;
			// ��������ָ�룬�Բ����Ƿ����û����Ե�Mock��
			gl_pMockChinaMarket = nullptr;
			gl_pMockWorldMarket = nullptr;

			if (CMFCVisualManager::GetInstance() != NULL) {
				delete CMFCVisualManager::GetInstance(); // ������MainFrameʱ��������һ���Ӿ����������ʶ��ڴ�ɾ��֮��
			}

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				pStock->SetUpdateProfileDB(false); // gl_pMockMainFrameʹ����������gl_pChinaMarket,�˴����ô˱�ʶ����ֹ�⹹gl_pMockMainFrameʱ�������ݿ⡣
			}
			ASSERT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse()) << "�˳�ʱ���뱣֤������´����";

			gl_fExitingSystem = false;
			delete gl_pMockMainFrame;
			EXPECT_TRUE(gl_fExitingSystem) << "MainFrame����ʱ���ô˱�ʶ";

			gl_pSinaRTWebInquiry = nullptr;
			gl_pTengxunRTWebInquiry = nullptr;
			gl_pNeteaseRTWebInquiry = nullptr;
			gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
			gl_pNeteaseDayLineWebInquiry2 = nullptr; // ����������ʷ����
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