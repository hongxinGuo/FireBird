#include"pch.h"

#include"WorldMarket.h"

#include"MockQuandlDataSource.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	static bool m_fSystemStatus;
	CMockQuandlDataSourcePtr m_pMockQuandlDataSource; // ����������ʷ����

	class CMockQuandlDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			m_fSystemStatus = gl_pWorldMarket->IsSystemReady();
			gl_pWorldMarket->SetSystemReady(true); // Quandl�������ȴ�ϵͳ��ʼ����ſ�ʹ�á�
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
		EXPECT_EQ(QuandlDataSource.GetInquiryFunction(), _T(""));
		EXPECT_EQ(QuandlDataSource.GetInquirySuffix(), _T("&api_key="));
		EXPECT_EQ(QuandlDataSource.GetInquiryToken(), _T("")) << "Ĭ��������ϵͳ�����ļ��У��˴���ʼ��ʱΪ��";
	}

	TEST_F(CMockQuandlDataSourceTest, TestGenerateInquiryMessage1) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();
		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		m_pMockQuandlDataSource->SetWebError(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(timePoint + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime()))
		.WillOnce(Return(timePoint + 1ms + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime()));
		EXPECT_CALL(*m_pMockQuandlDataSource, Inquire()).Times(1)
		.WillRepeatedly(DoAll(Invoke([p]() {
			m_pMockQuandlDataSource->SetInquiring(true);
			m_pMockQuandlDataSource->StoreInquiry(p);
		}), Return(true)));

		EXPECT_FALSE(m_pMockQuandlDataSource->GenerateInquiryMessage(120500)) << "ʱ��δ���������ȴ�";
		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquiring());
		EXPECT_FALSE(m_pMockQuandlDataSource->HaveInquiry());
		EXPECT_TRUE(m_pMockQuandlDataSource->GenerateInquiryMessage(120500)) << "��������";

		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockQuandlDataSource->HaveInquiry());
	}
}
