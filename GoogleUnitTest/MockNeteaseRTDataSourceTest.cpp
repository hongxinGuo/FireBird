#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseRTDataSource.h"
#include "ProductNeteaseRT.h"

using namespace testing;

namespace FireBirdTest {
	CMockNeteaseRTDataSourcePtr m_pMockNeteaseRTDataSource; // 网易实时数据采集
	class CMockNeteaseRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			m_pMockNeteaseRTDataSource = make_shared<CMockNeteaseRTDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			m_pMockNeteaseRTDataSource = nullptr;
			GeneralCheck();
		}
	};

	TEST_F(CMockNeteaseRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_pMockNeteaseRTDataSource->GetInquiryFunction(), _T("http://api.money.126.net/data/feed/"));
		EXPECT_STREQ(m_pMockNeteaseRTDataSource->GetInquiryToken(), _T(""));
		EXPECT_EQ(m_pMockNeteaseRTDataSource->GetInquiringNumber(), 900) << _T("DEBUG模式下网易默认值");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->ParseData(pData)) << "网易实时数据无需解析";
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestGenerateInquiryMessage) {
		EXPECT_FALSE(m_pMockNeteaseRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_TRUE(gl_pChinaMarket->IsFastReceivingRTData());

		m_pMockNeteaseRTDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pMockNeteaseRTDataSource, GetTickCount()).Times(5)
		.WillOnce(Return(0))
		.WillOnce(Return(10000))
		.WillOnce(Return(20000 + 1 + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(20000 + 1 + 2 * gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(20000 + 2 + 2 * gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));

		EXPECT_FALSE(m_pMockNeteaseRTDataSource->GenerateInquiryMessage(120000));
		EXPECT_FALSE(m_pMockNeteaseRTDataSource->GenerateInquiryMessage(120000)) << "Web Error, postponed 10 seconds";
		m_pMockNeteaseRTDataSource->SetErrorCode(0);
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->GenerateInquiryMessage(120600)) << "已过10秒且网络正常，申请数据";
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->IsInquiring());
		m_pMockNeteaseRTDataSource->SetInquiring(false);

		EXPECT_FALSE(m_pMockNeteaseRTDataSource->GenerateInquiryMessage(120100)) << "继续等待";
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->GenerateInquiryMessage(120600)) << "申请数据";

		EXPECT_TRUE(m_pMockNeteaseRTDataSource->HaveInquiry());
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->IsInquiring());
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestInquireRTData) {
		m_pMockNeteaseRTDataSource->SetInquiring(true);
		EXPECT_FALSE(m_pMockNeteaseRTDataSource->InquireRTData(1010));

		m_pMockNeteaseRTDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->InquireRTData(1010));
		EXPECT_EQ(m_pMockNeteaseRTDataSource->GetInquiryQueueSize(), 1);
		auto pProduct = m_pMockNeteaseRTDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseRT"));
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestGetWebData) {
		m_pMockNeteaseRTDataSource->SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockNeteaseRTDataSource, StartReadingThread)
		.Times(1);
		m_pMockNeteaseRTDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		auto p = make_shared<CProductNeteaseRT>();
		p->SetInquiryFunction(_T("http://api.money.126.net/data/feed/"));
		m_pMockNeteaseRTDataSource->SetCurrentInquiry(p);
		m_pMockNeteaseRTDataSource->CreateInquiryMessageFromCurrentProduct();
		const CString str = m_pMockNeteaseRTDataSource->GetInquiringString();
		EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
	}
}
