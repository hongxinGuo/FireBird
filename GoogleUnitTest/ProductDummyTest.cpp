#include"pch.h"

#include"GeneralCheck.h"
#include "ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	class CProductDummyTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE(""); GeneralCheck();
		}

	protected:
		CProductDummy dummy;
	};

	TEST_F(CProductDummyTest, TestInitialize) {
		EXPECT_STREQ(dummy.GetClassName(), _T("Dummy Product"));
		EXPECT_STREQ(dummy.GetInquiryFunction(), _T(""));
		EXPECT_EQ(dummy.GetIndex(), -1);
	}

	TEST_F(CProductDummyTest, TestCreateMessage) {
		EXPECT_STREQ(dummy.CreateMessage(), _T(""));
	}
}
