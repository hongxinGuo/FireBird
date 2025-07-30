#include"pch.h"

#include"GeneralCheck.h"
#include "ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	class CProductDummyTest : public Test {
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
		CProductDummy dummy;
	};

	TEST_F(CProductDummyTest, TestInitialize) {
		EXPECT_STREQ(typeid(dummy).name(), _T("class CProductDummy"));
		EXPECT_EQ(dummy.GetInquiryFunction(), _T(""));
		EXPECT_EQ(dummy.GetIndex(), 0);
	}

	TEST_F(CProductDummyTest, TestCreateMessage) {
		EXPECT_EQ(dummy.CreateMessage(), _T(""));
	}
}
