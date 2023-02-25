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
		static void SetUpTestSuite(void) {
			GeneralCheck();
			m_fSystemStatus = gl_pWorldMarket->IsSystemReady();
			gl_pWorldMarket->SetSystemReady(true); // Quandl�������ȴ�ϵͳ��ʼ����ſ�ʹ�á�
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

		EXPECT_TRUE(m_pMockQuandlDataSource->GenerateInquiryMessage(120000)) << "���籨���Ӻ������";
		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring());
		m_pMockQuandlDataSource->SetInquiring(false);
		m_pMockQuandlDataSource->SetErrorCode(0);
		EXPECT_FALSE(m_pMockQuandlDataSource->GenerateInquiryMessage(120500)) << "δ������ӣ������ȴ�";
		EXPECT_TRUE(m_pMockQuandlDataSource->GenerateInquiryMessage(120500)) << "�ѹ�����ӣ���������";

		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring());
	}

	TEST_F(CMockQuandlDataSourceTest, TestGetWebData) {
		m_pMockQuandlDataSource->SetInquireWebDataThreadRunning(false);
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockQuandlDataSource, StartReadingThread)
		.Times(1);
		m_pMockQuandlDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquireWebDataThreadRunning()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}

	/// <summary>
	/// ����Mock����ʱ��������õĺ�������ͬʱ����ΪMock������������޷������ˡ�
	/// ���������CVirtualDataSource���е�Read������
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
		EXPECT_TRUE(m_pMockQuandlDataSource->IsInquiring()) << "�˱�ʶû������";
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 1) << "�洢��һ��WebData";
		const auto p = m_pMockQuandlDataSource->GetReceivedData();
		EXPECT_STREQ(p->GetStockCode(), _T("Test"));
	}

	/// <summary>
	/// ����Mock����ʱ��������õĺ�������ͬʱ����ΪMock������������޷������ˡ�
	/// ���������CVirtualDataSource���е�Read������
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
		EXPECT_CALL(*m_pMockQuandlDataSource, CreateWebDataAfterSucceedReading).Times(0); // û�е��ô˺���

		m_pMockQuandlDataSource->Read();

		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquireWebDataThreadRunning());
		EXPECT_FALSE(m_pMockQuandlDataSource->IsInquiring()) << "�����ִ���ʱ���������ô˱�ʶ";
		EXPECT_EQ(m_pMockQuandlDataSource->GetReceivedDataSize(), 0) << "û��WebData";
		EXPECT_EQ(m_pMockQuandlDataSource->GetInquiryQueueSize(), 0);
	}
}
