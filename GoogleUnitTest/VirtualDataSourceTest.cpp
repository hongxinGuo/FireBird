#include"pch.h"

#include"GeneralCheck.h"

#include"VirtualDataSource.h"
#include"WebInquirer.h"

#include"WorldMarket.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu

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
		ASSERT_TRUE(dataSource.IsDataReceived());
		dataSource.SetDataReceived(false);
		ASSERT_FALSE(dataSource.IsDataReceived());
		dataSource.SetDataReceived(true);
		ASSERT_TRUE(dataSource.IsDataReceived());
	}

	TEST_F(CDataSourceTest, Test_IsDataReceivedAndClearFlag) {
		ASSERT_TRUE(dataSource.IsDataReceived());
		dataSource.IsDataReceivedAndClearFlag();
		ASSERT_FALSE(dataSource.IsDataReceived());
		dataSource.IsDataReceivedAndClearFlag();
		ASSERT_FALSE(dataSource.IsDataReceived());
	}

	TEST_F(CDataSourceTest, Test_SetInquiry) {
		CVirtualProductWebDataPtr p = std::make_shared<CVirtualProductWebData>();
		p->SetIndex(10000);

		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
		dataSource.StoreInquiry(p);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 1);
		CVirtualProductWebDataPtr p2 = dataSource.GetInquiry();
		EXPECT_EQ(p2->GetIndex(), 10000);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
	}
}