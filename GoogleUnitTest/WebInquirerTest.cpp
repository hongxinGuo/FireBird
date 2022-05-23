#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"WebInquirer.h"
#include"MockNeteaseDayLineWebInquiry.h"

#include"GeneralCheck.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CMockNeteaseDayLineWebInquiryPtr s_pMockNeteaseDayLineWebInquiry;
	static CMockNeteaseDayLineWebInquiryPtr s_pMockNeteaseDayLineWebInquiry2;

	class CWebInquirerTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			ASSERT_THAT(gl_pNeteaseDayLineWebInquiry, NotNull());
			s_pMockNeteaseDayLineWebInquiry = static_pointer_cast<CMockNeteaseDayLineWebInquiry>(gl_pNeteaseDayLineWebInquiry);
			ASSERT_THAT(gl_pNeteaseDayLineWebInquiry2, NotNull());
			s_pMockNeteaseDayLineWebInquiry2 = static_pointer_cast<CMockNeteaseDayLineWebInquiry>(gl_pNeteaseDayLineWebInquiry2);
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			s_pMockNeteaseDayLineWebInquiry = nullptr;
			s_pMockNeteaseDayLineWebInquiry2 = nullptr;

			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		virtual void TearDown(void) override {
			// clearup
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			GeneralCheck();
		}
	};

	TEST_F(CWebInquirerTest, TestPushPopParseNeteaseDayLine) {
		CNeteaseDayLineWebDataPtr pWebData = make_shared<CNeteaseDayLineWebData>();
		CNeteaseDayLineWebDataPtr pWebData2;
		pWebData->SetStockCode(_T("abcdefg"));

		EXPECT_EQ(gl_WebInquirer.GetParsedNeteaseDayLineDataSize(), 0);
		gl_WebInquirer.PushParsedNeteaseDayLineData(pWebData);
		EXPECT_EQ(gl_WebInquirer.GetParsedNeteaseDayLineDataSize(), 1);
		pWebData2 = gl_WebInquirer.PopParsedNeteaseDayLineData();
		EXPECT_STREQ(pWebData2->GetStockCode(), _T("abcdefg"));
	}

	TEST_F(CWebInquirerTest, TestGetNeteaseDayLineData) {
		int iSaved = gl_iMaxSavingOneDayLineThreads;
		EXPECT_FALSE(s_pMockNeteaseDayLineWebInquiry2->IsReadingWebData());
		EXPECT_FALSE(s_pMockNeteaseDayLineWebInquiry->IsReadingWebData());

		for (gl_iMaxSavingOneDayLineThreads = 2; gl_iMaxSavingOneDayLineThreads < 7; gl_iMaxSavingOneDayLineThreads++) {
			EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, PrepareNextInquiringStr)
				.Times(1)
				.WillOnce(Return(true))
				.RetiresOnSaturation();
			EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry2, PrepareNextInquiringStr)
				.Times(1)
				.WillOnce(Return(true))
				.RetiresOnSaturation();
			EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry2, StartReadingThread)
				.Times(1)
				.RetiresOnSaturation();
			EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, StartReadingThread())
				.Times(1)
				.RetiresOnSaturation();
			EXPECT_TRUE(gl_WebInquirer.GetNeteaseDayLineData());
			EXPECT_TRUE(s_pMockNeteaseDayLineWebInquiry2->IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
			EXPECT_TRUE(s_pMockNeteaseDayLineWebInquiry->IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
			s_pMockNeteaseDayLineWebInquiry2->SetReadingWebData(false);
			s_pMockNeteaseDayLineWebInquiry->SetReadingWebData(false);
		}

		gl_iMaxSavingOneDayLineThreads = 1;
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, PrepareNextInquiringStr)
			.Times(1)
			.WillOnce(Return(true))
			.RetiresOnSaturation();
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, StartReadingThread())
			.Times(1)
			.RetiresOnSaturation();
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry2, StartReadingThread()).Times(0);
		EXPECT_TRUE(gl_WebInquirer.GetNeteaseDayLineData());
		EXPECT_TRUE(s_pMockNeteaseDayLineWebInquiry->IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
		s_pMockNeteaseDayLineWebInquiry->SetReadingWebData(false);

		gl_iMaxSavingOneDayLineThreads = 7;
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, PrepareNextInquiringStr)
			.Times(1)
			.WillOnce(Return(true))
			.RetiresOnSaturation();
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry, StartReadingThread())
			.Times(1)
			.RetiresOnSaturation();
		EXPECT_CALL(*s_pMockNeteaseDayLineWebInquiry2, StartReadingThread()).Times(0); //默认状态下使用一个提取器
		EXPECT_TRUE(gl_WebInquirer.GetNeteaseDayLineData());
		EXPECT_TRUE(s_pMockNeteaseDayLineWebInquiry->IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
		s_pMockNeteaseDayLineWebInquiry->SetReadingWebData(false);

		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock()) << "采用Mock类，没有真正修改";

		// 恢复原态
		s_pMockNeteaseDayLineWebInquiry2->SetReadingWebData(false);
		s_pMockNeteaseDayLineWebInquiry->SetReadingWebData(false);
		gl_iMaxSavingOneDayLineThreads = iSaved;
	}

	TEST_F(CWebInquirerTest, TestPushPopFinnhubWebSocketData) {
		shared_ptr<string> pData = make_shared<string>(_T("abc"));
		shared_ptr<string> pData2;
		gl_WebInquirer.PushFinnhubWebSocketData(pData);
		EXPECT_EQ(gl_WebInquirer.FinnhubWebSocketDataSize(), 1);
		pData2 = gl_WebInquirer.PopFinnhubWebSocketData();
		EXPECT_STREQ(pData2->c_str(), _T("abc"));
	}

	TEST_F(CWebInquirerTest, TestPushPopTiingoIEXWebSocketData) {
		shared_ptr<string> pData = make_shared<string>(_T("abc"));
		shared_ptr<string> pData2;
		gl_WebInquirer.PushTiingoIEXWebSocketData(pData);
		EXPECT_EQ(gl_WebInquirer.TiingoIEXWebSocketDataSize(), 1);
		pData2 = gl_WebInquirer.PopTiingoIEXWebSocketData();
		EXPECT_STREQ(pData2->c_str(), _T("abc"));
	}

	TEST_F(CWebInquirerTest, TestPushPopTiingoCryptoWebSocketData) {
		shared_ptr<string> pData = make_shared<string>(_T("abc"));
		shared_ptr<string> pData2;
		gl_WebInquirer.PushTiingoCryptoWebSocketData(pData);
		EXPECT_EQ(gl_WebInquirer.TiingoCryptoWebSocketDataSize(), 1);
		pData2 = gl_WebInquirer.PopTiingoCryptoWebSocketData();
		EXPECT_STREQ(pData2->c_str(), _T("abc"));
	}

	TEST_F(CWebInquirerTest, TestPushPopTiingoForexWebSocketData) {
		shared_ptr<string> pData = make_shared<string>(_T("abc"));
		shared_ptr<string> pData2;
		gl_WebInquirer.PushTiingoForexWebSocketData(pData);
		EXPECT_EQ(gl_WebInquirer.TiingoForexWebSocketDataSize(), 1);
		pData2 = gl_WebInquirer.PopTiingoForexWebSocketData();
		EXPECT_STREQ(pData2->c_str(), _T("abc"));
	}
}