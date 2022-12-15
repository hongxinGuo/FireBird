#include"pch.h"

#include"ThreadStatus.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"NeteaseDayLineDataSource.h"

#include"thread.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CThreadReadNeteaseDayLineTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			NeteaseDayLineWebInquiry.SetDataSource(gl_pNeteaseDayLineDataSource.get());
		}

		virtual void TearDown(void) override {
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
		}
		CMockNeteaseDayLineWebInquiry NeteaseDayLineWebInquiry;
	};

	TEST_F(CThreadReadNeteaseDayLineTest, TestThreadReadNeteaseDayLine) {
		int iCreatingThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
		EXPECT_CALL(NeteaseDayLineWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(false));
		NeteaseDayLineWebInquiry.TESTSetBuffer(_T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n2022-01-05,'600600,浦发银行,101.0,103.0,98.51,99.04,99.66,1.34,0.7881,5484453,555944110.0,1.37799834093e+11,70287275317.0\r\n"));
		NeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("600601.SS"));
		NeteaseDayLineWebInquiry.SetReadingWebData(true);
		EXPECT_EQ(ThreadReadVirtualWebData(&NeteaseDayLineWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);

		EXPECT_FALSE(NeteaseDayLineWebInquiry.IsReadingWebData());
		NeteaseDayLineWebInquiry.SetReadingWebData(true);
		EXPECT_CALL(NeteaseDayLineWebInquiry, ReadingWebData())
			.Times(1)
			.WillOnce(Return(true));
		NeteaseDayLineWebInquiry.TESTSetBuffer(_T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n2022-01-05,'600600,浦发银行,101.0,103.0,98.51,99.04,99.66,1.34,0.7881,5484453,555944110.0,1.37799834093e+11,70287275317.0\r\n2022-01-04,'600600,浦发银行,99.66,100.06,96.32,99.0,99.0,0.66,0.7472,5200069,510503557.0,1.3597159867e+11,69354751070.2\r\n"));
		EXPECT_EQ(ThreadReadVirtualWebData(&NeteaseDayLineWebInquiry), (UINT)1);
		EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iCreatingThread);
		EXPECT_EQ(gl_pNeteaseDayLineDataSource->GetReceivedDataSize(), 1);

		// 恢复原状
		gl_pNeteaseDayLineDataSource->GetReceivedData();
	}
}