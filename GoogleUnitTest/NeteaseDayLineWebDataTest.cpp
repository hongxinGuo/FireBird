#include"pch.h"

 

import FireBird.Market.China;
#include"GeneralCheck.h"

import FireBird.Type.DayLineWebData;

namespace FireBirdTest {
	class CDownLoadedNeteaseDayLineTest : public testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
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

	TEST_F(CDownLoadedNeteaseDayLineTest, TestSkipNeteaseDayLineFirstInformationLine) {
		CDayLineWebData DayLine;
		INT64 lCurrentPos = 0;

		string str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n");
		DayLine.SetStockCode(_T("600000.SS"));
		EXPECT_TRUE(DayLine.SkipNeteaseDayLineInformationHeader(str, lCurrentPos));
		EXPECT_EQ(lCurrentPos, str.size());
		lCurrentPos = 0;

		str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\n"); // 缺少\r
		DayLine.SetStockCode(_T("600000.SS"));
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader(str, lCurrentPos));
		EXPECT_EQ(lCurrentPos, str.size());
		lCurrentPos = 0;

		str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r"); // 缺少\n
		DayLine.SetStockCode(_T("600000.SS"));
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader(str, lCurrentPos));
		EXPECT_EQ(lCurrentPos, str.size());
		lCurrentPos = 0;

		str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值"); // 缺少\r
		DayLine.SetStockCode(_T("600000.SS"));
		EXPECT_FALSE(DayLine.SkipNeteaseDayLineInformationHeader(str, lCurrentPos));
		EXPECT_EQ(lCurrentPos, str.size());
		lCurrentPos = 0;
	}
};
