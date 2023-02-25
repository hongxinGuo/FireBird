#include"pch.h"

#include"WorldMarket.h"

#include"MockQuandlDataSource.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	static bool m_fSystemStatus;
	CMockQuandlDataSourcePtr m_pMockQuandlDataSource; // 网易日线历史数据

	class CMockQuandlDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			m_fSystemStatus = gl_pWorldMarket->IsSystemReady();
			gl_pWorldMarket->SetSystemReady(true); // Quandl引擎必须等待系统初始化后才可使用。
		}

		static void TearDownTestSuite(void) {
			gl_pWorldMarket->SetSystemReady(m_fSystemStatus);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
			gl_pWorldMarket->CalculateTime();
			m_pMockQuandlDataSource = make_shared<CMockQuandlDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			m_pMockQuandlDataSource = nullptr;
			gl_pWorldMarket->SetResetMarket(true);
			GeneralCheck();
		}

	protected:
		CQuandlDataSource QuandlDataSource;
	};

	TEST_F(CMockQuandlDataSourceTest, TestInitialize) {
		EXPECT_STREQ(QuandlDataSource.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(QuandlDataSource.GetInquiryToken(), _T(""));
	}

	TEST_F(CMockQuandlDataSourceTest, TestGenerateInquiryMessage1) {
		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		m_pMockQuandlDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pMockQuandlDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(1 + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime()))
		.WillOnce(Return(300000 + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime()))
		.WillOnce(Return(300000 + 100000));
		EXPECT_CALL(*m_pMockQuandlDataSource, InquireQuandl()).Times(2)
		.WillRepeatedly(DoAll(Invoke([]() { m_pMockQuandlDataSource->SetInquiring(true); }), Return(true)));

		EXPECT_TRUE(m_pMockQuandlDataSource->GenerateInquiryMessage(120000)) << "网络报错，延后五分钟";
		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring());
		m_pMockQuandlDataSource->SetInquiring(false);
		m_pMockQuandlDataSource->SetErrorCode(0);
		EXPECT_FALSE(m_pMockQuandlDataSource->GenerateInquiryMessage(120500)) << "未过五分钟，继续等待";
		EXPECT_TRUE(m_pMockQuandlDataSource->GenerateInquiryMessage(120500)) << "已过五分钟，申请数据";

		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring());
	}

	TEST_F(CMockQuandlDataSourceTest, TestGetWebData) {
		m_pMockQuandlDataSource->SetInquireWebDataThreadRunning(false);
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, StartReadingThread)
		.Times(1);
		m_pMockQuandlDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquireWebDataThreadRunning()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	/// <summary>
	/// 测试Mock函数时，互相调用的函数不能同时声明为Mock函数，否则就无法测试了。
	/// 在这里测试CVirtualDataSource类中的Read函数。
	/// </summary>
	TEST_F(CMockQuandlDataSourceTest, TestRead1) {
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->SetStockCode(_T("Test"));
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 0);
		m_pMockQuandlDataSource->SetInquiring(true);
		m_pMockQuandlDataSource->SetInquireWebDataThreadRunning(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, ReadWebData).Times(1)
		.WillOnce(Invoke([]() { m_pMockQuandlDataSource->SetErrorCode(0); }));
		EXPECT_CALL(*m_pMockQuandlDataSource, CreateWebDataAfterSucceedReading).Times(1)
		.WillOnce(Return(pData));

		m_pMockQuandlDataSource->Read();

		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquireWebDataThreadRunning());
		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring()) << "此标识没有重置";
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 1) << "存储了一个WebData";
		const auto p = m_pMockQuandlDataSource->GetReceivedData();
		EXPECT_STREQ(p->GetStockCode(), _T("Test"));
	}

	/// <summary>
	/// 测试Mock函数时，互相调用的函数不能同时声明为Mock函数，否则就无法测试了。
	/// 在这里测试CVirtualDataSource类中的Read函数。
	/// </summary>
	TEST_F(CMockQuandlDataSourceTest, TestRead2) {
		auto p = make_shared<CVirtualWebProduct>();
		m_pMockQuandlDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockQuandlDataSource->GetInquiryQueueSize(), 1);
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 0);
		m_pMockQuandlDataSource->SetInquiring(true);
		m_pMockQuandlDataSource->SetInquireWebDataThreadRunning(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, ReadWebData).Times(1)
		.WillOnce(Invoke([]() { m_pMockQuandlDataSource->SetErrorCode(12002); }));
		EXPECT_CALL(*m_pMockQuandlDataSource, CreateWebDataAfterSucceedReading).Times(0); // 没有调用此函数

		m_pMockQuandlDataSource->Read();

		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquireWebDataThreadRunning());
		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquiring()) << "当出现错误时，立即重置此标识";
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 0) << "没有WebData";
		EXPECT_EQ(m_pMockQuandlDataSource->GetInquiryQueueSize(), 0);
	}
}
