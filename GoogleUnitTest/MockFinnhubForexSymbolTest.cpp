///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����FinnhubForexSymbol�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"
#include"DayLine.h"

#include"FinnhubForexSymbol.h"
#include"MockFinnhubForexSymbol.h"

using namespace testing;

#include<memory>
using std::make_shared;

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
