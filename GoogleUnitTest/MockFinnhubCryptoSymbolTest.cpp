///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����FinnhubCryptoSymbol�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

//#include"FinnhubCryptoSymbol.h"
//#include"MockFinnhubCryptoSymbol.h"
//#include"DayLine.h"

#include<memory>
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	class CMockFinnhubCryptoSymbolTest : public ::testing::Test {
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
