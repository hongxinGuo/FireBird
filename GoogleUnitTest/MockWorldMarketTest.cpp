///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����ChinaMarket�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "ChinaMarket.h"
#include"GeneralCheck.h"

#include"Thread.h"

#include"MockWorldMarket.h"

#include"FinnhubDataSource.h"
#include "SinaRTDataSource.h"
//#include "TiingoDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockWorldMarketPtr s_pMockWorldMarket;

	class CMockWorldMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			//EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			s_pMockWorldMarket = make_shared<CMockWorldMarket>(); // �ڴ����ɣ�
			s_pMockWorldMarket->ResetMarket();
			EXPECT_FALSE(s_pMockWorldMarket->IsSystemReady()) << "�г�Ĭ��Ϊ��δ׼����";
			s_pMockWorldMarket->SetSystemReady(true);// ����ϵͳĬ��Ϊ׼������

			EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
			gl_systemMessage.PopInformationMessage();
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();

			while (!s_pMockWorldMarket->IsMarketTaskEmpty()) s_pMockWorldMarket->DiscardCurrentMarketTask();

			for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
				auto pStock = gl_dataContainerTiingoStock.GetStock(i);
				pStock->SetUpdateProfileDB(false);
			}
			EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());
		}

		static void TearDownTestSuite() {
			s_pMockWorldMarket = nullptr;

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			//EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			SCOPED_TRACE("");
			GeneralCheck();
			gl_systemConfiguration.SetExitingSystem(false);
			gl_pFinnhubDataSource->SetUpdateStockProfile(true);
			EXPECT_EQ(gl_pFinnhubDataSource->InquiryQueueSize(), 0);
			EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());
		}

		void TearDown() override {
			// clearUp
			EXPECT_EQ(gl_pFinnhubDataSource->InquiryQueueSize(), 0);

			EXPECT_TRUE(s_pMockWorldMarket->IsSystemReady());
			EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());

			EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockWorldMarketTest, TestInResetTime) {
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());
		EXPECT_CALL(*s_pMockWorldMarket, GetResetTime())
		.WillRepeatedly(Return(13000));

		EXPECT_FALSE(s_pMockWorldMarket->InResetTime(12700));
		EXPECT_TRUE(s_pMockWorldMarket->InResetTime(12701));
		EXPECT_TRUE(s_pMockWorldMarket->InResetTime(13459));
		EXPECT_FALSE(s_pMockWorldMarket->InResetTime(13500));
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());
	}

	TEST_F(CMockWorldMarketTest, TestIsReadyToInquireWebData1) {
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0) << "Ĭ������ʵʱ���ݷ�����";
		EXPECT_FALSE(gl_pSinaRTDataSource->IsWebBusy()) << "Ĭ��ֵ";
		EXPECT_FALSE(gl_pChinaMarket->IsWebBusy());
		EXPECT_CALL(*s_pMockWorldMarket, GetResetTime())
		.WillRepeatedly(Return(13000));

		EXPECT_TRUE(s_pMockWorldMarket->IsReadyToInquireWebData(12700));
		EXPECT_FALSE(s_pMockWorldMarket->IsReadyToInquireWebData(12701));
		EXPECT_FALSE(s_pMockWorldMarket->IsReadyToInquireWebData(13459));
		EXPECT_TRUE(s_pMockWorldMarket->IsReadyToInquireWebData(13500));
	}

	TEST_F(CMockWorldMarketTest, TestIsReadyToInquireWebData2) {
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0) << "Ĭ������ʵʱ���ݷ�����";
		EXPECT_FALSE(gl_pSinaRTDataSource->IsWebBusy()) << "Ĭ��ֵ";
		gl_pSinaRTDataSource->SetWebBusy(true);
		EXPECT_TRUE(gl_pChinaMarket->IsWebBusy());
		EXPECT_CALL(*s_pMockWorldMarket, GetResetTime())
		.WillRepeatedly(Return(13000));

		EXPECT_FALSE(s_pMockWorldMarket->IsReadyToInquireWebData(12700));
		EXPECT_FALSE(s_pMockWorldMarket->IsReadyToInquireWebData(12701));
		EXPECT_FALSE(s_pMockWorldMarket->IsReadyToInquireWebData(13459));
		EXPECT_FALSE(s_pMockWorldMarket->IsReadyToInquireWebData(13500));

		// �ָ�ԭ״
		gl_pSinaRTDataSource->SetWebBusy(false);
	}

	TEST_F(CMockWorldMarketTest, TestUpdateToken) {
		s_pMockWorldMarket->UpdateToken();
	}
}
