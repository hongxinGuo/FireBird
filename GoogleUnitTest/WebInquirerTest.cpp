#include"pch.h"

#include"ChinaMarket.h"

#include"MockNeteaseDayLineWebInquiry.h"

#include"GeneralCheck.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	static CMockNeteaseDayLineWebInquiryPtr s_pMockNeteaseDayLineWebInquiry;

	class CWebInquirerTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			ASSERT_THAT(gl_pNeteaseDayLineWebInquiry, NotNull());
			s_pMockNeteaseDayLineWebInquiry = static_pointer_cast<CMockNeteaseDayLineWebInquiry>(gl_pNeteaseDayLineWebInquiry);
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			s_pMockNeteaseDayLineWebInquiry = nullptr;

			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		virtual void TearDown(void) override {
			// clearUp
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			GeneralCheck();
		}
	};

	TEST_F(CWebInquirerTest, TestPushPopWebSocketData) {
		shared_ptr<string> pData = make_shared<string>(_T("abc"));
		shared_ptr<string> pData2;
		gl_finnhubWebSocket.PushData(pData);
		EXPECT_EQ(gl_finnhubWebSocket.DataSize(), 1);
		pData2 = gl_finnhubWebSocket.PopData();
		EXPECT_STREQ(pData2->c_str(), _T("abc"));
	}
}