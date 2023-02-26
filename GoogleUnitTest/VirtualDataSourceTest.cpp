#include"pch.h"

#include"GeneralCheck.h"
#include "ProductNeteaseRT.h"
#include "TimeConvert.h"

#include"VirtualDataSource.h"

#include"WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CVirtualDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}

	protected:
		CVirtualDataSource dataSource;
	};

	TEST_F(CVirtualDataSourceTest, TestInitialize) {
		EXPECT_STREQ(dataSource.GetHeaders(), _T(""));
		EXPECT_STREQ(dataSource.GetInquiringString(), _T(""));
		EXPECT_STREQ(dataSource.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(dataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(dataSource.GetByteRead(), 0);
	}

	TEST_F(CVirtualDataSourceTest, TestInquire) {
		EXPECT_TRUE(dataSource.GenerateInquiryMessage(10101010));
	}

	TEST_F(CVirtualDataSourceTest, TestEnable) {
		EXPECT_TRUE(dataSource.IsEnable());
		dataSource.Enable(false);
		EXPECT_FALSE(dataSource.IsEnable());
		dataSource.Enable(true);
		EXPECT_TRUE(dataSource.IsEnable());
	}

	TEST_F(CVirtualDataSourceTest, Test_IsInquiry) {
		ASSERT_FALSE(dataSource.IsInquiring());
		dataSource.SetInquiring(true);
		ASSERT_TRUE(dataSource.IsInquiring());
		dataSource.SetInquiring(false);
		ASSERT_FALSE(dataSource.IsInquiring());
	}

	TEST_F(CVirtualDataSourceTest, Test_IsInquiringAndClearFlag) {
		ASSERT_FALSE(dataSource.IsInquiring());
		dataSource.SetInquiring(true);
		dataSource.IsInquiringAndClearFlag();
		ASSERT_FALSE(dataSource.IsInquiring());
		dataSource.IsInquiringAndClearFlag();
		ASSERT_FALSE(dataSource.IsInquiring());
	}

	TEST_F(CVirtualDataSourceTest, Test_SetInquiry) {
		auto p = std::make_shared<CVirtualWebProduct>();
		p->SetIndex(10000);

		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
		EXPECT_FALSE(dataSource.HaveInquiry());
		dataSource.StoreInquiry(p);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 1);
		EXPECT_TRUE(dataSource.HaveInquiry());
		const CVirtualProductWebDataPtr p2 = dataSource.GetCurrentProduct();
		EXPECT_EQ(p2->GetIndex(), 10000);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
		EXPECT_FALSE(dataSource.HaveInquiry());
	}

	TEST_F(CVirtualDataSourceTest, TestGetCurrentInquiry) {
		const auto pProduct = std::make_shared<CVirtualWebProduct>();
		pProduct->SetIndex(10000);

		auto p = dataSource.GetCurrentInquiry();
		EXPECT_TRUE(p == nullptr);

		dataSource.SetCurrentInquiry(pProduct);
		const auto p2 = dataSource.GetCurrentInquiry();
		EXPECT_EQ(p2->GetIndex(), 10000);
	}

	TEST_F(CVirtualDataSourceTest, TestCreateInquiryMessageFromCurrentProduct) {
		const auto pProduct = std::make_shared<CProductNeteaseRT>(); // 这个product不生成自己的Inquiry，直接赋值而已。
		pProduct->SetInquiryFunction(_T("TestGetInquiry"));
		dataSource.StoreInquiry(pProduct);
		dataSource.GetCurrentProduct();

		dataSource.CreateInquiryMessageFromCurrentProduct();
		EXPECT_STREQ(dataSource.GetInquiringString(), _T("TestGetInquiry"));
	}

	TEST_F(CVirtualDataSourceTest, TestCreateWebDataAfterSucceedReading) {
		dataSource.TESTSetBuffer(_T("{ \"data\": 2}"));
		const time_t tUTCTime = GetUTCTime();
		TestSetUTCTime(0);

		const auto pWebData = dataSource.CreateWebDataAfterSucceedReading();

		EXPECT_TRUE(pWebData != nullptr);
		EXPECT_EQ(pWebData->GetTime(), 0) << "设置为当前的UTCTime";
		EXPECT_TRUE(pWebData->GetDataBuffer() == _T("{ \"data\": 2}"));

		// restore
		TestSetUTCTime(tUTCTime);
	}

	TEST_F(CVirtualDataSourceTest, TestStoreReceivedData) {
		const CWebDataPtr pData = make_shared<CWebData>();
		pData->SetTime(10102020);

		EXPECT_FALSE(dataSource.HaveReceivedData());
		dataSource.StoreReceivedData(pData);
		EXPECT_TRUE(dataSource.HaveReceivedData());
		const auto pData2 = dataSource.GetReceivedData();
		EXPECT_FALSE(dataSource.HaveReceivedData());
		EXPECT_EQ(pData2->GetTime(), 10102020);
	}

	TEST_F(CVirtualDataSourceTest, TestGetHeaders) {
		EXPECT_STREQ(dataSource.GetHeaders(), _T(""));
		dataSource.SetHeaders(_T("abcdefg"));
		EXPECT_STREQ(dataSource.GetHeaders(), _T("abcdefg"));
	}

	TEST_F(CVirtualDataSourceTest, TestGetInquiringStringPrefix) {
		dataSource.SetInquiryFunction(_T("abcdefghigh"));
		EXPECT_STREQ(dataSource.GetInquiryFunction(), _T("abcdefghigh"));
		dataSource.SetInquiryFunction(_T(""));
	}

	TEST_F(CVirtualDataSourceTest, TestGetInquiringStringSuffix) {
		dataSource.SetInquiryToken(_T("cdefghigh"));
		EXPECT_STREQ(dataSource.GetInquiryToken(), _T("cdefghigh"));
		dataSource.SetInquiryToken(_T(""));
	}

	TEST_F(CVirtualDataSourceTest, TestCreateTotalInquiringString) {
		dataSource.SetInquirySuffix(_T("abcdef"));
		dataSource.CreateTotalInquiringString();
		EXPECT_STREQ(dataSource.GetInquiringString(), _T("abcdef"));
	}

	TEST_F(CVirtualDataSourceTest, TestIsWebError) {
		EXPECT_FALSE(dataSource.IsWebError());
		dataSource.SetErrorCode(102);
		EXPECT_TRUE(dataSource.IsWebError());
		EXPECT_EQ(dataSource.GetErrorCode(), static_cast<DWORD>(102));
		dataSource.SetErrorCode(0);
		EXPECT_FALSE(dataSource.IsWebError());
		EXPECT_EQ(dataSource.GetErrorCode(), 0);
	}

	TEST_F(CVirtualDataSourceTest, TestIsTimeout) {
		EXPECT_FALSE(dataSource.IsTimeout());
		dataSource.SetErrorCode(12002);
		EXPECT_TRUE(dataSource.IsTimeout());
		dataSource.SetErrorCode(12000);
		EXPECT_FALSE(dataSource.IsTimeout());
	}

	TEST_F(CVirtualDataSourceTest, TestGetByteRead) {
		EXPECT_EQ(dataSource.GetByteRead(), 0);
		dataSource.SetByteRead(10000);
		EXPECT_EQ(dataSource.GetByteRead(), 10000);
		dataSource.AddByteRead(10000);
		EXPECT_EQ(dataSource.GetByteRead(), 20000);
	}

	TEST_F(CVirtualDataSourceTest, TestGetInquiringString) {
		EXPECT_STREQ(dataSource.GetInquiringString(), _T(""));
		dataSource.SetInquiringString(_T("abcdefg"));
		EXPECT_STREQ(dataSource.GetInquiringString(), _T("abcdefg"));
		dataSource.AppendInquiringString(_T("hijk"));
		EXPECT_STREQ(dataSource.GetInquiringString(), _T("abcdefghijk"));
	}

	TEST_F(CVirtualDataSourceTest, TestGetInquiringNumber) {
		EXPECT_EQ(dataSource.GetInquiringNumber(), 500) << _T("默认值为500");
		dataSource.SetInquiringNumber(800);
		EXPECT_EQ(dataSource.GetInquiringNumber(), 800);
	}

	TEST_F(CVirtualDataSourceTest, TestGetTotalByteRead) {
		dataSource.ClearTotalByteRead();
		EXPECT_EQ(dataSource.GetTotalByteRead(), 0);
		dataSource.SetTotalByteRead(1000);
		EXPECT_EQ(dataSource.GetTotalByteRead(), 1000);
		dataSource.ClearTotalByteRead();
		EXPECT_EQ(dataSource.GetTotalByteRead(), 0);
	}

	TEST_F(CVirtualDataSourceTest, TestGetCurrentInquiryTime) {
		EXPECT_EQ(dataSource.GetCurrentInquiryTime(), 0);
		dataSource.SetCurrentInquiryTime(10102020);
		EXPECT_EQ(dataSource.GetCurrentInquiryTime(), 10102020);
	}

	TEST_F(CVirtualDataSourceTest, TestDiscardProduct) {
		auto p = make_shared<CVirtualWebProduct>();
		auto p2 = make_shared<CVirtualWebProduct>();
		dataSource.StoreInquiry(p);
		dataSource.StoreInquiry(p2);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 2);

		dataSource.DiscardProduct();

		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
	}

	TEST_F(CVirtualDataSourceTest, TestDiscardReceivedData) {
		auto p = make_shared<CWebData>();
		auto p2 = make_shared<CWebData>();
		dataSource.StoreReceivedData(p);
		dataSource.StoreReceivedData(p2);
		EXPECT_EQ(dataSource.GetReceivedDataSize(), 2);

		dataSource.DiscardReceivedData();

		EXPECT_EQ(dataSource.GetReceivedDataSize(), 0);
	}

	TEST_F(CVirtualDataSourceTest, TestXferReadingToBuffer) {
		char buffer[1024 * 16]{'a', 'b', 'c', 'd'};
		buffer[1024 * 16 - 1] = 'e';
		dataSource.TESTSetWebBuffer(buffer, 1024 * 16);
		dataSource.XferReadingToBuffer(0, 1024 * 16);
		EXPECT_EQ(dataSource.GetData(0), 'a');
		EXPECT_EQ(dataSource.GetData(1), 'b');
		EXPECT_EQ(dataSource.GetData(2), 'c');
		EXPECT_EQ(dataSource.GetData(3), 'd');
		EXPECT_EQ(dataSource.GetData(1024 * 16 -1), 'e');
	}
}
