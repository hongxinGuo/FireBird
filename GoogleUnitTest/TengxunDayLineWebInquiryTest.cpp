#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"TengxunDayLineDataSource.h"
#include"MockTengxunDayLineWebInquiry.h"
#include "TengxunDayLineWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
	class CTengxunDayLineWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
		}

		void SetUp(void) override {
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());

			m_TengxunDayLineWebInquiry.SetDataSource(gl_pTengxunDayLineDataSource.get());
			m_MockTengxunDayLineWebInquiry.SetDataSource(gl_pTengxunDayLineDataSource.get());
			gl_pChinaMarket->CalculateTime();
			m_MockTengxunDayLineWebInquiry.ResetDownLoadingStockCode();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			m_MockTengxunDayLineWebInquiry.ResetDownLoadingStockCode();
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		CMockTengxunDayLineWebInquiry m_MockTengxunDayLineWebInquiry; // ����������ʷ����
		CTengxunDayLineWebInquiry m_TengxunDayLineWebInquiry;
	};

	TEST_F(CTengxunDayLineWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_MockTengxunDayLineWebInquiry.GetInquiryFunction(), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param="));
		EXPECT_STREQ(m_MockTengxunDayLineWebInquiry.GetInquirySuffix(), _T(""));
		EXPECT_STREQ(m_MockTengxunDayLineWebInquiry.GetInquiryToken(), _T(""));
		EXPECT_STREQ(m_MockTengxunDayLineWebInquiry.GetConnectionName(), _T("TengxunDayLine"));
	}

	TEST_F(CTengxunDayLineWebInquiryTest, TestGetWebData) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		m_MockTengxunDayLineWebInquiry.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockTengxunDayLineWebInquiry.GetWebData());
		m_MockTengxunDayLineWebInquiry.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_MockTengxunDayLineWebInquiry, PrepareNextInquiringString())
			.Times(1)
			.WillOnce(Return(true))
			.RetiresOnSaturation();
		EXPECT_CALL(m_MockTengxunDayLineWebInquiry, StartReadingThread)
			.Times(1);
		m_MockTengxunDayLineWebInquiry.GetWebData();
		EXPECT_TRUE(m_MockTengxunDayLineWebInquiry.IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CTengxunDayLineWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_MockTengxunDayLineWebInquiry.ReportStatus(1));
	}

	TEST_F(CTengxunDayLineWebInquiryTest, TestSetDownLoadingStockCode) {
		EXPECT_STREQ(m_MockTengxunDayLineWebInquiry.GetDownLoadingStockCode(), _T(""));
		m_MockTengxunDayLineWebInquiry.SetDownLoadingStockCode(_T("000001.SS"));
		EXPECT_STREQ(m_MockTengxunDayLineWebInquiry.GetDownLoadingStockCode(), _T("000001.SS"));
		m_MockTengxunDayLineWebInquiry.SetDownLoadingStockCode(_T("0600001"));
		EXPECT_STREQ(m_MockTengxunDayLineWebInquiry.GetDownLoadingStockCode(), _T("0600001"));
		m_MockTengxunDayLineWebInquiry.SetDownLoadingStockCode(_T("2600001"));
		EXPECT_STREQ(m_MockTengxunDayLineWebInquiry.GetDownLoadingStockCode(), _T("2600001"));
	}

	TEST_F(CTengxunDayLineWebInquiryTest, TestPrepareNextInquiringStr) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		CString str;
		gl_pChinaMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (m_TengxunDayLineWebInquiry.PrepareNextInquiringString()) {
				str = m_TengxunDayLineWebInquiry.GetInquiringString();
				EXPECT_STREQ(str.Left(57), _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param="));
			}
			else {
				EXPECT_EQ(str.GetLength(), 0);
			}
		}
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_LE(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		// Ŀǰ���������뺯���ڣ���Ϊ��̬�������ڣ��ʶ��޷�֪��ȷ��λ���ˡ�
		//EXPECT_FALSE(gl_pChinaMarket->GetStock(0)->IsDayLineNeedUpdate());

		// �ָ�ԭ̬
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
		}
	}
}
