///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试FinnhubForexSymbol的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

////#include"gtest/gtest.h"

#include"GeneralCheck.h"

 

using namespace testing;

namespace FireBirdTest {
	class CMockFinnhubForexSymbolTest : public ::testing::Test {
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
	};
}
