#include"pch.h"

////#include"gtest/gtest.h"

#include"GeneralCheck.h"
#include "ProductNeteaseRT.h"

#include"VirtualDataSource.h"

#include"WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CVirtualDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp

			SCOPED_TRACE("");
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

	TEST_F(CVirtualDataSourceTest, Test_SetInquiry) {
		const auto p = std::make_shared<CVirtualWebProduct>();
		p->SetIndex(10000);

		EXPECT_EQ(dataSource.InquiryQueueSize(), 0);
		EXPECT_FALSE(dataSource.HaveInquiry());
		dataSource.StoreInquiry(p);
		EXPECT_EQ(dataSource.InquiryQueueSize(), 1);
		EXPECT_TRUE(dataSource.HaveInquiry());
		const CVirtualProductWebDataPtr p2 = dataSource.GetCurrentProduct();
		EXPECT_EQ(p2->GetIndex(), 10000);
		EXPECT_EQ(dataSource.InquiryQueueSize(), 0);
		EXPECT_FALSE(dataSource.HaveInquiry());
	}

	TEST_F(CVirtualDataSourceTest, TestGetCurrentInquiry) {
		const auto pProduct = std::make_shared<CVirtualWebProduct>();
		pProduct->SetIndex(10000);

		const auto p = dataSource.GetCurrentInquiry();
		EXPECT_TRUE(p == nullptr);

		dataSource.SetCurrentInquiry(pProduct);
		const auto p2 = dataSource.GetCurrentInquiry();
		EXPECT_EQ(p2->GetIndex(), 10000);
	}

	TEST_F(CVirtualDataSourceTest, TestCreateInquiryMessageFromCurrentProduct) {
		const auto pProduct = std::make_shared<CProductNeteaseRT>();
		pProduct->SetInquiryFunction(_T("TestGetInquiry")); // 这个product不生成自己的Inquiry，直接赋值而已。
		dataSource.StoreInquiry(pProduct);
		dataSource.GetCurrentProduct();

		dataSource.CreateCurrentInquireString();
		EXPECT_STREQ(dataSource.GetInquiringString(), _T("TestGetInquiry"));
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
		dataSource.SetWebError(true);
		EXPECT_TRUE(dataSource.IsWebError());
		dataSource.SetWebError(false);
		EXPECT_FALSE(dataSource.IsWebError());
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
		const auto p = make_shared<CVirtualWebProduct>();
		const auto p2 = make_shared<CVirtualWebProduct>();
		dataSource.StoreInquiry(p);
		dataSource.StoreInquiry(p2);
		EXPECT_EQ(dataSource.InquiryQueueSize(), 2);

		dataSource.DiscardAllInquiry();

		EXPECT_EQ(dataSource.InquiryQueueSize(), 0);
	}
}
