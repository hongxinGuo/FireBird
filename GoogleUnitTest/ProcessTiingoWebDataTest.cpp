#include"pch.h"
#include"globedef.h"
#include"WebInquirer.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

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
  TiingoWebData tiingoWebData2(2, _T(""), _T("[{\"Missing\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
  // 第二个数据缺项
  TiingoWebData tiingoWebData3(3, _T(""), _T("[{\"permaTicker\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"Missing\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
  // 正确的数据
  TiingoWebData tiingoWebData4(4, _T(""), _T("[{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"sector have data\",\"industry\":\"industry have data\",\"sicCode\":\"1234\",\"sicSector\":\"sicSector have data\",\"sicIndustry\":\"sicIndustry have data\",\"reportingCurrency\":\"usd\",\"location\":\"location have data\",\"companyWebsite\":\"companyWebsite have data\",\"secFilingWebsite\":\"secFileingWebsite have data\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
  // 正确的数据
  TiingoWebData tiingoWebData10(10, _T(""), _T("[{\"permaTicker\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));

  class ProcessTiingoStockProfileTest : public::testing::TestWithParam<TiingoWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      TiingoWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pWebData = pData->m_pData;
      m_vStock.resize(0);
    }
    virtual void TearDown(void) override {
      // clearup
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

  public:
    long m_lIndex;
    CWebDataPtr m_pWebData;
    vector<CWorldStockPtr> m_vStock;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStockProfile1,
                           ProcessTiingoStockProfileTest,
                           testing::Values(&tiingoWebData1, &tiingoWebData2,
                                           &tiingoWebData3, &tiingoWebData4, &tiingoWebData10));

  TEST_P(ProcessTiingoStockProfileTest, TestProcessStockProfile0) {
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
    EXPECT_STREQ(m_vStock.at(0)->m_strTiingoIndustry, _T("industry have data"));
    EXPECT_STREQ(m_vStock.at(0)->m_strTiingoSector, _T("sector have data"));
    EXPECT_STREQ(m_vStock.at(0)->m_strSICIndustry, _T("sicIndustry have data"));
    EXPECT_STREQ(m_vStock.at(0)->m_strSICSector, _T("sicSector have data"));
    EXPECT_STREQ(m_vStock.at(0)->m_strCompanyWebSite, _T("companyWebsite have data"));
    EXPECT_STREQ(m_vStock.at(0)->m_strSECFilingWebSite, _T("secFileingWebsite have data"));
    EXPECT_EQ(m_vStock.at(0)->m_iSICCode, 1234);
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_vStock.size(), 2);
    EXPECT_STREQ(m_vStock.at(1)->m_strTiingoPermaTicker, _T("US000000000091"));
    EXPECT_STREQ(m_vStock.at(1)->GetSymbol(), _T("AA"));
    EXPECT_TRUE(m_vStock.at(1)->m_fIsActive);
    EXPECT_FALSE(m_vStock.at(1)->m_fIsADR);

    EXPECT_STREQ(m_vStock.at(1)->m_strTiingoIndustry, _T(" ")) << "当字符串为Field not available for free/evcaluation时，返回空串(一个空格)";
    EXPECT_STREQ(m_vStock.at(1)->m_strTiingoSector, _T(" "));
    EXPECT_EQ(m_vStock.at(1)->m_iSICCode, 0);
    EXPECT_STREQ(m_vStock.at(1)->m_strSICIndustry, _T(" "));
    EXPECT_STREQ(m_vStock.at(1)->m_strSICSector, _T(" "));
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
      ASSERT_FALSE(gl_fNormalMode);
      TiingoWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pStock->SetDayLineNeedUpdate(true);
      m_pStock->SetDayLineNeedSaving(false);
      m_pStock->SetUpdateProfileDB(false);
      m_pStock->UnloadDayLine();
      m_pWebData = pData->m_pData;

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      m_pStock->SetDayLineNeedUpdate(true);
      m_pStock->SetDayLineNeedSaving(false);
      m_pStock->SetUpdateProfileDB(false);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
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
    EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), strMessage);
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
    EXPECT_EQ(pDayLine->GetFormatedMarketDate(), 20210311);
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
}