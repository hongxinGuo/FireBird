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

	TEST_F(CDataSourceTest, Test_IsDataReceived) {
		ASSERT_TRUE(dataSource.IsWebInquiryFinished());
		dataSource.SetWebInquiryFinished(false);
		ASSERT_FALSE(dataSource.IsWebInquiryFinished());
		dataSource.SetWebInquiryFinished(true);
		ASSERT_TRUE(dataSource.IsWebInquiryFinished());
	}

	TEST_F(CDataSourceTest, Test_IsWebInquiryHaveRunAndClearFlag) {
		ASSERT_TRUE(dataSource.IsWebInquiryFinished());
		dataSource.IsWebInquiryFinishedAndClearFlag();
		ASSERT_FALSE(dataSource.IsWebInquiryFinished());
		dataSource.IsWebInquiryFinishedAndClearFlag();
		ASSERT_FALSE(dataSource.IsWebInquiryFinished());
	}

	TEST_F(CDataSourceTest, Test_SetInquiry) {
		auto p = std::make_shared<CVirtualWebProduct>();
		p->SetIndex(10000);

		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
		dataSource.StoreInquiry(p);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 1);
		CVirtualProductWebDataPtr p2 = dataSource.GetInquiry();
		EXPECT_EQ(p2->GetIndex(), 10000);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
	}
}
