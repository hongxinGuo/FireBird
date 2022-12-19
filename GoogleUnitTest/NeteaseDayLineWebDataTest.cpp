#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"NeteaseDayLineWebData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // 网易日线历史数据

	class CDownLoadedNeteaseDayLineTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CDownLoadedNeteaseDayLineTest, TestSkipNeteaseDayLineFirstInformationLine) {
		CNeteaseDayLineWebInquiry DayLineWebInquiry;
		CNeteaseDayLineWebData DayLine;

		CString str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n");
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteRead(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_TRUE(DayLine.SkipNeteaseDayLineInformationHeader());
		EXPECT_EQ(DayLine.GetCurrentPos(), str.GetLength());
		DayLine.SetCurrentPos(0);

		str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\n"); // 缺少\r
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteRead(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader());
		EXPECT_EQ(DayLine.GetCurrentPos(), str.GetLength());
		DayLine.SetCurrentPos(0);

		str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r"); // 缺少\n
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteRead(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader());
		EXPECT_EQ(DayLine.GetCurrentPos(), str.GetLength());
		DayLine.SetCurrentPos(0);

		str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值"); // 缺少\r
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteRead(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader());
		EXPECT_EQ(DayLine.GetCurrentPos(), str.GetLength());
		DayLine.SetCurrentPos(0);
	}

	TEST_F(CDownLoadedNeteaseDayLineTest, TestTransferNeteaseDayLineWebDataToBuffer) {
		CNeteaseDayLineWebInquiry DayLineWebInquiry;
		CNeteaseDayLineWebData DayLine;
		CString str = _T("abcedfg\r\n");
		EXPECT_EQ(DayLine.GetBufferLength(), 0);
		DayLineWebInquiry.SetDownLoadingStockCode(_T("600000.SS"));
		DayLineWebInquiry.SetByteRead(str.GetLength());
		for (int i = 0; i < str.GetLength(); i++) {
			DayLineWebInquiry.SetData(i, str.GetAt(i));
		}
		DayLine.TransferNeteaseDayLineWebDataToBuffer(&DayLineWebInquiry);
		EXPECT_EQ(DayLine.GetBufferLength(), str.GetLength());
	}
};