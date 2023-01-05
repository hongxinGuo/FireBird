#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubStockBasicFinancial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	class CFinnhubStockBasicFinancialTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CFinnhubStockBasicFinancialTest, TestInitialize) { }

	TEST_F(CFinnhubStockBasicFinancialTest, TestIsNewData) {
		CFinnhubStockBasicFinancial finnhubStockBasicFinancial;
		vector<CItemOfBasicFinancialSeasonData> vDBData;
		CItemOfBasicFinancialSeasonData data;
		data.m_symbol = _T("AAPL");
		data.m_type = _T("currentRatio");
		data.m_date = 20190101;
		data.m_value = 1.0;
		vDBData.push_back(data);
		data.m_symbol = _T("AAPL");
		data.m_type = _T("currentRatio");
		data.m_date = 20200101;
		data.m_value = 2.0;
		vDBData.push_back(data);
		data.m_symbol = _T("AAPL");
		data.m_type = _T("currentRatio");
		data.m_date = 20210101;
		data.m_value = 3.0;
		vDBData.push_back(data);

		CValueOfPeriod valueData{20190101, 1.0};
		EXPECT_FALSE(finnhubStockBasicFinancial.IsNewData(_T("currentRatio"), valueData, vDBData)) << "数据已存在于数据库中";
		EXPECT_TRUE(finnhubStockBasicFinancial.IsNewData(_T("cashRatio"), valueData, vDBData));
		valueData.m_period = 20220101;
		EXPECT_TRUE(finnhubStockBasicFinancial.IsNewData(_T("currentRatio"), valueData, vDBData));
		EXPECT_TRUE(finnhubStockBasicFinancial.IsNewData(_T("cashRatio"), valueData, vDBData));
	}

	TEST_F(CFinnhubStockBasicFinancialTest, TestSaveQuarterData) {
		CFinnhubStockBasicFinancial instance;
		CSetFinnhubStockBasicFinancialQuarter setQuarter;
		vector<CItemOfBasicFinancialSeasonData> seasonDBData;
		CItemOfBasicFinancialSeasonData dbData;

		instance.m_symbol = _T("300020.SZ");
		instance.m_quarter.m_cashRatio.push_back({20210930, 1.0}); // 数据库中已有
		instance.m_quarter.m_cashRatio.push_back({20211231, 2.0}); // 数据库中已有
		instance.m_quarter.m_cashRatio.push_back({20220331, 3.0}); // 这个是新数据

		setQuarter.m_strFilter = _T("[Symbol] = '300020.SZ'");
		setQuarter.Open();
		while (!setQuarter.IsEOF()) {
			dbData.m_symbol = setQuarter.m_symbol;
			dbData.m_type = setQuarter.m_type;
			dbData.m_date = setQuarter.m_date;
			dbData.m_value = setQuarter.m_value;
			seasonDBData.push_back(dbData);
			setQuarter.MoveNext();
		}
		setQuarter.Close();

		setQuarter.m_strFilter = _T("[Symbol] = '300020.SZ'");
		setQuarter.Open();
		setQuarter.m_pDatabase->BeginTrans();
		instance.SaveQuarterData(setQuarter, instance.m_quarter.m_cashRatio, _T("cashRatio"), seasonDBData);
		setQuarter.m_pDatabase->CommitTrans();
		setQuarter.Close();

		// 检查数据库中的数据，并恢复原状
		setQuarter.m_strFilter = _T("[Symbol] = '300020.SZ' AND [Type] = 'cashRatio'");
		setQuarter.m_strSort = _T("[Date] ASC");
		setQuarter.Open();
		setQuarter.m_pDatabase->BeginTrans();
		setQuarter.MoveLast();
		EXPECT_EQ(setQuarter.m_date, 20220331) << "新数据存入了数据库";
		setQuarter.Delete();
		setQuarter.MovePrev();
		EXPECT_EQ(setQuarter.m_date, 20211231) << "数据库中已有此数据，无需插入新数据";
		setQuarter.MovePrev();
		EXPECT_EQ(setQuarter.m_date, 20210930) << "数据库中已有此数据， 无需插入新数据";
		setQuarter.MovePrev();
		EXPECT_EQ(setQuarter.m_date, 20210630) << "数据库中已有此数据， 无需插入新数据";
		setQuarter.MovePrev();
		setQuarter.m_pDatabase->CommitTrans();
		setQuarter.Close();
	}

	TEST_F(CFinnhubStockBasicFinancialTest, TestSaveAnnualData) {
		CFinnhubStockBasicFinancial instance;
		CSetFinnhubStockBasicFinancialAnnual setAnnual;
		vector<CItemOfBasicFinancialSeasonData> seasonDBData;
		CItemOfBasicFinancialSeasonData dbData;

		instance.m_symbol = _T("300020.SZ");
		instance.m_annual.m_cashRatio.push_back({20191231, 1.0}); // 数据库中已有
		instance.m_annual.m_cashRatio.push_back({20201231, 2.0}); // 数据库中已有
		instance.m_annual.m_cashRatio.push_back({20211231, 3.0}); // 这个是新数据

		setAnnual.m_strFilter = _T("[Symbol] = '300020.SZ'");
		setAnnual.Open();
		while (!setAnnual.IsEOF()) {
			dbData.m_symbol = setAnnual.m_symbol;
			dbData.m_type = setAnnual.m_type;
			dbData.m_date = setAnnual.m_date;
			dbData.m_value = setAnnual.m_value;
			seasonDBData.push_back(dbData);
			setAnnual.MoveNext();
		}
		setAnnual.Close();

		setAnnual.m_strFilter = _T("[Symbol] = '300020.SZ'");
		setAnnual.Open();
		setAnnual.m_pDatabase->BeginTrans();
		instance.SaveAnnualData(setAnnual, instance.m_annual.m_cashRatio, _T("cashRatio"), seasonDBData);
		setAnnual.m_pDatabase->CommitTrans();
		setAnnual.Close();

		// 检查数据库中的数据，并恢复原状
		setAnnual.m_strFilter = _T("[Symbol] = '300020.SZ' AND [Type] = 'cashRatio'");
		setAnnual.m_strSort = _T("[Date] ASC");
		setAnnual.Open();
		setAnnual.m_pDatabase->BeginTrans();
		setAnnual.MoveLast();
		EXPECT_EQ(setAnnual.m_date, 20211231) << "新数据存入了数据库";
		setAnnual.Delete();
		setAnnual.MovePrev();
		EXPECT_EQ(setAnnual.m_date, 20201231) << "数据库中已有此数据，无需插入新数据";
		setAnnual.MovePrev();
		EXPECT_EQ(setAnnual.m_date, 20191231) << "数据库中已有此数据， 无需插入新数据";
		setAnnual.MovePrev();
		EXPECT_EQ(setAnnual.m_date, 20181231) << "数据库中已有此数据， 无需插入新数据";
		setAnnual.MovePrev();
		setAnnual.m_pDatabase->CommitTrans();
		setAnnual.Close();
	}

	TEST_F(CFinnhubStockBasicFinancialTest, TestSaveAllAnnualData) {
		CFinnhubStockBasicFinancial instance, instanceLoaded;
		CSetFinnhubStockBasicFinancialAnnual setAnnual;
		vector<CItemOfBasicFinancialSeasonData> seasonDBData;
		CItemOfBasicFinancialSeasonData dbData;

		instance.m_symbol = _T("200054.SZ");
		instance.m_annual.m_cashRatio.push_back({19800101, 1.0});
		instance.m_annual.m_currentRatio.push_back({19800101, 2.0});
		instance.m_annual.m_ebitPerShare.push_back({19800101, 3.0});
		instance.m_annual.m_eps.push_back({19800101, 4.0});
		instance.m_annual.m_grossMargin.push_back({19800101, 5.0});
		instance.m_annual.m_longtermDebtTotalAsset.push_back({19800101, 6.0});
		instance.m_annual.m_longtermDebtTotalCapital.push_back({19800101, 7.0});
		instance.m_annual.m_longtermDebtTotalEquity.push_back({19800101, 8.0});
		instance.m_annual.m_netDebtToTotalCapital.push_back({19800101, 9.0});
		instance.m_annual.m_netDebtToTotalEquity.push_back({19800101, 10.0});
		instance.m_annual.m_netMargin.push_back({19800101, 11.0});
		instance.m_annual.m_operatingMargin.push_back({19800101, 12.0});
		instance.m_annual.m_pretaxMargin.push_back({19800101, 13.0});
		instance.m_annual.m_salesPerShare.push_back({19800101, 14.0});
		instance.m_annual.m_sgaToSale.push_back({19800101, 15.0});
		instance.m_annual.m_totalDebtToEquity.push_back({19800101, 16.0});
		instance.m_annual.m_totalDebtToTotalAsset.push_back({19800101, 17.0});
		instance.m_annual.m_totalDebtToTotalCapital.push_back({19800101, 18.0});
		instance.m_annual.m_totalRatio.push_back({19800101, 19.0});

		setAnnual.m_strFilter = _T("[Symbol] = '200054.SZ'");
		setAnnual.Open();
		while (!setAnnual.IsEOF()) {
			dbData.m_symbol = setAnnual.m_symbol;
			dbData.m_type = setAnnual.m_type;
			dbData.m_date = setAnnual.m_date;
			dbData.m_value = setAnnual.m_value;
			seasonDBData.push_back(dbData);
			setAnnual.MoveNext();
		}
		setAnnual.Close();

		setAnnual.m_strFilter = _T("[Symbol] = '200054.SZ'");
		setAnnual.Open();
		setAnnual.m_pDatabase->BeginTrans();
		instance.SaveAllAnnualData(setAnnual, seasonDBData);
		setAnnual.m_pDatabase->CommitTrans();
		setAnnual.Close();

		int i = 0;
		setAnnual.m_strFilter = _T("[Date] = 19800101 AND [Symbol] = '200054.SZ'");
		setAnnual.Open();
		while (!setAnnual.IsEOF()) {
			i = setAnnual.m_value;
			switch (i) {
			case 1:
				EXPECT_STREQ(setAnnual.m_type, _T("cashRatio"));
				break;
			case 2:
				EXPECT_STREQ(setAnnual.m_type, _T("currentRatio"));
				break;
			case 3:
				EXPECT_STREQ(setAnnual.m_type, _T("ebitPerShare"));
				break;
			case 4:
				EXPECT_STREQ(setAnnual.m_type, _T("eps"));
				break;
			case 5:
				EXPECT_STREQ(setAnnual.m_type, _T("grossMargin"));
				break;
			case 6:
				EXPECT_STREQ(setAnnual.m_type, _T("longtermDebtTotalAsset"));
				break;
			case 7:
				EXPECT_STREQ(setAnnual.m_type, _T("longtermDebtTotalCapital"));
				break;
			case 8:
				EXPECT_STREQ(setAnnual.m_type, _T("longtermDebtTotalEquity"));
				break;
			case 9:
				EXPECT_STREQ(setAnnual.m_type, _T("netDebtToTotalCapital"));
				break;
			case 10:
				EXPECT_STREQ(setAnnual.m_type, _T("netDebtToTotalEquity"));
				break;
			case 11:
				EXPECT_STREQ(setAnnual.m_type, _T("netMargin"));
				break;
			case 12:
				EXPECT_STREQ(setAnnual.m_type, _T("operatingMargin"));
				break;
			case 13:
				EXPECT_STREQ(setAnnual.m_type, _T("pretaxMargin"));
				break;
			case 14:
				EXPECT_STREQ(setAnnual.m_type, _T("salesPerShare"));
				break;
			case 15:
				EXPECT_STREQ(setAnnual.m_type, _T("sgaToSale"));
				break;
			case 16:
				EXPECT_STREQ(setAnnual.m_type, _T("totalDebtToEquity"));
				break;
			case 17:
				EXPECT_STREQ(setAnnual.m_type, _T("totalDebtToTotalAsset"));
				break;
			case 18:
				EXPECT_STREQ(setAnnual.m_type, _T("totalDebtToTotalCapital"));
				break;
			case 19:
				EXPECT_STREQ(setAnnual.m_type, _T("totalRatio"));

				break;
			default:
				EXPECT_TRUE(false);
				break;
			}
			setAnnual.MoveNext();
		}
		setAnnual.Close();

		// 恢复原状
		setAnnual.m_strFilter = _T("[Date] = 19800101");
		setAnnual.Open();
		setAnnual.m_pDatabase->BeginTrans();
		while (!setAnnual.IsEOF()) {
			setAnnual.Delete();
			setAnnual.MoveNext();
		}
		setAnnual.m_pDatabase->CommitTrans();
		setAnnual.Close();
	}

	TEST_F(CFinnhubStockBasicFinancialTest, TestSaveAllQuarterData) {
		CFinnhubStockBasicFinancial instance;
		CSetFinnhubStockBasicFinancialQuarter setQuarter;
		vector<CItemOfBasicFinancialSeasonData> seasonDBData;
		CItemOfBasicFinancialSeasonData dbData;

		instance.m_symbol = _T("200054.SZ");
		instance.m_quarter.m_cashRatio.push_back({19800101, 1.0});
		instance.m_quarter.m_currentRatio.push_back({19800101, 2.0});
		instance.m_quarter.m_ebitPerShare.push_back({19800101, 3.0});
		instance.m_quarter.m_eps.push_back({19800101, 4.0});
		instance.m_quarter.m_grossMargin.push_back({19800101, 5.0});
		instance.m_quarter.m_longtermDebtTotalAsset.push_back({19800101, 6.0});
		instance.m_quarter.m_longtermDebtTotalCapital.push_back({19800101, 7.0});
		instance.m_quarter.m_longtermDebtTotalEquity.push_back({19800101, 8.0});
		instance.m_quarter.m_netDebtToTotalCapital.push_back({19800101, 9.0});
		instance.m_quarter.m_netDebtToTotalEquity.push_back({19800101, 10.0});
		instance.m_quarter.m_netMargin.push_back({19800101, 11.0});
		instance.m_quarter.m_operatingMargin.push_back({19800101, 12.0});
		instance.m_quarter.m_pretaxMargin.push_back({19800101, 13.0});
		instance.m_quarter.m_salesPerShare.push_back({19800101, 14.0});
		instance.m_quarter.m_sgaToSale.push_back({19800101, 15.0});
		instance.m_quarter.m_totalDebtToEquity.push_back({19800101, 16.0});
		instance.m_quarter.m_totalDebtToTotalAsset.push_back({19800101, 17.0});
		instance.m_quarter.m_totalDebtToTotalCapital.push_back({19800101, 18.0});
		instance.m_quarter.m_totalRatio.push_back({19800101, 19.0});

		setQuarter.m_strFilter = _T("[Symbol] = '200054.SZ'");
		setQuarter.Open();
		while (!setQuarter.IsEOF()) {
			dbData.m_symbol = setQuarter.m_symbol;
			dbData.m_type = setQuarter.m_type;
			dbData.m_date = setQuarter.m_date;
			dbData.m_value = setQuarter.m_value;
			seasonDBData.push_back(dbData);
			setQuarter.MoveNext();
		}
		setQuarter.Close();

		setQuarter.m_strFilter = _T("[Symbol] = '200054.SZ'");
		setQuarter.Open();
		setQuarter.m_pDatabase->BeginTrans();
		instance.SaveAllQuarterData(setQuarter, seasonDBData);
		setQuarter.m_pDatabase->CommitTrans();
		setQuarter.Close();

		int i = 0;
		setQuarter.m_strFilter = _T("[Symbol] = '200054.SZ' AND [Date] = 19800101");
		setQuarter.Open();
		while (!setQuarter.IsEOF()) {
			i = setQuarter.m_value;
			switch (i) {
			case 1:
				EXPECT_STREQ(setQuarter.m_type, _T("cashRatio"));
				break;
			case 2:
				EXPECT_STREQ(setQuarter.m_type, _T("currentRatio"));
				break;
			case 3:
				EXPECT_STREQ(setQuarter.m_type, _T("ebitPerShare"));
				break;
			case 4:
				EXPECT_STREQ(setQuarter.m_type, _T("eps"));
				break;
			case 5:
				EXPECT_STREQ(setQuarter.m_type, _T("grossMargin"));
				break;
			case 6:
				EXPECT_STREQ(setQuarter.m_type, _T("longtermDebtTotalAsset"));
				break;
			case 7:
				EXPECT_STREQ(setQuarter.m_type, _T("longtermDebtTotalCapital"));
				break;
			case 8:
				EXPECT_STREQ(setQuarter.m_type, _T("longtermDebtTotalEquity"));
				break;
			case 9:
				EXPECT_STREQ(setQuarter.m_type, _T("netDebtToTotalCapital"));
				break;
			case 10:
				EXPECT_STREQ(setQuarter.m_type, _T("netDebtToTotalEquity"));
				break;
			case 11:
				EXPECT_STREQ(setQuarter.m_type, _T("netMargin"));
				break;
			case 12:
				EXPECT_STREQ(setQuarter.m_type, _T("operatingMargin"));
				break;
			case 13:
				EXPECT_STREQ(setQuarter.m_type, _T("pretaxMargin"));
				break;
			case 14:
				EXPECT_STREQ(setQuarter.m_type, _T("salesPerShare"));
				break;
			case 15:
				EXPECT_STREQ(setQuarter.m_type, _T("sgaToSale"));
				break;
			case 16:
				EXPECT_STREQ(setQuarter.m_type, _T("totalDebtToEquity"));
				break;
			case 17:
				EXPECT_STREQ(setQuarter.m_type, _T("totalDebtToTotalAsset"));
				break;
			case 18:
				EXPECT_STREQ(setQuarter.m_type, _T("totalDebtToTotalCapital"));
				break;
			case 19:
				EXPECT_STREQ(setQuarter.m_type, _T("totalRatio"));
				break;
			default:
				EXPECT_TRUE(false);
				break;
			}
			setQuarter.MoveNext();
		}
		setQuarter.Close();

		// 恢复原状
		setQuarter.m_strFilter = _T("[Date] = 19800101");
		setQuarter.Open();
		setQuarter.m_pDatabase->BeginTrans();
		while (!setQuarter.IsEOF()) {
			setQuarter.Delete();
			setQuarter.MoveNext();
		}
		setQuarter.m_pDatabase->CommitTrans();
		setQuarter.Close();
	}

	TEST_F(CFinnhubStockBasicFinancialTest, TestAppendQuarterData) {
		CFinnhubStockBasicFinancial instance;
		CSetFinnhubStockBasicFinancialQuarter setQuarter;
		vector<CItemOfBasicFinancialSeasonData> seasonDBData;
		CItemOfBasicFinancialSeasonData dbData;

		instance.m_symbol = _T("200054.SZ");
		instance.m_quarter.m_cashRatio.push_back({19800101, 1.0});
		instance.m_quarter.m_currentRatio.push_back({19800101, 2.0});
		instance.m_quarter.m_ebitPerShare.push_back({19800101, 3.0});
		instance.m_quarter.m_eps.push_back({19800101, 4.0});
		instance.m_quarter.m_grossMargin.push_back({19800101, 5.0});
		instance.m_quarter.m_longtermDebtTotalAsset.push_back({19800101, 6.0});
		instance.m_quarter.m_longtermDebtTotalCapital.push_back({19800101, 7.0});
		instance.m_quarter.m_longtermDebtTotalEquity.push_back({19800101, 8.0});
		instance.m_quarter.m_netDebtToTotalCapital.push_back({19800101, 9.0});
		instance.m_quarter.m_netDebtToTotalEquity.push_back({19800101, 10.0});
		instance.m_quarter.m_netMargin.push_back({19800101, 11.0});
		instance.m_quarter.m_operatingMargin.push_back({19800101, 12.0});
		instance.m_quarter.m_pretaxMargin.push_back({19800101, 13.0});
		instance.m_quarter.m_salesPerShare.push_back({19800101, 14.0});
		instance.m_quarter.m_sgaToSale.push_back({19800101, 15.0});
		instance.m_quarter.m_totalDebtToEquity.push_back({19800101, 16.0});
		instance.m_quarter.m_totalDebtToTotalAsset.push_back({19800101, 17.0});
		instance.m_quarter.m_totalDebtToTotalCapital.push_back({19800101, 18.0});
		instance.m_quarter.m_totalRatio.push_back({19800101, 19.0});

		setQuarter.m_strFilter = _T("[Symbol] = '200054.SZ'");
		setQuarter.Open();
		setQuarter.m_pDatabase->BeginTrans();
		instance.AppendQuarterData(setQuarter);
		setQuarter.m_pDatabase->CommitTrans();
		setQuarter.Close();

		int i = 0;
		setQuarter.m_strFilter = _T("[Symbol] = '200054.SZ' AND [Date] = 19800101");
		setQuarter.Open();
		while (!setQuarter.IsEOF()) {
			i = setQuarter.m_value;
			switch (i) {
			case 1:
				EXPECT_STREQ(setQuarter.m_type, _T("cashRatio"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 1.0);
				break;
			case 2:
				EXPECT_STREQ(setQuarter.m_type, _T("currentRatio"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 2.0);
				break;
			case 3:
				EXPECT_STREQ(setQuarter.m_type, _T("ebitPerShare"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 3.0);
				break;
			case 4:
				EXPECT_STREQ(setQuarter.m_type, _T("eps"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 4.0);
				break;
			case 5:
				EXPECT_STREQ(setQuarter.m_type, _T("grossMargin"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 5.0);
				break;
			case 6:
				EXPECT_STREQ(setQuarter.m_type, _T("longtermDebtTotalAsset"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 6.0);
				break;
			case 7:
				EXPECT_STREQ(setQuarter.m_type, _T("longtermDebtTotalCapital"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 7.0);
				break;
			case 8:
				EXPECT_STREQ(setQuarter.m_type, _T("longtermDebtTotalEquity"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 8.0);
				break;
			case 9:
				EXPECT_STREQ(setQuarter.m_type, _T("netDebtToTotalCapital"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 9.0);
				break;
			case 10:
				EXPECT_STREQ(setQuarter.m_type, _T("netDebtToTotalEquity"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 10.0);
				break;
			case 11:
				EXPECT_STREQ(setQuarter.m_type, _T("netMargin"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 11.0);
				break;
			case 12:
				EXPECT_STREQ(setQuarter.m_type, _T("operatingMargin"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 12.0);
				break;
			case 13:
				EXPECT_STREQ(setQuarter.m_type, _T("pretaxMargin"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 13.0);
				break;
			case 14:
				EXPECT_STREQ(setQuarter.m_type, _T("salesPerShare"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 14.0);
				break;
			case 15:
				EXPECT_STREQ(setQuarter.m_type, _T("sgaToSale"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 15.0);
				break;
			case 16:
				EXPECT_STREQ(setQuarter.m_type, _T("totalDebtToEquity"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 16.0);
				break;
			case 17:
				EXPECT_STREQ(setQuarter.m_type, _T("totalDebtToTotalAsset"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 17.0);
				break;
			case 18:
				EXPECT_STREQ(setQuarter.m_type, _T("totalDebtToTotalCapital"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 18.0);
				break;
			case 19:
				EXPECT_STREQ(setQuarter.m_type, _T("totalRatio"));
				EXPECT_DOUBLE_EQ(setQuarter.m_value, 19.0);
				break;
			default:
				EXPECT_TRUE(false);
				break;
			}
			setQuarter.MoveNext();
		}
		setQuarter.Close();

		// 恢复原状
		setQuarter.m_strFilter = _T("[Date] = 19800101");
		setQuarter.Open();
		setQuarter.m_pDatabase->BeginTrans();
		while (!setQuarter.IsEOF()) {
			setQuarter.Delete();
			setQuarter.MoveNext();
		}
		setQuarter.m_pDatabase->CommitTrans();
		setQuarter.Close();
	}

	TEST_F(CFinnhubStockBasicFinancialTest, TestAppendAnnualData) {
		CFinnhubStockBasicFinancial instance, instanceLoaded;
		CSetFinnhubStockBasicFinancialAnnual setAnnual;
		vector<CItemOfBasicFinancialSeasonData> seasonDBData;
		CItemOfBasicFinancialSeasonData dbData;

		instance.m_symbol = _T("200054.SZ");
		instance.m_annual.m_cashRatio.push_back({19800101, 1.0});
		instance.m_annual.m_currentRatio.push_back({19800101, 2.0});
		instance.m_annual.m_ebitPerShare.push_back({19800101, 3.0});
		instance.m_annual.m_eps.push_back({19800101, 4.0});
		instance.m_annual.m_grossMargin.push_back({19800101, 5.0});
		instance.m_annual.m_longtermDebtTotalAsset.push_back({19800101, 6.0});
		instance.m_annual.m_longtermDebtTotalCapital.push_back({19800101, 7.0});
		instance.m_annual.m_longtermDebtTotalEquity.push_back({19800101, 8.0});
		instance.m_annual.m_netDebtToTotalCapital.push_back({19800101, 9.0});
		instance.m_annual.m_netDebtToTotalEquity.push_back({19800101, 10.0});
		instance.m_annual.m_netMargin.push_back({19800101, 11.0});
		instance.m_annual.m_operatingMargin.push_back({19800101, 12.0});
		instance.m_annual.m_pretaxMargin.push_back({19800101, 13.0});
		instance.m_annual.m_salesPerShare.push_back({19800101, 14.0});
		instance.m_annual.m_sgaToSale.push_back({19800101, 15.0});
		instance.m_annual.m_totalDebtToEquity.push_back({19800101, 16.0});
		instance.m_annual.m_totalDebtToTotalAsset.push_back({19800101, 17.0});
		instance.m_annual.m_totalDebtToTotalCapital.push_back({19800101, 18.0});
		instance.m_annual.m_totalRatio.push_back({19800101, 19.0});

		setAnnual.m_strFilter = _T("[Symbol] = '200054.SZ'");
		setAnnual.Open();
		setAnnual.m_pDatabase->BeginTrans();
		instance.AppendAnnualData(setAnnual);
		setAnnual.m_pDatabase->CommitTrans();
		setAnnual.Close();

		int i = 0;
		setAnnual.m_strFilter = _T("[Date] = 19800101 AND [Symbol] = '200054.SZ'");
		setAnnual.Open();
		while (!setAnnual.IsEOF()) {
			i = setAnnual.m_value;
			switch (i) {
			case 1:
				EXPECT_STREQ(setAnnual.m_type, _T("cashRatio"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 1.0);
				break;
			case 2:
				EXPECT_STREQ(setAnnual.m_type, _T("currentRatio"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 2.0);
				break;
			case 3:
				EXPECT_STREQ(setAnnual.m_type, _T("ebitPerShare"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 3.0);
				break;
			case 4:
				EXPECT_STREQ(setAnnual.m_type, _T("eps"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 4.0);
				break;
			case 5:
				EXPECT_STREQ(setAnnual.m_type, _T("grossMargin"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 5.0);
				break;
			case 6:
				EXPECT_STREQ(setAnnual.m_type, _T("longtermDebtTotalAsset"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 6.0);
				break;
			case 7:
				EXPECT_STREQ(setAnnual.m_type, _T("longtermDebtTotalCapital"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 7.0);
				break;
			case 8:
				EXPECT_STREQ(setAnnual.m_type, _T("longtermDebtTotalEquity"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 8.0);
				break;
			case 9:
				EXPECT_STREQ(setAnnual.m_type, _T("netDebtToTotalCapital"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 9.0);
				break;
			case 10:
				EXPECT_STREQ(setAnnual.m_type, _T("netDebtToTotalEquity"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 10.0);
				break;
			case 11:
				EXPECT_STREQ(setAnnual.m_type, _T("netMargin"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 11.0);
				break;
			case 12:
				EXPECT_STREQ(setAnnual.m_type, _T("operatingMargin"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 12.0);
				break;
			case 13:
				EXPECT_STREQ(setAnnual.m_type, _T("pretaxMargin"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 13.0);
				break;
			case 14:
				EXPECT_STREQ(setAnnual.m_type, _T("salesPerShare"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 14.0);
				break;
			case 15:
				EXPECT_STREQ(setAnnual.m_type, _T("sgaToSale"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 15.0);
				break;
			case 16:
				EXPECT_STREQ(setAnnual.m_type, _T("totalDebtToEquity"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 16.0);
				break;
			case 17:
				EXPECT_STREQ(setAnnual.m_type, _T("totalDebtToTotalAsset"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 17.0);
				break;
			case 18:
				EXPECT_STREQ(setAnnual.m_type, _T("totalDebtToTotalCapital"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 18.0);
				break;
			case 19:
				EXPECT_STREQ(setAnnual.m_type, _T("totalRatio"));
				EXPECT_DOUBLE_EQ(setAnnual.m_value, 19.0);
				break;
			default:
				EXPECT_TRUE(false);
				break;
			}
			setAnnual.MoveNext();
		}
		setAnnual.Close();

		// 恢复原状
		setAnnual.m_strFilter = _T("[Date] = 19800101");
		setAnnual.Open();
		setAnnual.m_pDatabase->BeginTrans();
		while (!setAnnual.IsEOF()) {
			setAnnual.Delete();
			setAnnual.MoveNext();
		}
		setAnnual.m_pDatabase->CommitTrans();
		setAnnual.Close();
	}

	TEST_F(CFinnhubStockBasicFinancialTest, Test_LoadSaveMetric) {
		CFinnhubStockBasicFinancial instance, instanceLoaded;
		CSetFinnhubStockBasicFinancialMetric setMetric, setMetricLoad, setMetric2;

		instance.m_symbol = _T("NEW_CODE");

		instance.m_10DayAverageTradingVolume = 1;
		instance.m_13WeekPriceReturnDaily = 2;
		instance.m_26WeekPriceReturnDaily = 3;
		instance.m_52WeekPriceReturnDaily = 4;
		instance.m_3MonthAverageTradingVolume = 5;
		instance.m_52WeekHigh = 6;
		instance.m_52WeekHighDate = 7;
		instance.m_52WeekLow = 8;
		instance.m_52WeekLowDate = 9;
		instance.m_5DayPriceReturnDaily = 10;

		instance.m_assetTurnoverAnnual = 11;
		instance.m_assetTurnoverTTM = 12;

		instance.m_bookValuePerShareAnnual = 13;
		instance.m_bookValuePerShareQuarterly = 14;
		instance.m_bookValueShareGrowth5Y = 15;
		instance.m_beta = 16;

		instance.m_capitalSpendingGrowth5Y = 17;
		instance.m_cashFlowPerShareAnnual = 18;
		instance.m_cashFlowPerShareTTM = 19;
		instance.m_cashPerSharePerShareAnnual = 20;
		instance.m_cashPerSharePerShareQuarterly = 21;
		instance.m_currentEV_freeCashFlowAnnual = 22;
		instance.m_currentEV_freeCashFlowTTM = 23;
		instance.m_currentDividendYieldTTM = 24;
		instance.m_currentRatioAnnual = 25;
		instance.m_currentRatioQuarterly = 26;

		instance.m_dividendGrowthRate5Y = 27;
		instance.m_dividendPerShare5Y = 28;
		instance.m_dividendPerShareAnnual = 29;
		instance.m_dividendsPerShareTTM = 30;
		instance.m_dividendYield5Y = 31;
		instance.m_dividendYieldIndicatedAnnual = 32;

		instance.m_ebitdaCagr5Y = 33;
		instance.m_ebitdaInterimCagr5Y = 34;
		instance.m_ebitdPerShareTTM = 35;
		instance.m_epsBasicExclExtraItemsAnnual = 36;
		instance.m_epsBasicExclExtraItemsTTM = 37;
		instance.m_epsExclExtraItemsAnnual = 38;
		instance.m_epsExclExtraItemsTTM = 39;
		instance.m_epsGrowth3Y = 40;
		instance.m_epsGrowth5Y = 41;
		instance.m_epsGrowthQuarterlyYoy = 42;
		instance.m_epsGrowthTTMYoy = 43;
		instance.m_epsInclExtraItemsAnnual = 44;
		instance.m_epsInclExtraItemsTTM = 45;
		instance.m_epsNormalizedAnnual = 46;

		instance.m_focfCagr5Y = 47;
		instance.m_freeCashFlowAnnual = 48;
		instance.m_freeCashFlowPerShareTTM = 49;
		instance.m_freeCashFlowTTM = 50;
		instance.m_freeOperatingCashFlow_revenue5Y = 51;
		instance.m_freeOperatingCashFlow_revenueTTM = 52;

		instance.m_grossMargin5Y = 53;
		instance.m_grossMarginAnnual = 54;
		instance.m_grossMarginTTM = 55;

		instance.m_inventoryTurnoverAnnual = 56;
		instance.m_inventoryTurnoverTTM = 57;

		instance.m_longTermDebt_equityAnnual = 58;
		instance.m_longTermDebt_equityQuarterly = 59;

		instance.m_marketCapitalization = 60;
		instance.m_monthToDatePriceReturnDaily = 61;

		instance.m_netDebtAnnual = 62;
		instance.m_netDebtInterim = 63;
		instance.m_netIncomeEmployeeAnnual = 64;
		instance.m_netIncomeEmployeeTTM = 65;
		instance.m_netInterestCoverageAnnual = 66;
		instance.m_netInterestCoverageTTM = 67;
		instance.m_netMarginGrowth5Y = 68;
		instance.m_netProfitMargin5Y = 69;
		instance.m_netProfitMarginAnnual = 70;
		instance.m_netProfitMarginTTM = 71;

		instance.m_operatingMargin5Y = 72;
		instance.m_operatingMarginAnnual = 73;
		instance.m_operatingMarginTTM = 74;

		instance.m_payoutRatioAnnual = 75;
		instance.m_payoutRatioTTM = 76;
		instance.m_pbAnnual = 77;
		instance.m_pbQuarterly = 78;
		instance.m_pcfShareTTM = 79;
		instance.m_peBasicExclExtraTTM = 80;
		instance.m_peExclExtraAnnual = 81;
		instance.m_peExclExtraHighTTM = 82;
		instance.m_peExclExtraTTM = 83;
		instance.m_peExclLowTTM = 84;
		instance.m_peInclExtraTTM = 85;
		instance.m_peNormalizedAnnual = 86;
		instance.m_pfcfShareTTM = 87;
		instance.m_pfcfShareAnnual = 88;
		instance.m_pretaxMargin5Y = 89;
		instance.m_pretaxMarginAnnual = 90;
		instance.m_pretaxMarginTTM = 91;
		instance.m_priceRelativeToSP50013Week = 92;
		instance.m_priceRelativeToSP50026Week = 93;
		instance.m_priceRelativeToSP5004Week = 94;
		instance.m_priceRelativeToSP50052Week = 95;
		instance.m_priceRelativeToSP500Ytd = 96;
		instance.m_psAnnual = 97;
		instance.m_psTTM = 98;
		instance.m_ptbvAnnual = 99;
		instance.m_ptbvQuarterly = 100;

		instance.m_quickRatioAnnual = 101;
		instance.m_quickRatioQuarterly = 102;

		instance.m_receivablesTurnoverAnnual = 103;
		instance.m_receivablesTurnoverTTM = 104;
		instance.m_revenueEmployeeAnnual = 105;
		instance.m_revenueEmployeeTTM = 106;
		instance.m_revenueGrowth3Y = 107;
		instance.m_revenueGrowth5Y = 108;
		instance.m_revenueGrowthQuarterlyYoy = 109;
		instance.m_revenueGrowthTTMYoy = 110;
		instance.m_revenuePerShareAnnual = 111;
		instance.m_revenuePerShareTTM = 112;
		instance.m_revenueShareGrowth5Y = 113;
		instance.m_roaa5Y = 114;
		instance.m_roae5Y = 115;
		instance.m_roaeTTM = 116;
		instance.m_roaRfy = 117;
		instance.m_roeRfy = 118;
		instance.m_roeTTM = 119;
		instance.m_roi5Y = 120;
		instance.m_roiAnnual = 121;
		instance.m_roiTTM = 122;

		instance.m_tangibleBookValuePerShareAnnual = 123;
		instance.m_tangibleBookValuePerShareQuarterly = 124;
		instance.m_tbvCagr5Y = 125;
		instance.m_totalDebtCagr5Y = 126;
		instance.m_totalDebt_totalEquityAnnual = 127;
		instance.m_totalDebt_totalEquityQuarterly = 128;

		instance.m_yearToDatePriceReturnDaily = 129;

		setMetric.Open();
		setMetric.m_pDatabase->BeginTrans();
		instance.AppendMetric(setMetric);
		setMetric.m_pDatabase->CommitTrans();
		setMetric.Close();

		setMetricLoad.m_strFilter = _T("[Symbol] = 'NEW_CODE'");
		setMetricLoad.Open();
		instanceLoaded.LoadMetric(setMetricLoad);
		setMetricLoad.Close();

		EXPECT_DOUBLE_EQ(instance.m_10DayAverageTradingVolume, instanceLoaded.m_10DayAverageTradingVolume);
		EXPECT_DOUBLE_EQ(instance.m_13WeekPriceReturnDaily, instanceLoaded.m_13WeekPriceReturnDaily);
		EXPECT_DOUBLE_EQ(instance.m_26WeekPriceReturnDaily, instanceLoaded.m_26WeekPriceReturnDaily);
		EXPECT_DOUBLE_EQ(instance.m_3MonthAverageTradingVolume, instanceLoaded.m_3MonthAverageTradingVolume);
		EXPECT_DOUBLE_EQ(instance.m_52WeekPriceReturnDaily, instanceLoaded.m_52WeekPriceReturnDaily);
		EXPECT_DOUBLE_EQ(instance.m_52WeekHigh, instanceLoaded.m_52WeekHigh);
		EXPECT_EQ(instance.m_52WeekHighDate, instanceLoaded.m_52WeekHighDate);
		EXPECT_DOUBLE_EQ(instance.m_52WeekLow, instanceLoaded.m_52WeekLow);
		EXPECT_EQ(instance.m_52WeekLowDate, instanceLoaded.m_52WeekLowDate);
		EXPECT_DOUBLE_EQ(instance.m_5DayPriceReturnDaily, instanceLoaded.m_5DayPriceReturnDaily);

		EXPECT_DOUBLE_EQ(instance.m_assetTurnoverAnnual, instanceLoaded.m_assetTurnoverAnnual);
		EXPECT_DOUBLE_EQ(instance.m_assetTurnoverTTM, instanceLoaded.m_assetTurnoverTTM);

		EXPECT_DOUBLE_EQ(instance.m_beta, instanceLoaded.m_beta);
		EXPECT_DOUBLE_EQ(instance.m_bookValuePerShareAnnual, instanceLoaded.m_bookValuePerShareAnnual);
		EXPECT_DOUBLE_EQ(instance.m_bookValuePerShareQuarterly, instanceLoaded.m_bookValuePerShareQuarterly);
		EXPECT_DOUBLE_EQ(instance.m_bookValueShareGrowth5Y, instanceLoaded.m_bookValueShareGrowth5Y);

		EXPECT_DOUBLE_EQ(instance.m_capitalSpendingGrowth5Y, instanceLoaded.m_capitalSpendingGrowth5Y);
		EXPECT_DOUBLE_EQ(instance.m_cashFlowPerShareAnnual, instanceLoaded.m_cashFlowPerShareAnnual);
		EXPECT_DOUBLE_EQ(instance.m_cashFlowPerShareTTM, instanceLoaded.m_cashFlowPerShareTTM);
		EXPECT_DOUBLE_EQ(instance.m_cashPerSharePerShareAnnual, instanceLoaded.m_cashPerSharePerShareAnnual);
		EXPECT_DOUBLE_EQ(instance.m_cashPerSharePerShareQuarterly, instanceLoaded.m_cashPerSharePerShareQuarterly);
		EXPECT_DOUBLE_EQ(instance.m_currentEV_freeCashFlowAnnual, instanceLoaded.m_currentEV_freeCashFlowAnnual);
		EXPECT_DOUBLE_EQ(instance.m_currentEV_freeCashFlowTTM, instanceLoaded.m_currentEV_freeCashFlowTTM);
		EXPECT_DOUBLE_EQ(instance.m_currentDividendYieldTTM, instanceLoaded.m_currentDividendYieldTTM);
		EXPECT_DOUBLE_EQ(instance.m_currentRatioAnnual, instanceLoaded.m_currentRatioAnnual);
		EXPECT_DOUBLE_EQ(instance.m_currentRatioQuarterly, instanceLoaded.m_currentRatioQuarterly);

		EXPECT_DOUBLE_EQ(instance.m_dividendGrowthRate5Y, instanceLoaded.m_dividendGrowthRate5Y);
		EXPECT_DOUBLE_EQ(instance.m_dividendPerShare5Y, instanceLoaded.m_dividendPerShare5Y);
		EXPECT_DOUBLE_EQ(instance.m_dividendPerShareAnnual, instanceLoaded.m_dividendPerShareAnnual);
		EXPECT_DOUBLE_EQ(instance.m_dividendsPerShareTTM, instanceLoaded.m_dividendsPerShareTTM);
		EXPECT_DOUBLE_EQ(instance.m_dividendYield5Y, instanceLoaded.m_dividendYield5Y);
		EXPECT_DOUBLE_EQ(instance.m_dividendYieldIndicatedAnnual, instanceLoaded.m_dividendYieldIndicatedAnnual);

		EXPECT_DOUBLE_EQ(instance.m_ebitdaCagr5Y, instanceLoaded.m_ebitdaCagr5Y);
		EXPECT_DOUBLE_EQ(instance.m_ebitdaInterimCagr5Y, instanceLoaded.m_ebitdaInterimCagr5Y);
		EXPECT_DOUBLE_EQ(instance.m_ebitdPerShareTTM, instanceLoaded.m_ebitdPerShareTTM);
		EXPECT_DOUBLE_EQ(instance.m_epsBasicExclExtraItemsAnnual, instanceLoaded.m_epsBasicExclExtraItemsAnnual);
		EXPECT_DOUBLE_EQ(instance.m_epsBasicExclExtraItemsTTM, instanceLoaded.m_epsBasicExclExtraItemsTTM);
		EXPECT_DOUBLE_EQ(instance.m_epsExclExtraItemsAnnual, instanceLoaded.m_epsExclExtraItemsAnnual);
		EXPECT_DOUBLE_EQ(instance.m_epsExclExtraItemsTTM, instanceLoaded.m_epsExclExtraItemsTTM);
		EXPECT_DOUBLE_EQ(instance.m_epsGrowth3Y, instanceLoaded.m_epsGrowth3Y);
		EXPECT_DOUBLE_EQ(instance.m_epsGrowth5Y, instanceLoaded.m_epsGrowth5Y);
		EXPECT_DOUBLE_EQ(instance.m_epsGrowthQuarterlyYoy, instanceLoaded.m_epsGrowthQuarterlyYoy);
		EXPECT_DOUBLE_EQ(instance.m_epsGrowthTTMYoy, instanceLoaded.m_epsGrowthTTMYoy);
		EXPECT_DOUBLE_EQ(instance.m_epsInclExtraItemsAnnual, instanceLoaded.m_epsInclExtraItemsAnnual);
		EXPECT_DOUBLE_EQ(instance.m_epsInclExtraItemsTTM, instanceLoaded.m_epsInclExtraItemsTTM);
		EXPECT_DOUBLE_EQ(instance.m_epsNormalizedAnnual, instanceLoaded.m_epsNormalizedAnnual);

		EXPECT_DOUBLE_EQ(instance.m_focfCagr5Y, instanceLoaded.m_focfCagr5Y);
		EXPECT_DOUBLE_EQ(instance.m_freeCashFlowAnnual, instanceLoaded.m_freeCashFlowAnnual);
		EXPECT_DOUBLE_EQ(instance.m_freeCashFlowPerShareTTM, instanceLoaded.m_freeCashFlowPerShareTTM);
		EXPECT_DOUBLE_EQ(instance.m_freeCashFlowTTM, instanceLoaded.m_freeCashFlowTTM);
		EXPECT_DOUBLE_EQ(instance.m_freeOperatingCashFlow_revenue5Y, instanceLoaded.m_freeOperatingCashFlow_revenue5Y);
		EXPECT_DOUBLE_EQ(instance.m_freeOperatingCashFlow_revenueTTM, instanceLoaded.m_freeOperatingCashFlow_revenueTTM);

		EXPECT_DOUBLE_EQ(instance.m_grossMargin5Y, instanceLoaded.m_grossMargin5Y);
		EXPECT_DOUBLE_EQ(instance.m_grossMarginAnnual, instanceLoaded.m_grossMarginAnnual);
		EXPECT_DOUBLE_EQ(instance.m_grossMarginTTM, instanceLoaded.m_grossMarginTTM);

		EXPECT_DOUBLE_EQ(instance.m_inventoryTurnoverAnnual, instanceLoaded.m_inventoryTurnoverAnnual);
		EXPECT_DOUBLE_EQ(instance.m_inventoryTurnoverTTM, instanceLoaded.m_inventoryTurnoverTTM);

		EXPECT_DOUBLE_EQ(instance.m_longTermDebt_equityAnnual, instanceLoaded.m_longTermDebt_equityAnnual);
		EXPECT_DOUBLE_EQ(instance.m_longTermDebt_equityQuarterly, instanceLoaded.m_longTermDebt_equityQuarterly);

		EXPECT_DOUBLE_EQ(instance.m_marketCapitalization, instanceLoaded.m_marketCapitalization);
		EXPECT_DOUBLE_EQ(instance.m_monthToDatePriceReturnDaily, instanceLoaded.m_monthToDatePriceReturnDaily);

		EXPECT_DOUBLE_EQ(instance.m_netDebtAnnual, instanceLoaded.m_netDebtAnnual);
		EXPECT_DOUBLE_EQ(instance.m_netDebtInterim, instanceLoaded.m_netDebtInterim);
		EXPECT_DOUBLE_EQ(instance.m_netIncomeEmployeeAnnual, instanceLoaded.m_netIncomeEmployeeAnnual);
		EXPECT_DOUBLE_EQ(instance.m_netIncomeEmployeeTTM, instanceLoaded.m_netIncomeEmployeeTTM);
		EXPECT_DOUBLE_EQ(instance.m_netInterestCoverageAnnual, instanceLoaded.m_netInterestCoverageAnnual);
		EXPECT_DOUBLE_EQ(instance.m_netInterestCoverageTTM, instanceLoaded.m_netInterestCoverageTTM);
		EXPECT_DOUBLE_EQ(instance.m_netMarginGrowth5Y, instanceLoaded.m_netMarginGrowth5Y);
		EXPECT_DOUBLE_EQ(instance.m_netProfitMargin5Y, instanceLoaded.m_netProfitMargin5Y);
		EXPECT_DOUBLE_EQ(instance.m_netProfitMarginAnnual, instanceLoaded.m_netProfitMarginAnnual);
		EXPECT_DOUBLE_EQ(instance.m_netProfitMarginTTM, instanceLoaded.m_netProfitMarginTTM);

		EXPECT_DOUBLE_EQ(instance.m_operatingMargin5Y, instanceLoaded.m_operatingMargin5Y);
		EXPECT_DOUBLE_EQ(instance.m_operatingMarginAnnual, instanceLoaded.m_operatingMarginAnnual);
		EXPECT_DOUBLE_EQ(instance.m_operatingMarginTTM, instanceLoaded.m_operatingMarginTTM);

		EXPECT_DOUBLE_EQ(instance.m_payoutRatioAnnual, instanceLoaded.m_payoutRatioAnnual);
		EXPECT_DOUBLE_EQ(instance.m_payoutRatioTTM, instanceLoaded.m_payoutRatioTTM);
		EXPECT_DOUBLE_EQ(instance.m_pbAnnual, instanceLoaded.m_pbAnnual);
		EXPECT_DOUBLE_EQ(instance.m_pbQuarterly, instanceLoaded.m_pbQuarterly);
		EXPECT_DOUBLE_EQ(instance.m_peBasicExclExtraTTM, instanceLoaded.m_peBasicExclExtraTTM);
		EXPECT_DOUBLE_EQ(instance.m_peExclExtraAnnual, instanceLoaded.m_peExclExtraAnnual);
		EXPECT_DOUBLE_EQ(instance.m_peExclExtraHighTTM, instanceLoaded.m_peExclExtraHighTTM);
		EXPECT_DOUBLE_EQ(instance.m_peExclExtraTTM, instanceLoaded.m_peExclExtraTTM);
		EXPECT_DOUBLE_EQ(instance.m_peExclLowTTM, instanceLoaded.m_peExclLowTTM);
		EXPECT_DOUBLE_EQ(instance.m_peInclExtraTTM, instanceLoaded.m_peInclExtraTTM);
		EXPECT_DOUBLE_EQ(instance.m_peNormalizedAnnual, instanceLoaded.m_peNormalizedAnnual);
		EXPECT_DOUBLE_EQ(instance.m_pcfShareTTM, instanceLoaded.m_pcfShareTTM);
		EXPECT_DOUBLE_EQ(instance.m_pfcfShareAnnual, instanceLoaded.m_pfcfShareAnnual);
		EXPECT_DOUBLE_EQ(instance.m_pfcfShareTTM, instanceLoaded.m_pfcfShareTTM);
		EXPECT_DOUBLE_EQ(instance.m_pretaxMargin5Y, instanceLoaded.m_pretaxMargin5Y);
		EXPECT_DOUBLE_EQ(instance.m_pretaxMarginAnnual, instanceLoaded.m_pretaxMarginAnnual);
		EXPECT_DOUBLE_EQ(instance.m_pretaxMarginTTM, instanceLoaded.m_pretaxMarginTTM);
		EXPECT_DOUBLE_EQ(instance.m_priceRelativeToSP50013Week, instanceLoaded.m_priceRelativeToSP50013Week);
		EXPECT_DOUBLE_EQ(instance.m_priceRelativeToSP50026Week, instanceLoaded.m_priceRelativeToSP50026Week);
		EXPECT_DOUBLE_EQ(instance.m_priceRelativeToSP5004Week, instanceLoaded.m_priceRelativeToSP5004Week);
		EXPECT_DOUBLE_EQ(instance.m_priceRelativeToSP50052Week, instanceLoaded.m_priceRelativeToSP50052Week);
		EXPECT_DOUBLE_EQ(instance.m_priceRelativeToSP500Ytd, instanceLoaded.m_priceRelativeToSP500Ytd);
		EXPECT_DOUBLE_EQ(instance.m_psAnnual, instanceLoaded.m_psAnnual);
		EXPECT_DOUBLE_EQ(instance.m_psTTM, instanceLoaded.m_psTTM);
		EXPECT_DOUBLE_EQ(instance.m_ptbvAnnual, instanceLoaded.m_ptbvAnnual);
		EXPECT_DOUBLE_EQ(instance.m_ptbvQuarterly, instanceLoaded.m_ptbvQuarterly);

		EXPECT_DOUBLE_EQ(instance.m_quickRatioAnnual, instanceLoaded.m_quickRatioAnnual);
		EXPECT_DOUBLE_EQ(instance.m_quickRatioQuarterly, instanceLoaded.m_quickRatioQuarterly);

		EXPECT_DOUBLE_EQ(instance.m_receivablesTurnoverAnnual, instanceLoaded.m_receivablesTurnoverAnnual);
		EXPECT_DOUBLE_EQ(instance.m_receivablesTurnoverTTM, instanceLoaded.m_receivablesTurnoverTTM);
		EXPECT_DOUBLE_EQ(instance.m_revenueEmployeeAnnual, instanceLoaded.m_revenueEmployeeAnnual);
		EXPECT_DOUBLE_EQ(instance.m_revenueEmployeeTTM, instanceLoaded.m_revenueEmployeeTTM);
		EXPECT_DOUBLE_EQ(instance.m_revenueGrowth3Y, instanceLoaded.m_revenueGrowth3Y);
		EXPECT_DOUBLE_EQ(instance.m_revenueGrowth5Y, instanceLoaded.m_revenueGrowth5Y);
		EXPECT_DOUBLE_EQ(instance.m_revenueGrowthQuarterlyYoy, instanceLoaded.m_revenueGrowthQuarterlyYoy);
		EXPECT_DOUBLE_EQ(instance.m_revenueGrowthTTMYoy, instanceLoaded.m_revenueGrowthTTMYoy);
		EXPECT_DOUBLE_EQ(instance.m_revenuePerShareAnnual, instanceLoaded.m_revenuePerShareAnnual);
		EXPECT_DOUBLE_EQ(instance.m_revenuePerShareTTM, instanceLoaded.m_revenuePerShareTTM);
		EXPECT_DOUBLE_EQ(instance.m_revenueShareGrowth5Y, instanceLoaded.m_revenueShareGrowth5Y);
		EXPECT_DOUBLE_EQ(instance.m_roaa5Y, instanceLoaded.m_roaa5Y);
		EXPECT_DOUBLE_EQ(instance.m_roae5Y, instanceLoaded.m_roae5Y);
		EXPECT_DOUBLE_EQ(instance.m_roaeTTM, instanceLoaded.m_roaeTTM);
		EXPECT_DOUBLE_EQ(instance.m_roaRfy, instanceLoaded.m_roaRfy);
		EXPECT_DOUBLE_EQ(instance.m_roeRfy, instanceLoaded.m_roeRfy);
		EXPECT_DOUBLE_EQ(instance.m_roeTTM, instanceLoaded.m_roeTTM);
		EXPECT_DOUBLE_EQ(instance.m_roi5Y, instanceLoaded.m_roi5Y);
		EXPECT_DOUBLE_EQ(instance.m_roiAnnual, instanceLoaded.m_roiAnnual);
		EXPECT_DOUBLE_EQ(instance.m_roiTTM, instanceLoaded.m_roiTTM);

		EXPECT_DOUBLE_EQ(instance.m_tangibleBookValuePerShareAnnual, instanceLoaded.m_tangibleBookValuePerShareAnnual);
		EXPECT_DOUBLE_EQ(instance.m_tangibleBookValuePerShareQuarterly, instanceLoaded.m_tangibleBookValuePerShareQuarterly);
		EXPECT_DOUBLE_EQ(instance.m_tbvCagr5Y, instanceLoaded.m_tbvCagr5Y);
		EXPECT_DOUBLE_EQ(instance.m_totalDebtCagr5Y, instanceLoaded.m_totalDebtCagr5Y);
		EXPECT_DOUBLE_EQ(instance.m_totalDebt_totalEquityAnnual, instanceLoaded.m_totalDebt_totalEquityAnnual);
		EXPECT_DOUBLE_EQ(instance.m_totalDebt_totalEquityQuarterly, instanceLoaded.m_totalDebt_totalEquityQuarterly);

		EXPECT_DOUBLE_EQ(instance.m_yearToDatePriceReturnDaily, instanceLoaded.m_yearToDatePriceReturnDaily);

		// restore default
		setMetric2.m_strFilter = _T("[Symbol] = 'NEW_CODE'");
		setMetric2.Open();
		setMetric2.m_pDatabase->BeginTrans();
		while (!setMetric2.IsEOF()) {
			setMetric2.Delete();
			setMetric2.MoveNext();
		}
		setMetric2.m_pDatabase->CommitTrans();
		setMetric2.Close();
	}
}
