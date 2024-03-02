///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����ChinaMarket�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

#include"MockWorldMarket.h"

#include"FinnhubDataSource.h"
#include "TiingoDataSource.h"

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

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockWorldMarketTest, TestUpdateToken) {
		s_pMockWorldMarket->UpdateToken();
	}
}
