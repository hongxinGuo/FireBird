#pragma once

#ifndef __GOOGLEMOCK__
#error _T("������ϵͳ����ʹ��GOOGLE MOCK")
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
	// ��������ʱ�������Mock������Ϊȫ�ֱ������ڲ���ϵͳ�˳�ʱ������,���������ڲ�����Ϣ�����з��ֹ���
	CMockWorldMarketPtr gl_pMockWorldMarket;
	CMockChinaMarketPtr gl_pMockChinaMarket;
	CMockMainFramePtr gl_pMockMainFrame; // ��Mock��ʹ����ʵ�ĸ��г���(gl_pChinaMarket, gl_pWorldMarket, ...)

	class TestEnvironment : public Environment {
		// ȫ�ֳ�ʼ������main()�������á�
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

			// ����ȫ������ָ��Ϊʵ����
			gl_pChinaMarket = make_shared<CChinaMarket>();
			gl_pWorldMarket = make_shared<CWorldMarket>();
			gl_pChinaMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pWorldMarket->ResetMarket();

			EXPECT_EQ(gl_vMarketPtr.size(), 0);
			gl_vMarketPtr.push_back(gl_pWorldMarket); // ������Ʊ�г�
			gl_vMarketPtr.push_back(gl_pChinaMarket); // �й���Ʊ�г�

			gl_pMockChinaMarket = make_shared<CMockChinaMarket>();
			gl_pMockChinaMarket->ResetMarket();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			gl_pMockWorldMarket = make_shared<CMockWorldMarket>(); // �ڴ����ɣ���ȫ��TearDown�Ÿ�ֵnullptr.�������׿���������Ϣ
			gl_pMockWorldMarket->ResetMarket();
			EXPECT_LE(gl_pMockChinaMarket->GetDayLineNeedUpdateNumber(), gl_pMockChinaMarket->GetTotalStock());
			EXPECT_LE(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL); //
			gl_pMockMainFrame = make_shared<CMockMainFrame>();
			EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL) << "������MainFrameʱ��������һ���Ӿ������������˳�ʱ��Ҫɾ��֮";

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				auto pStock = gl_pChinaMarket->GetStock(i);
				pStock->SetDayLineNeedUpdate(true);
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // �޸Ļ�Ծ��Ʊ��IPO״̬

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
				if (pStock->GetDayLineEndDate() == 20210430) pStock->SetIPOStatus(_STOCK_IPOED_); // �޸Ļ�Ծ��Ʊ��IPO״̬

				if (IsEarlyThen(pStock->GetDayLineEndDate(), gl_pMockChinaMarket->GetMarketDate(), 30)) {
					if (pStock->GetDayLineEndDate() == 20210430) {
						EXPECT_TRUE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol(); //"����Ʊ���߽�����������30��ʱ��װ���Ʊ�������ݿ�ʱҪ����´����";
						pStock->SetUpdateProfileDB(false);
					}
				}
			}

			if (gl_pChinaMarket->GetMarketDate() >= 20210531) {
				// Ŀǰ�������ݿ�����߽�������Ϊ20210430
				if (gl_pChinaMarket->GetDayOfWeek() == 1) {
					// ���������һ
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

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();

			auto pStock1 = gl_pWorldMarket->GetStock(_T("AAPL"));
			pStock1->SetUpdateCompanyProfile(true);

			// ���Ԥװ���finnhubInaccessibleExchange
			gl_finnhubInaccessibleExchange.Clear();

			GeneralCheck();
		}

		void TearDown(void) override {
			// ����Ҫ���⽫����Mockȫ�ֱ�������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Expxplorer���ڣ��еõ����Խ����
			GeneralCheck();

			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_TRUE(pStock->IsDayLineNeedUpdate()) << pStock->GetSymbol();
			}

			gl_systemStatus.SetExitingSystem(true);
			// ��������ָ�룬�Բ����Ƿ����û����Ե�Mock��
			gl_pMockChinaMarket = nullptr;
			gl_pMockWorldMarket = nullptr;

			if (CMFCVisualManager::GetInstance() != nullptr) {
				delete CMFCVisualManager::GetInstance(); // ������MainFrameʱ��������һ���Ӿ����������ʶ��ڴ�ɾ��֮��
			}

			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				const auto pStock = gl_pChinaMarket->GetStock(i);
				EXPECT_FALSE(pStock->IsUpdateProfileDB()) << pStock->GetSymbol();
				pStock->SetUpdateProfileDB(false);
				// gl_pMockMainFrameʹ����������gl_pChinaMarket,�˴����ô˱�ʶ����ֹ�⹹gl_pMockMainFrameʱ�������ݿ⡣
			}
			ASSERT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse()) << "�˳�ʱ���뱣֤������´����";

			gl_systemStatus.SetExitingSystem(false);
			gl_pMockMainFrame = nullptr;
			EXPECT_TRUE(gl_systemStatus.IsExitingSystem()) << "MainFrame����ʱ���ô˱�ʶ";

			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			while (gl_ThreadStatus.IsWebInquiringThreadRunning()) Sleep(1);
			gl_pWorldMarket = nullptr;
			gl_pChinaMarket = nullptr;
			gl_vMarketPtr.clear();

			// ������finnhubInaccessibleExchange�ļ�
			gl_finnhubInaccessibleExchange.SetUpdate(false);
		}
	};
}
