#include"pch.h"

#include"GeneralCheck.h"

#include"ChinaMarket.h"
#include "ProductEastmoneyDayLine.h"
#include "WebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductEastDayLineTest : public Test {
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
			eastmoneyDayLine.SetInquiryNumber(1);
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductEastmoneyDayLine eastmoneyDayLine;
	};

	TEST_F(CProductEastDayLineTest, TestInitialize) {
		EXPECT_EQ(eastmoneyDayLine.GetInquiryFunction(), "");
	}

	TEST_F(CProductEastDayLineTest, TestParseEastmoneyDayLine) {
		const string sEastmoneyDayLine = R"({"rc":0,"rt":17,"svr":183641822,"lt":1,"full":0,"dlmkts":"","dsc":"0","data":{"code":"601872","market":1,"name":"招商轮船","decimal":2,"dktotal":4621,"preKPrice":6.08,"klines":["2024-12-18,6.11,6.18,6.28,6.10,842743,542829495.00,2.96,1.64,0.10,1.03","2024-12-19,6.15,6.09,6.16,6.03,528781,333859220.00,2.10,-1.46,-0.09,0.65","2024-12-20,6.08,5.97,6.10,5.97,701931,437784952.00,2.13,-1.97,-0.12,0.86","2024-12-23,5.95,6.06,6.08,5.95,675187,423018527.00,2.18,1.51,0.09,0.83","2024-12-24,6.04,6.30,6.34,6.03,1129216,727525144.00,5.12,3.96,0.24,1.39","2024-12-25,6.30,6.26,6.36,6.24,456611,296825401.00,1.90,-0.63,-0.04,0.56","2024-12-26,6.26,6.19,6.26,6.15,374180,239964964.00,1.76,-1.12,-0.07,0.46","2024-12-27,6.16,6.24,6.25,6.16,371161,239139229.00,1.45,0.81,0.05,0.46","2024-12-30,6.24,6.29,6.31,6.23,377719,245904544.00,1.28,0.80,0.05,0.46","2024-12-31,6.29,6.18,6.32,6.17,483794,313479921.00,2.38,-1.75,-0.11,0.59"]}})";

		string_view svData = sEastmoneyDayLine;
		const shared_ptr<vector<CDayLine>> pvDayLine = eastmoneyDayLine.ParseEastmoneyDayLine(svData, "601872.SS");

		EXPECT_EQ(pvDayLine->size(), 10);
		EXPECT_EQ(pvDayLine->at(0).GetStockSymbol(), "601872.SS") << "此时使用标准股票代码形式";
		EXPECT_EQ(pvDayLine->at(0).GetDate(), toLocalDays(20241218));
		EXPECT_EQ(pvDayLine->at(0).GetOpen(), 6110);
		EXPECT_EQ(pvDayLine->at(0).GetClose(), 6180);
		EXPECT_EQ(pvDayLine->at(0).GetHigh(), 6280);
		EXPECT_EQ(pvDayLine->at(0).GetLow(), 6100);
		EXPECT_EQ(pvDayLine->at(0).GetLastClose(), 0) << "第一个数据没有昨收盘数据";
		EXPECT_EQ(pvDayLine->at(0).GetVolume(), 84274300);
		EXPECT_EQ(pvDayLine->at(1).GetStockSymbol(), "601872.SS") << "此时使用标准股票代码形式";
		EXPECT_EQ(pvDayLine->at(1).GetDate(), toLocalDays(20241219));
		EXPECT_EQ(pvDayLine->at(1).GetOpen(), 6150);
		EXPECT_EQ(pvDayLine->at(1).GetClose(), 6090);
		EXPECT_EQ(pvDayLine->at(1).GetHigh(), 6160);
		EXPECT_EQ(pvDayLine->at(1).GetLow(), 6030);
		EXPECT_EQ(pvDayLine->at(1).GetLastClose(), 6180) << "第二个数据使用前一天的收盘数据";
		EXPECT_EQ(pvDayLine->at(1).GetVolume(), 52878100);
	}

	TEST_F(CProductEastDayLineTest, TestParseEastmoneyDayLine2) {
		const string sEastmoneyDayLine = R"({"rc":100,"rt":1,"svr":181669693,"lt":1,"full":1,"dlmkts":"","dsc":"0","data":null})";

		string_view svData = sEastmoneyDayLine;
		const shared_ptr<vector<CDayLine>> pvDayLine = eastmoneyDayLine.ParseEastmoneyDayLine(svData, "601872.SS");

		EXPECT_EQ(pvDayLine->size(), 0);
	}
}
