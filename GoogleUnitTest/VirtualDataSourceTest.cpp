#include"pch.h"

#include"GeneralCheck.h"

#include"VirtualDataSource.h"

#include"WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}

	protected:
		CVirtualDataSource dataSource;
	};

	TEST_F(CDataSourceTest, TestInitialize) {
		EXPECT_STREQ(dataSource.GetHeaders(), _T(""));
		EXPECT_STREQ(dataSource.GetInquiringString(), _T(""));
		EXPECT_STREQ(dataSource.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(dataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(dataSource.GetByteRead(), 0);
		EXPECT_FALSE(dataSource.IsInquiringWebData());
	}

	TEST_F(CDataSourceTest, TestInquire) {
		EXPECT_TRUE(dataSource.Inquire(10101010));
	}

	TEST_F(CDataSourceTest, TestEnable) {
		EXPECT_TRUE(dataSource.IsEnable());
		dataSource.Enable(false);
		EXPECT_FALSE(dataSource.IsEnable());
		dataSource.Enable(true);
		EXPECT_TRUE(dataSource.IsEnable());
	}

	TEST_F(CDataSourceTest, Test_IsInquiry) {
		ASSERT_FALSE(dataSource.IsInquiring());
		dataSource.SetInquiring(true);
		ASSERT_TRUE(dataSource.IsInquiring());
		dataSource.SetInquiring(false);
		ASSERT_FALSE(dataSource.IsInquiring());
	}

	TEST_F(CDataSourceTest, Test_IsInquiringAndClearFlag) {
		ASSERT_FALSE(dataSource.IsInquiring());
		dataSource.SetInquiring(true);
		dataSource.IsInquiringAndClearFlag();
		ASSERT_FALSE(dataSource.IsInquiring());
		dataSource.IsInquiringAndClearFlag();
		ASSERT_FALSE(dataSource.IsInquiring());
	}

	TEST_F(CDataSourceTest, Test_SetInquiry) {
		auto p = std::make_shared<CVirtualWebProduct>();
		p->SetIndex(10000);

		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
		EXPECT_FALSE(dataSource.HaveInquiry());
		dataSource.StoreInquiry(p);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 1);
		EXPECT_TRUE(dataSource.HaveInquiry());
		const CVirtualProductWebDataPtr p2 = dataSource.GetInquiry();
		EXPECT_EQ(p2->GetIndex(), 10000);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
		EXPECT_FALSE(dataSource.HaveInquiry());
	}

	TEST_F(CDataSourceTest, TestGetCurrentInquiry) {
		const auto pProduct = std::make_shared<CVirtualWebProduct>();
		pProduct->SetIndex(10000);

		auto p = dataSource.GetCurrentInquiry();
		EXPECT_TRUE(p == nullptr);

		dataSource.SetCurrentInquiry(pProduct);
		const auto p2 = dataSource.GetCurrentInquiry();
		EXPECT_EQ(p2->GetIndex(), 10000);
	}

	TEST_F(CDataSourceTest, TestStoreReceivedData) {
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->SetTime(10102020);

		EXPECT_FALSE(dataSource.HaveReceivedData());
		dataSource.StoreReceivedData(pData);
		EXPECT_TRUE(dataSource.HaveReceivedData());
		const auto pData2 = dataSource.GetReceivedData();
		EXPECT_FALSE(dataSource.HaveReceivedData());
		EXPECT_EQ(pData2->GetTime(), 10102020);
	}

	TEST_F(CDataSourceTest, TestGetHeaders) {
		EXPECT_STREQ(dataSource.GetHeaders(), _T(""));
		dataSource.SetHeaders(_T("abcdefg"));
		EXPECT_STREQ(dataSource.GetHeaders(), _T("abcdefg"));
	}

	TEST_F(CDataSourceTest, TestReset) {
		dataSource.SetByteRead(1000);
		dataSource.Reset();
		EXPECT_EQ(dataSource.GetByteRead(), 0);
		EXPECT_FALSE(dataSource.IsWebError());
	}

	TEST_F(CDataSourceTest, TestGetInquiringStringPrefix) {
		dataSource.SetInquiryFunction(_T("abcdefghigh"));
		EXPECT_STREQ(dataSource.GetInquiryFunction(), _T("abcdefghigh"));
		dataSource.SetInquiryFunction(_T(""));
	}

	TEST_F(CDataSourceTest, TestGetInquiringStringSuffix) {
		dataSource.SetInquiryToken(_T("cdefghigh"));
		EXPECT_STREQ(dataSource.GetInquiryToken(), _T("cdefghigh"));
		dataSource.SetInquiryToken(_T(""));
	}

	TEST_F(CDataSourceTest, TestCreateTotalInquiringString) {
		dataSource.SetInquirySuffix(_T("abcdef"));
		dataSource.CreateTotalInquiringString();
		EXPECT_STREQ(dataSource.GetInquiringString(), _T("abcdef"));
	}

	TEST_F(CDataSourceTest, TestIsReadingWebData) {
		EXPECT_FALSE(dataSource.IsInquiringWebData());
		dataSource.SetInquiringWebData(true);
		EXPECT_TRUE(dataSource.IsInquiringWebData());
		dataSource.SetInquiringWebData(false);
		EXPECT_FALSE(dataSource.IsInquiringWebData());
	}

	TEST_F(CDataSourceTest, TestIsWebError) {
		EXPECT_FALSE(dataSource.IsWebError());
		dataSource.SetErrorCode(102);
		EXPECT_TRUE(dataSource.IsWebError());
		EXPECT_EQ(dataSource.GetErrorCode(), static_cast<DWORD>(102));
		dataSource.SetErrorCode(0);
		EXPECT_FALSE(dataSource.IsWebError());
		EXPECT_EQ(dataSource.GetErrorCode(), 0);
	}

	TEST_F(CDataSourceTest, TestIsTimeout) {
		EXPECT_FALSE(dataSource.IsTimeout());
		dataSource.SetErrorCode(12002);
		EXPECT_TRUE(dataSource.IsTimeout());
		dataSource.SetErrorCode(12000);
		EXPECT_FALSE(dataSource.IsTimeout());
	}

	TEST_F(CDataSourceTest, TestGetByteRead) {
		EXPECT_EQ(dataSource.GetByteRead(), 0);
		dataSource.SetByteRead(10000);
		EXPECT_EQ(dataSource.GetByteRead(), 10000);
		dataSource.AddByteRead(10000);
		EXPECT_EQ(dataSource.GetByteRead(), 20000);
	}

	TEST_F(CDataSourceTest, TestGetInquiringString) {
		EXPECT_STREQ(dataSource.GetInquiringString(), _T(""));
		dataSource.SetInquiringString(_T("abcdefg"));
		EXPECT_STREQ(dataSource.GetInquiringString(), _T("abcdefg"));
		dataSource.AppendInquiringString(_T("hijk"));
		EXPECT_STREQ(dataSource.GetInquiringString(), _T("abcdefghijk"));
	}

	TEST_F(CDataSourceTest, TestGetInquiringNumber) {
		EXPECT_EQ(dataSource.GetInquiringNumber(), 500) << _T("Ä¬ÈÏÖµÎª500");
		dataSource.SetInquiringNumber(800);
		EXPECT_EQ(dataSource.GetInquiringNumber(), 800);
	}

	TEST_F(CDataSourceTest, TestGetTotalByteRead) {
		dataSource.ClearTotalByteRead();
		EXPECT_EQ(dataSource.GetTotalByteRead(), 0);
		dataSource.SetTotalByteRead(1000);
		EXPECT_EQ(dataSource.GetTotalByteRead(), 1000);
		dataSource.ClearTotalByteRead();
		EXPECT_EQ(dataSource.GetTotalByteRead(), 0);
	}

	TEST_F(CDataSourceTest, TestGetCurrentInquiryTime) {
		EXPECT_EQ(dataSource.GetCurrentInquiryTime(), 0);
		dataSource.SetCurrentInquiryTime(10102020);
		EXPECT_EQ(dataSource.GetCurrentInquiryTime(), 10102020);
	}
}
