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
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			m_fSystemStatus = gl_pWorldMarket->IsSystemReady();
			gl_pWorldMarket->SetSystemReady(true); // Quandl引擎必须等待系统初始化后才可使用。
		}

		static void TearDownTestSuite() {
			gl_pWorldMarket->SetSystemReady(m_fSystemStatus);
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			gl_pWorldMarket->CalculateTime();
			m_pMockQuandlDataSource = make_shared<CMockQuandlDataSource>();
		}

		void TearDown() override {
			// clearUp
			m_pMockQuandlDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CQuandlDataSource QuandlDataSource;
	};

	TEST_F(CMockQuandlDataSourceTest, TestInitialize) {
		EXPECT_STREQ(QuandlDataSource.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(QuandlDataSource.GetInquirySuffix(), _T("&api_key="));
		EXPECT_STREQ(QuandlDataSource.GetInquiryToken(), _T("")) << "默认令牌在系统配置文件中，此处初始化时为空";
	}

	TEST_F(CMockQuandlDataSourceTest, TestGenerateInquiryMessage1) {
		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		m_pMockQuandlDataSource->SetWebError(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(gl_systemConfiguration.GetWorldMarketQuandlInquiryTime()))
		.WillOnce(Return(1 + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime()));
		EXPECT_CALL(*m_pMockQuandlDataSource, Inquire()).Times(1)
		.WillRepeatedly(DoAll(Invoke([p]() {
			m_pMockQuandlDataSource->SetInquiring(true);
			m_pMockQuandlDataSource->StoreInquiry(p);
		}), Return(true)));

		EXPECT_FALSE(m_pMockQuandlDataSource->GenerateInquiryMessage(120500)) << "时间未到，继续等待";
		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquiring());
		EXPECT_FALSE(m_pMockQuandlDataSource->HaveInquiry());
		EXPECT_TRUE(m_pMockQuandlDataSource->GenerateInquiryMessage(120500)) << "申请数据";

		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockQuandlDataSource->HaveInquiry());
	}

	/// <summary>
	/// 测试Mock函数时，互相调用的函数不能同时声明为Mock函数，否则就无法测试了。
	/// 在这里测试CVirtualDataSource类中的Read函数。
	/// </summary>
	TEST_F(CMockQuandlDataSourceTest, TestGetWebDataImp1) {
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->SetStockCode(_T("Test"));
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 0);
		m_pMockQuandlDataSource->SetInquiring(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, ReadWebData).Times(1)
		.WillOnce(Invoke([]() { m_pMockQuandlDataSource->SetWebError(false); }));
		EXPECT_CALL(*m_pMockQuandlDataSource, CreateWebData).Times(1)
		.WillOnce(Return(pData));

		m_pMockQuandlDataSource->GetWebDataImp();

		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring()) << "此标识没有重置";
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 1) << "存储了一个WebData";
		const auto p = m_pMockQuandlDataSource->GetReceivedData();
		EXPECT_STREQ(p->GetStockCode(), _T("Test"));
	}

	/// <summary>
	/// 测试Mock函数时，互相调用的函数不能同时声明为Mock函数，否则就无法测试了。
	/// 在这里测试CVirtualDataSource类中的Read函数。
	/// </summary>
	TEST_F(CMockQuandlDataSourceTest, TestGetWebDataImp2) {
		const auto p = make_shared<CVirtualWebProduct>();
		m_pMockQuandlDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockQuandlDataSource->InquiryQueueSize(), 1);
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 0);
		m_pMockQuandlDataSource->SetInquiring(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, ReadWebData).Times(1)
		.WillOnce(Invoke([]() { m_pMockQuandlDataSource->SetWebError(true); }));
		EXPECT_CALL(*m_pMockQuandlDataSource, CreateWebData).Times(0); // 没有调用此函数

		m_pMockQuandlDataSource->GetWebDataImp();

		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquiring()) << "当出现错误时，立即重置此标识";
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 0) << "没有WebData";
		EXPECT_EQ(m_pMockQuandlDataSource->InquiryQueueSize(), 0);
	}
}
