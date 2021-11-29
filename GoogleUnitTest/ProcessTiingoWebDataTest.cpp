#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"WebInquirer.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	struct TiingoWebData {
		TiingoWebData(long lIndex, CString strSymbol, CString strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = make_shared<CWebData>();
			m_pData->__TEST_SetBuffer__(strData);
		}

		~TiingoWebData() {
		}

	public:
		long m_lIndex;
		CString m_strSymbol;
		CWebDataPtr m_pData;
	};

	// 格式不对，缺乏'{'
	TiingoWebData tiingoWebData1(1, _T(""), _T("[\"permaTicker\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 第一个数据缺项
	TiingoWebData tiingoWebData2(2, _T(""), _T("[{\"Missing\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 第二个数据缺项
	TiingoWebData tiingoWebData3(3, _T(""), _T("[{\"permaTicker\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"Missing\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 正确的数据
	TiingoWebData tiingoWebData4(4, _T(""), _T("[{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"sector have data\",\"industry\":\"industry have data\",\"sicCode\":\"1234\",\"sicSector\":\"sicSector have data\",\"sicIndustry\":\"sicIndustry have data\",\"reportingCurrency\":\"usd\",\"location\":\"location have data\",\"companyWebsite\":\"companyWebsite have data\",\"secFilingWebsite\":\"secFileingWebsite have data\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 正确的数据
	TiingoWebData tiingoWebData10(10, _T(""), _T("[{\"permaTicker\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));

	class ProcessTiingoStockTest : public::testing::TestWithParam<TiingoWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_vStock.resize(0);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		vector<CTiingoStockPtr> m_vStock;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStock1,
		ProcessTiingoStockTest,
		testing::Values(&tiingoWebData1, &tiingoWebData2,
			&tiingoWebData3, &tiingoWebData4, &tiingoWebData10));

	TEST_P(ProcessTiingoStockTest, TestProcessStockProfile0) {
		bool fSucceed = false;
		fSucceed = gl_pWorldMarket->ProcessTiingoStockSymbol(m_pWebData, m_vStock);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_vStock.size(), 0);
			break;
		case 2: // 格式不对
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_vStock.size(), 0);
			break;
		case 3: // 缺乏address项
			EXPECT_FALSE(fSucceed);
			EXPECT_EQ(m_vStock.size(), 1);
			break;
		case 4:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_vStock.size(), 1);
			EXPECT_STREQ(m_vStock.at(0)->m_strTiingoPermaTicker, _T("US000000000091"));
			EXPECT_STREQ(m_vStock.at(0)->m_strTicker, _T("AA"));
			EXPECT_STREQ(m_vStock.at(0)->m_strName, _T("Alcoa Corp"));
			EXPECT_TRUE(m_vStock.at(0)->m_fIsActive);
			EXPECT_FALSE(m_vStock.at(0)->m_fIsADR);
			EXPECT_STREQ(m_vStock.at(0)->m_strTiingoIndustry, _T("industry have data"));
			EXPECT_STREQ(m_vStock.at(0)->m_strTiingoSector, _T("sector have data"));
			EXPECT_STREQ(m_vStock.at(0)->m_strSICIndustry, _T("sicIndustry have data"));
			EXPECT_STREQ(m_vStock.at(0)->m_strSICSector, _T("sicSector have data"));
			EXPECT_STREQ(m_vStock.at(0)->m_strReportingCurrency, _T("usd"));
			EXPECT_STREQ(m_vStock.at(0)->m_strLocation, _T("location have data"));
			EXPECT_STREQ(m_vStock.at(0)->m_strCompanyWebSite, _T("companyWebsite have data"));
			EXPECT_STREQ(m_vStock.at(0)->m_strSECFilingWebSite, _T("secFileingWebsite have data"));
			EXPECT_EQ(m_vStock.at(0)->m_lStatementUpdateDate, 20210302);
			EXPECT_EQ(m_vStock.at(0)->m_lDailyDataUpdateDate, 20210312);
			EXPECT_EQ(m_vStock.at(0)->m_iSICCode, 1234);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(m_vStock.size(), 2);
			EXPECT_STREQ(m_vStock.at(1)->m_strTiingoPermaTicker, _T("US000000000091"));
			EXPECT_STREQ(m_vStock.at(1)->m_strTicker, _T("AA"));
			EXPECT_STREQ(m_vStock.at(1)->m_strName, _T("Alcoa Corp"));
			EXPECT_TRUE(m_vStock.at(1)->m_fIsActive);
			EXPECT_FALSE(m_vStock.at(1)->m_fIsADR);
			EXPECT_STREQ(m_vStock.at(1)->m_strTiingoIndustry, _T(" ")) << "当字符串为Field not available for free/evcaluation时，返回空串(一个空格)";
			EXPECT_STREQ(m_vStock.at(1)->m_strTiingoSector, _T(" "));
			EXPECT_EQ(m_vStock.at(1)->m_iSICCode, 0);
			EXPECT_STREQ(m_vStock.at(1)->m_strSICIndustry, _T(" "));
			EXPECT_STREQ(m_vStock.at(1)->m_strSICSector, _T(" "));
			EXPECT_STREQ(m_vStock.at(1)->m_strReportingCurrency, _T("usd"));
			EXPECT_STREQ(m_vStock.at(1)->m_strLocation, _T(" "));
			EXPECT_STREQ(m_vStock.at(1)->m_strCompanyWebSite, _T(" "));
			EXPECT_STREQ(m_vStock.at(1)->m_strSECFilingWebSite, _T(" "));
			EXPECT_EQ(m_vStock.at(1)->m_lStatementUpdateDate, 20210302);
			EXPECT_EQ(m_vStock.at(1)->m_lDailyDataUpdateDate, 20210312);
			break;
		}
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	TiingoWebData tiingoWebData31(1, _T("AAPL"), _T("[\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有date项
	TiingoWebData tiingoWebData32(2, _T("AAPL"), _T("[{\"Missing\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有close项
	TiingoWebData tiingoWebData33(3, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"Missing\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有high项
	TiingoWebData tiingoWebData35(5, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"Missing\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有low项
	TiingoWebData tiingoWebData36(6, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"Missing\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有open项
	TiingoWebData tiingoWebData37(7, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"Missing\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有volume项
	TiingoWebData tiingoWebData38(8, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"Missing\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第二个数据没有date项
	TiingoWebData tiingoWebData39(9, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"Missing\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 正确的数据
	TiingoWebData tiingoWebData40(10, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));

	class ProcessTiingoStockDayLineTest : public::testing::TestWithParam<TiingoWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetDayLineNeedUpdate(true);
			m_pStock->SetDayLineNeedSaving(false);
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->UnloadDayLine();
			m_pWebData = pData->m_pData;
		}

		virtual void TearDown(void) override {
			// clearup
			m_pStock->SetDayLineNeedUpdate(true);
			m_pStock->SetDayLineNeedSaving(false);
			m_pStock->SetUpdateProfileDB(false);
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStockDayLine1,
		ProcessTiingoStockDayLineTest,
		testing::Values(&tiingoWebData31, &tiingoWebData32, &tiingoWebData33, &tiingoWebData35,
			&tiingoWebData36, &tiingoWebData37, &tiingoWebData38, &tiingoWebData39, &tiingoWebData40));

	TEST_P(ProcessTiingoStockDayLineTest, TestProcessTiingoStockDayLine0) {
		bool fSucceed = false;
		CString strMessage;
		CDayLinePtr pDayLine;

		fSucceed = gl_pWorldMarket->ProcessTiingoStockDayLine(m_pWebData, m_pStock);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			strMessage = _T("Tiingo下载");
			strMessage += m_pStock->GetSymbol();
			strMessage += _T("日线故障\n");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
			break;
		case 2: //
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			break;
		case 3: //
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			break;
		case 5:
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			break;
		case 6:
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			break;
		case 7:
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			break;
		case 8:
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			break;
		case 9:
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetDayLineSize(), 0);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
			EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetDayLineSize(), 2);
			pDayLine = m_pStock->GetDayLine(0);
			EXPECT_EQ(pDayLine->GetMarketDate(), 20210311);
			EXPECT_EQ(pDayLine->GetClose(), 121960);
			EXPECT_EQ(pDayLine->GetHigh(), 123210);
			EXPECT_EQ(pDayLine->GetLow(), 121260);
			EXPECT_EQ(pDayLine->GetOpen(), 122540);
			EXPECT_EQ(pDayLine->GetVolume(), 103026514);
			break;
		default:
			break;
		}
	}

	struct TiingoWebSocketData {
		TiingoWebSocketData(long lIndex, CString strSymbol, CString strData) {
			m_lIndex = lIndex;
			m_strSymbol = strSymbol;
			m_pData = strData;
		}

		~TiingoWebSocketData() {
		}

	public:
		long m_lIndex;
		CString m_strSymbol;
		string m_pData;
	};

	TiingoWebSocketData tiingoForexData1(1, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"eurnok\",\"2019-07-05T15:49:15.157000+00:00\",5000000.0,9.6764,9.678135,5000000.0,9.67987]}"));
	TiingoWebSocketData tiingoForexData2(2, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	TiingoWebSocketData tiingoForexData4(4, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	TiingoWebSocketData tiingoForexData5(5, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// data名称错为'dta'
	TiingoWebSocketData tiingoForexData6(6, _T(""), _T("{\"messageType\":\"A\",\"service\":\"fx\",\"dta\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// data首项必须为‘fx'
	TiingoWebSocketData tiingoForexData7(7, _T(""), _T("{\"messageType\":\"A\",\"service\":\"ifx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// messageType错误
	TiingoWebSocketData tiingoForexData8(8, _T(""), _T("{\"messageType\":\"B\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// jsong格式错误
	TiingoWebSocketData tiingoForexData9(9, _T(""), _T("\"messageType\":\"A\",\"service\":\"fx\",\"data\":[\"Q\",\"gbpaud\",\"2019-07-05T15:49:15.236000+00:00\",1000000.0,1.79457,1.79477,5000000.0,1.79497]}"));
	// heart beat
	TiingoWebSocketData tiingoForexData10(3, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// authenizition
	TiingoWebSocketData tiingoForexData11(4, _T(""), _T("{\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}"));

	class ProcessOneTiingoForexWebSocketDataTest : public::testing::TestWithParam<TiingoWebSocketData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		shared_ptr<string> m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoForexWebSocketData1, ProcessOneTiingoForexWebSocketDataTest,
		testing::Values(&tiingoForexData1, &tiingoForexData2, &tiingoForexData6, &tiingoForexData7, &tiingoForexData8, &tiingoForexData9,
			&tiingoForexData10, &tiingoForexData11));

	TEST_P(ProcessOneTiingoForexWebSocketDataTest, TestProcessOneTiingoForexWebSocketData0) {
		bool fSucceed = false;
		CTiingoForexSocketPtr pForex;
		fSucceed = gl_pWorldMarket->ProcessOneTiingoForexWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_TRUE(fSucceed);
			pForex = gl_pWorldMarket->PopTiingoForexSocket();
			EXPECT_EQ(pForex->m_chMessageType, 'Q');
			EXPECT_STREQ(pForex->m_strSymbol, _T("eurnok"));
			EXPECT_DOUBLE_EQ(pForex->m_dBidSize, 5000000);
			EXPECT_DOUBLE_EQ(pForex->m_dBidPrice, 9.6764);
			EXPECT_DOUBLE_EQ(pForex->m_dMidPrice, 9.678135);
			EXPECT_DOUBLE_EQ(pForex->m_dAskSize, 5000000.0);
			EXPECT_DOUBLE_EQ(pForex->m_dAskPrice, 9.67987);
			break;
		case 2: // 正确
			EXPECT_TRUE(fSucceed);
			pForex = gl_pWorldMarket->PopTiingoForexSocket();
			break;
		case 6: //
			EXPECT_FALSE(fSucceed);
			break;
		case 7: //
			EXPECT_FALSE(fSucceed);
			break;
		case 8: // messageType错误
			EXPECT_FALSE(fSucceed);
			break;
		case 9: // jsong格式错误
			EXPECT_FALSE(fSucceed);
			break;
		case 10: // heart beat
			EXPECT_TRUE(fSucceed);
			break;
		case 11: // authenizition
			EXPECT_TRUE(fSucceed);
			break;
		default:
			break;
		}
	}

	TiingoWebSocketData tiingoCryptoData1(1, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	TiingoWebSocketData tiingoCryptoData2(2, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"T\",\"evxbtc\",\"2019-01-30T18:03:40.056000+00:00\",\"binance\",405.0,9.631e-05]}"));
	// heart beat
	TiingoWebSocketData tiingoCryptoData3(3, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// authenizition
	TiingoWebSocketData tiingoCryptoData4(4, _T(""), _T("{\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"},\"data\":{\"subscriptionId\":2563396}}"));
	// messageType必须为'A' 'H' 或者'I'
	TiingoWebSocketData tiingoCryptoData5(5, _T(""), _T("{\"messageType\":\"B\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	// data错为'dta'
	TiingoWebSocketData tiingoCryptoData6(6, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"dta\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));
	// data第一项必须为'Q'或者'T'
	TiingoWebSocketData tiingoCryptoData7(7, _T(""), _T("{\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"A\",\"evxbtc\",\"2019-01-30T18:03:40.056000+00:00\",\"binance\",405.0,9.631e-05]}"));
	// json格式错误
	TiingoWebSocketData tiingoCryptoData9(9, _T(""), _T("\"messageType\":\"A\",\"service\":\"crypto_data\",\"data\":[\"Q\",\"neojpy\",\"2019-01-30T18:03:40.195515+00:00\",\"bitfinex\",38.11162867,787.82,787.83,42.4153887,787.84]}"));

	class ProcessOneTiingoCryptoWebSocketDataTest : public::testing::TestWithParam<TiingoWebSocketData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
			m_pWebData = nullptr;
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		shared_ptr<string> m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoCryptoWebSocketData1, ProcessOneTiingoCryptoWebSocketDataTest,
		testing::Values(&tiingoCryptoData1, &tiingoCryptoData2, &tiingoCryptoData3, &tiingoCryptoData4, &tiingoCryptoData5,
			&tiingoCryptoData6, &tiingoCryptoData7, &tiingoCryptoData9));

	TEST_P(ProcessOneTiingoCryptoWebSocketDataTest, TestProcessOneTiingoCryptoWebSocketData0) {
		bool fSucceed = false;
		CTiingoCryptoSocketPtr pCrypto;
		fSucceed = gl_pWorldMarket->ProcessOneTiingoCryptoWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确 Q
			EXPECT_TRUE(fSucceed);
			pCrypto = gl_pWorldMarket->PopTiingoCryptoSocket();
			EXPECT_EQ(pCrypto->m_chMessageType, 'Q');
			EXPECT_STREQ(pCrypto->m_strExchange, _T("bitfinex"));
			EXPECT_STREQ(pCrypto->m_strSymbol, _T("neojpy"));
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidSize, 38.11162867);
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidPrice, 787.82);
			EXPECT_DOUBLE_EQ(pCrypto->m_dMidPrice, 787.83);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskPrice, 787.84);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskSize, 42.4153887);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastSize, 0);
			break;
		case 2: // 正确 T
			EXPECT_TRUE(fSucceed);
			pCrypto = gl_pWorldMarket->PopTiingoCryptoSocket();
			EXPECT_EQ(pCrypto->m_chMessageType, 'T');
			EXPECT_STREQ(pCrypto->m_strExchange, _T("binance"));
			EXPECT_STREQ(pCrypto->m_strSymbol, _T("evxbtc"));
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidSize, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dBidPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dMidPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskPrice, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dAskSize, 0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastSize, 405.0);
			EXPECT_DOUBLE_EQ(pCrypto->m_dLastPrice, 9.631e-05);
			break;
		case 3: // 正确 heart beat
			EXPECT_TRUE(fSucceed);
			break;
		case 4: // 正确 authenizition
			EXPECT_TRUE(fSucceed);
			break;
		case 5: // messageType错误
			EXPECT_FALSE(fSucceed);
			break;
		case 6:
			EXPECT_FALSE(fSucceed);
			break;
		case 7:
			EXPECT_FALSE(fSucceed);
			break;
		case 9: // json格式错误
			EXPECT_FALSE(fSucceed);
			break;
		default:
			break;
		}
	}

	// 正确数据
	TiingoWebSocketData tiingoIEXData1(1, _T(""), _T("{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	TiingoWebSocketData tiingoIEXData2(2, _T(""), _T("{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"T\",\"2019-01-30T13:33:45.594808294-05:00\",1548873225594808294,\"wes\",null,null,null,null,null,50.285,200,null,0,0,0,0]}"));
	// authenization
	TiingoWebSocketData tiingoIEXData3(3, _T(""), _T("{\"data\":{\"subscriptionId\":2563367},\"messageType\":\"I\",\"response\":{\"code\":200,\"message\":\"Success\"}}"));
	// Heart beat
	TiingoWebSocketData tiingoIEXData4(4, _T(""), _T("{\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}"));
	// messageType只能为'A''I''H'
	TiingoWebSocketData tiingoIEXData5(5, _T(""), _T("{\"messageType\":\"B\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// data的首项只能为‘Q’ ‘T’ 或者‘B’
	TiingoWebSocketData tiingoIEXData6(6, _T(""), _T("{\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"C\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// 错误。service的名称不为"iex",
	TiingoWebSocketData tiingoIEXData7(7, _T(""), _T("{\"messageType\":\"A\",\"service\":\"ex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));
	// service错为servi
	TiingoWebSocketData tiingoIEXData8(8, _T(""), _T("{\"messageType\":\"A\",\"servi\":\"iex\",\"data\":[\"T\",\"2019-01-30T13:33:45.594808294-05:00\",1548873225594808294,\"wes\",null,null,null,null,null,50.285,200,null,0,0,0,0]}"));
	// json格式错误
	TiingoWebSocketData tiingoIEXData9(9, _T(""), _T("\"messageType\":\"A\",\"service\":\"iex\",\"data\":[\"Q\",\"2019-01-30T13:33:45.383129126-05:00\",1548873225383129126,\"vym\",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}"));

	class ProcessOneTiingoIEXWebSocketDataTest : public::testing::TestWithParam<TiingoWebSocketData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebSocketData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = nullptr;
			m_pWebData = make_shared<string>(pData->m_pData);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}

	public:
		long m_lIndex;
		shared_ptr<string> m_pWebData;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessOneTiingoIEXWebSocketData1, ProcessOneTiingoIEXWebSocketDataTest,
		testing::Values(&tiingoIEXData1, &tiingoIEXData2, &tiingoIEXData3, &tiingoIEXData4, &tiingoIEXData5,
			&tiingoIEXData6, &tiingoIEXData7, &tiingoIEXData8, &tiingoIEXData9));

	TEST_P(ProcessOneTiingoIEXWebSocketDataTest, TestProcessOneTiingoIEXWebSocketData0) {
		CTiingoIEXSocketPtr pTiingoIEX;
		bool fSucceed = false;
		fSucceed = gl_pWorldMarket->ProcessOneTiingoIEXWebSocketData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确 Q
			EXPECT_TRUE(fSucceed);
			pTiingoIEX = gl_pWorldMarket->PopTiingoIEXSocket();
			EXPECT_EQ(pTiingoIEX->m_iNanoseconds, 1548873225383129126);
			EXPECT_STREQ(pTiingoIEX->m_strSymbol, _T("vym"));
			EXPECT_EQ(pTiingoIEX->m_dBidSize, 100);
			EXPECT_EQ(pTiingoIEX->m_dBidPrice, 81.58);
			EXPECT_EQ(pTiingoIEX->m_dMidPrice, 81.585);
			EXPECT_EQ(pTiingoIEX->m_dAskPrice, 81.59);
			EXPECT_EQ(pTiingoIEX->m_dAskSize, 100);
			EXPECT_EQ(pTiingoIEX->m_dLastPrice, 0);
			EXPECT_EQ(pTiingoIEX->m_dLastSize, 0);
			EXPECT_EQ(pTiingoIEX->m_iHalted, 0);
			EXPECT_EQ(pTiingoIEX->m_iAfterHour, 0);
			EXPECT_EQ(pTiingoIEX->m_iISO, 0);
			EXPECT_EQ(pTiingoIEX->m_iOddlot, 0);
			EXPECT_EQ(pTiingoIEX->m_iNMSRule611, 0); break;
		case 2: // 正确 T
			EXPECT_TRUE(fSucceed);
			pTiingoIEX = gl_pWorldMarket->PopTiingoIEXSocket();
			EXPECT_EQ(pTiingoIEX->m_iNanoseconds, 1548873225594808294);
			EXPECT_STREQ(pTiingoIEX->m_strSymbol, _T("wes"));
			EXPECT_EQ(pTiingoIEX->m_dBidSize, 0);
			EXPECT_EQ(pTiingoIEX->m_dBidPrice, 0);
			EXPECT_EQ(pTiingoIEX->m_dMidPrice, 0);
			EXPECT_EQ(pTiingoIEX->m_dAskPrice, 0);
			EXPECT_EQ(pTiingoIEX->m_dAskSize, 0);
			EXPECT_EQ(pTiingoIEX->m_dLastPrice, 50.285);
			EXPECT_EQ(pTiingoIEX->m_dLastSize, 200);
			EXPECT_EQ(pTiingoIEX->m_iHalted, 0);
			EXPECT_EQ(pTiingoIEX->m_iAfterHour, 0);
			EXPECT_EQ(pTiingoIEX->m_iISO, 0);
			EXPECT_EQ(pTiingoIEX->m_iOddlot, 0);
			EXPECT_EQ(pTiingoIEX->m_iNMSRule611, 0);
			break;
		case 3: // authenization
			EXPECT_TRUE(fSucceed);
			break;
		case 4: // Heart beat
			EXPECT_TRUE(fSucceed);
			break;
		case 5: //
			EXPECT_FALSE(fSucceed);
			break;
		case 6: //
			EXPECT_FALSE(fSucceed);
			break;
		case 7: // service的名称必须为"iex"
			EXPECT_FALSE(fSucceed);
			break;
		case 8: // service错为serci
			EXPECT_FALSE(fSucceed);
			break;
		case 9: // jsong格式错误
			EXPECT_FALSE(fSucceed);
			break;
		default:
			break;
		}
	}
}