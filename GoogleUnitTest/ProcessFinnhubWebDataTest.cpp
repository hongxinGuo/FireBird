#include"pch.h"
#include"globedef.h"
#include"WebInquirer.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

namespace StockAnalysisTest {
  struct FinnhubWebData {
    FinnhubWebData(long lIndex, CString strSymbol, CString strData) {
      m_lIndex = lIndex;
      m_strSymbol = strSymbol;
      m_pData = make_shared<CWebData>();
      m_pData->__TEST_SetBuffer__(strData);
    }

    ~FinnhubWebData() {
    }

  public:
    long m_lIndex;
    CString m_strSymbol;
    CWebDataPtr m_pData;
  };

  // 数据少于20个字符
  FinnhubWebData finnhubWebData1(1, _T("AAPL"), _T("{\"acoy\"}"));
  // 格式不对(缺开始的‘{’），无法顺利Parser
  FinnhubWebData finnhubWebData2(2, _T("AAPL"), _T("\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
  // 数据缺乏address项
  FinnhubWebData finnhubWebData3(3, _T("AAPL"), _T("{\"adss\":\"contctively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
  // 正确的数据
  FinnhubWebData finnhubWebData10(10, _T("AAPL"), _T("{\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));

  class ProcessFinnhubStockProfileTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pStock->SetProfileUpdated(false);
      m_pStock->SetProfileUpdateDate(000000);
      m_pStock->SetCity(_T(""));
      m_pWebData = pData->m_pData;
    }
    virtual void TearDown(void) override {
      // clearup
      m_pStock->SetUpdateProfileDB(false);
    }

  public:
    long m_lIndex;
    CWorldStockPtr m_pStock;
    CWebDataPtr m_pWebData;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockProfile1, ProcessFinnhubStockProfileTest, testing::Values(&finnhubWebData1, &finnhubWebData2,
                                                                                                            &finnhubWebData3, &finnhubWebData10));

  TEST_P(ProcessFinnhubStockProfileTest, TestProcessStockProfile0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubStockProfile(m_pWebData, m_pStock);
    switch (m_lIndex) {
    case 1: // 少于20个字符
    EXPECT_FALSE(fSucceed);
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetFormatedMarketDate());
    break;
    case 2: // 格式不对
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsProfileUpdated());
    EXPECT_EQ(m_pStock->GetProfileUpdateDate(), 000000);
    break;
    case 3: // 缺乏address项
    EXPECT_FALSE(fSucceed);
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetFormatedMarketDate());
    EXPECT_STRNE(m_pStock->GetCity(), _T("slaughterer")) << "没有赋值此项";
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_STREQ(m_pStock->GetTicker(), _T("AAPL"));
    EXPECT_STREQ(m_pStock->GetCity(), _T("slaughterer"));
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    EXPECT_EQ(m_pStock->GetProfileUpdateDate(), gl_pWorldMarket->GetFormatedMarketDate());
    break;
    default:
    break;
    }
  }

  // 数据少于30个字符
  FinnhubWebData finnhubWebData11(1, _T("AAPL"), _T("{\"acoy\"}"));
  // 格式不对(缺开始的‘{’），无法顺利Parser
  FinnhubWebData finnhubWebData12(2, _T("AAPL"), _T("\"country\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));
  // 数据缺乏country项
  FinnhubWebData finnhubWebData13(3, _T("AAPL"), _T("{\"counry\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));
  // 正确的数据
  FinnhubWebData finnhubWebData20(10, _T("AAPL"), _T("{\"country\":\"US\",\"currency\":\"USD\",\"exchange\":\"NASDAQ NMS - GLOBAL MARKET\",\"finnhubIndustry\":\"Technology\",\"ipo\":\"1980-12-12\",\"logo\":\"https://finnhub.io/api/logo?symbol=AAPL\",\"marketCapitalization\":2014236,\"name\":\"Apple Inc\",\"phone\":\"14089961010.0\",\"shareOutstanding\":16788.096,\"ticker\":\"AAPL\",\"weburl\":\"https://www.apple.com/\"}"));

  class ProcessFinnhubStockProfileConciseTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pStock->SetCountry(_T(""));
      m_pWebData = pData->m_pData;
    }
    virtual void TearDown(void) override {
      // clearup
    }

  public:
    long m_lIndex;
    CWorldStockPtr m_pStock;
    CWebDataPtr m_pWebData;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockProfileConcise1, ProcessFinnhubStockProfileConciseTest, testing::Values(&finnhubWebData11, &finnhubWebData12,
                                                                                                                          &finnhubWebData13, &finnhubWebData20));

  TEST_P(ProcessFinnhubStockProfileConciseTest, TestProcessStockProfileConcise0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubStockProfileConcise(m_pWebData, m_pStock);
    switch (m_lIndex) {
    case 1: // 少于20个字符
    EXPECT_TRUE(fSucceed);
    break;
    case 2: // 格式不对
    EXPECT_FALSE(fSucceed);
    break;
    case 3: // 缺乏address项
    EXPECT_FALSE(fSucceed);
    EXPECT_STRNE(m_pStock->GetCountry(), _T("US")) << "没有赋值此项";
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_STREQ(m_pStock->GetTicker(), _T("AAPL"));
    EXPECT_STREQ(m_pStock->GetCountry(), _T("US"));
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘{’），无法顺利Parser
  FinnhubWebData finnhubWebData22(2, _T("AAPL"), _T("[\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"mic\":\"XNAS\",\"symbol\":\"CERC\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"mic\":\"OOTC\",\"symbol\":\"ATGVF\",\"type\":\"Common Stock\"}]"));
  // 数据缺乏currency项
  FinnhubWebData finnhubWebData23(3, _T("AAPL"), _T("[{\"cuency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"mic\":\"XNAS\",\"symbol\":\"CERC\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"mic\":\"OOTC\",\"symbol\":\"ATGVF\",\"type\":\"Common Stock\"}]"));
  // 正确的数据
  FinnhubWebData finnhubWebData30(10, _T("AAPL"), _T("[{\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"A\",\"figi\":\"BBG001QHW0Y8\",\"mic\":\"XNAS\",\"symbol\":\"A\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"AA\",\"figi\":\"BBG009LQZG05\",\"mic\":\"OOTC\",\"symbol\":\"AA\",\"type\":\"Common Stock\"}]"));

  class ProcessFinnhubStockSymbolTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pStock->SetCurrency(_T(""));
      m_pWebData = pData->m_pData;
      m_vStock.resize(0);
    }
    virtual void TearDown(void) override {
      // clearup
    }

  public:
    long m_lIndex;
    CWorldStockPtr m_pStock;
    CWebDataPtr m_pWebData;
    vector<CWorldStockPtr> m_vStock;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockSymbol1, ProcessFinnhubStockSymbolTest, testing::Values(&finnhubWebData22, &finnhubWebData23,
                                                                                                          &finnhubWebData30));

  TEST_P(ProcessFinnhubStockSymbolTest, TestProcessFinnhubStockSymbol0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubStockSymbol(m_pWebData, m_vStock);
    switch (m_lIndex) {
    case 2: // 格式不对
    EXPECT_FALSE(fSucceed);
    break;
    case 3: // 缺乏currency项
    EXPECT_FALSE(fSucceed);
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_STREQ(m_vStock.at(0)->GetSymbol(), _T("A"));
    EXPECT_STREQ(m_vStock.at(1)->GetSymbol(), _T("AA"));
    EXPECT_EQ(m_vStock.size(), 2);
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘{’），无法顺利Parser
  FinnhubWebData finnhubWebData31(1, _T("AAPL"), _T("\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
  // s项报告非ok
  FinnhubWebData finnhubWebData32(2, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"not ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
  // s项报告no data
  FinnhubWebData finnhubWebData33(3, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"no_data\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
  // 数据缺乏t项
  FinnhubWebData finnhubWebData34(4, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"a\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
  // 缺乏c项。不影响结果
  FinnhubWebData finnhubWebData35(5, _T("AAPL"), _T("{\"a\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
  // 缺乏h项
  FinnhubWebData finnhubWebData36(6, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"a\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
  // 缺乏l项
  FinnhubWebData finnhubWebData37(7, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"a\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
  // 缺乏o项
  FinnhubWebData finnhubWebData38(8, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"a\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
  // 缺乏v项
  FinnhubWebData finnhubWebData39(9, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"a\":[36665,105291,68286]}"));
  // 正确的数据
  FinnhubWebData finnhubWebData40(10, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"s\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));

  class ProcessFinnhubStockCandleTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pStock->SetUpdateProfileDB(false);
      m_pStock->SetCurrency(_T(""));
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

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockCandle1,
                           ProcessFinnhubStockCandleTest,
                           testing::Values(&finnhubWebData31, &finnhubWebData32, &finnhubWebData33, &finnhubWebData35, &finnhubWebData35,
                                           &finnhubWebData36, &finnhubWebData37, &finnhubWebData38, &finnhubWebData39, &finnhubWebData40));

  TEST_P(ProcessFinnhubStockCandleTest, TestProcessFinnhubStockCandle0) {
    bool fSucceed = false;
    CString strMessage;

    fSucceed = gl_pWorldMarket->ProcessFinnhubStockCandle(m_pWebData, m_pStock);
    switch (m_lIndex) {
    case 1: // 格式不对
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
    strMessage = _T("下载");
    strMessage += m_pStock->GetSymbol();
    strMessage += _T("日线故障\n");
    EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), strMessage);
    break;
    case 2: // s项报告not ok
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
    strMessage = _T("下载");
    strMessage += m_pStock->GetSymbol();
    strMessage += _T("日线返回值不为ok\n");
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), strMessage);
    EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), strMessage);
    break;
    case 3: // s项报告 no data
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    break;
    case 4:
    EXPECT_FALSE(fSucceed);
    break;
    case 5:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    break;
    case 6:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    break;
    case 7:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    break;
    case 8:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    break;
    case 9:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pStock->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pStock->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘{’），无法顺利Parser
  FinnhubWebData finnhubWebData53(3, _T("AAPL"), _T("\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
  // 缺c
  FinnhubWebData finnhubWebData54(4, _T("AAPL"), _T("{\"a\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
  // 缺h
  FinnhubWebData finnhubWebData55(5, _T("AAPL"), _T("{\"c\":121.03,\"a\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
  // 缺l
  FinnhubWebData finnhubWebData56(6, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"a\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
  // 缺o
  FinnhubWebData finnhubWebData57(7, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"a\":120.4,\"pc\":121.96,\"t\":1615507200}"));
  // 缺pc
  FinnhubWebData finnhubWebData58(8, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"a\":121.96,\"t\":1615507200}"));
  // 缺乏t
  FinnhubWebData finnhubWebData59(9, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"a\":1615507200}"));
  // 正确的数据
  FinnhubWebData finnhubWebData60(10, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));

  class ProcessFinnhubStockQuoteTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pStock->SetNew(0);
      m_pStock->SetHigh(0);
      m_pStock->SetLow(0);
      m_pStock->SetOpen(0);
      m_pStock->SetLastClose(0);
      m_pStock->SetTransactionTime(0);
      m_pWebData = pData->m_pData;
    }
    virtual void TearDown(void) override {
      // clearup
      m_pStock->SetNew(0);
      m_pStock->SetHigh(0);
      m_pStock->SetLow(0);
      m_pStock->SetOpen(0);
      m_pStock->SetLastClose(0);
      m_pStock->SetTransactionTime(0);
    }

  public:
    long m_lIndex;
    CWorldStockPtr m_pStock;
    CWebDataPtr m_pWebData;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockQuote1,
                           ProcessFinnhubStockQuoteTest,
                           testing::Values(&finnhubWebData53, &finnhubWebData54, &finnhubWebData55, &finnhubWebData56,
                                           &finnhubWebData57, &finnhubWebData58, &finnhubWebData58, &finnhubWebData60));

  TEST_P(ProcessFinnhubStockQuoteTest, TestProcessFinnhubStockQuote0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubStockQuote(m_pWebData, m_pStock);
    switch (m_lIndex) {
    case 3: // 格式不对
    EXPECT_FALSE(fSucceed);
    break;
    case 4: // 缺乏c
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_pStock->GetNew(), 0);
    EXPECT_EQ(m_pStock->GetHigh(), 0);
    EXPECT_EQ(m_pStock->GetLow(), 0);
    EXPECT_EQ(m_pStock->GetOpen(), 0);
    EXPECT_EQ(m_pStock->GetLastClose(), 0);
    EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
    break;
    case 5: // 缺乏h项
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_pStock->GetNew(), 121030);
    EXPECT_EQ(m_pStock->GetHigh(), 0);
    EXPECT_EQ(m_pStock->GetLow(), 0);
    EXPECT_EQ(m_pStock->GetOpen(), 0);
    EXPECT_EQ(m_pStock->GetLastClose(), 0);
    EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
    break;
    case 6: // 缺乏l项
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_pStock->GetNew(), 121030);
    EXPECT_EQ(m_pStock->GetHigh(), 121170);
    EXPECT_EQ(m_pStock->GetLow(), 0);
    EXPECT_EQ(m_pStock->GetOpen(), 0);
    EXPECT_EQ(m_pStock->GetLastClose(), 0);
    EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
    break;
    case 7: // 缺乏o项
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_pStock->GetNew(), 121030);
    EXPECT_EQ(m_pStock->GetHigh(), 121170);
    EXPECT_EQ(m_pStock->GetLow(), 119160);
    EXPECT_EQ(m_pStock->GetOpen(), 0);
    EXPECT_EQ(m_pStock->GetLastClose(), 0);
    EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
    break;
    case 8: // 缺乏pc
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_pStock->GetNew(), 121030);
    EXPECT_EQ(m_pStock->GetHigh(), 121170);
    EXPECT_EQ(m_pStock->GetLow(), 119160);
    EXPECT_EQ(m_pStock->GetOpen(), 120400);
    EXPECT_EQ(m_pStock->GetLastClose(), 0);
    EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
    break;
    case 9: // 缺乏t项
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_pStock->GetNew(), 121030);
    EXPECT_EQ(m_pStock->GetHigh(), 121170);
    EXPECT_EQ(m_pStock->GetLow(), 119160);
    EXPECT_EQ(m_pStock->GetOpen(), 120400);
    EXPECT_EQ(m_pStock->GetLastClose(), 121960);
    EXPECT_EQ(m_pStock->GetTransactionTime(), 0);
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_EQ(m_pStock->GetNew(), 121030);
    EXPECT_EQ(m_pStock->GetHigh(), 121170);
    EXPECT_EQ(m_pStock->GetLow(), 119160);
    EXPECT_EQ(m_pStock->GetOpen(), 120400);
    EXPECT_EQ(m_pStock->GetLastClose(), 121960);
    EXPECT_EQ(m_pStock->GetTransactionTime(), 1615507200);
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘{’），无法顺利Parser
  FinnhubWebData finnhubWebData61(1, _T("OANDA:EUR_ZAR"), _T("\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
  // s项报告非ok
  FinnhubWebData finnhubWebData62(2, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"not ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
  // s项报告no data
  FinnhubWebData finnhubWebData63(3, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"no_data\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
  // 数据缺乏t项
  FinnhubWebData finnhubWebData64(4, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
  // 缺乏c项。不影响结果
  FinnhubWebData finnhubWebData65(5, _T("OANDA:EUR_ZAR"), _T("{\"a\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
  // 缺乏h项
  FinnhubWebData finnhubWebData66(6, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"a\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
  // 缺乏l项
  FinnhubWebData finnhubWebData67(7, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"a\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
  // 缺乏o项
  FinnhubWebData finnhubWebData68(8, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"a\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
  // 缺乏v项
  FinnhubWebData finnhubWebData69(9, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"a\":[36521,47505]}"));
  // 正确的数据
  FinnhubWebData finnhubWebData70(10, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));

  class ProcessFinnhubForexCandleTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pData->m_strSymbol));
      m_pForexSymbol = gl_pWorldMarket->GetForexSymbol(pData->m_strSymbol);
      EXPECT_TRUE(m_pForexSymbol != nullptr);
      m_pForexSymbol->SetUpdateProfileDB(false);
      m_pWebData = pData->m_pData;

      m_pForexSymbol->SetDayLineNeedUpdate(true);
      m_pForexSymbol->SetDayLineNeedSaving(false);
      m_pForexSymbol->SetUpdateProfileDB(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

      //m_pForexSymbol->SetDayLineNeedUpdate(true);
      // m_pForexSymbol->SetDayLineNeedSaving(false);
      //m_pForexSymbol->SetUpdateProfileDB(false);
    }

  public:
    long m_lIndex;
    CForexSymbolPtr m_pForexSymbol;
    CWebDataPtr m_pWebData;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexCandle1,
                           ProcessFinnhubForexCandleTest,
                           testing::Values(&finnhubWebData61, &finnhubWebData62, &finnhubWebData63, &finnhubWebData64, &finnhubWebData65,
                                           &finnhubWebData66, &finnhubWebData67, &finnhubWebData68, &finnhubWebData69, &finnhubWebData70));

  TEST_P(ProcessFinnhubForexCandleTest, TestProcessFinnhubForexCandle0) {
    bool fSucceed = false;
    CString strMessage;

    fSucceed = gl_pWorldMarket->ProcessFinnhubForexCandle(m_pWebData, m_pForexSymbol);
    switch (m_lIndex) {
    case 1: // 格式不对
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pForexSymbol->IsUpdateProfileDB());
    strMessage = _T("下载");
    strMessage += m_pForexSymbol->GetSymbol();
    strMessage += _T("日线故障\n");
    EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), strMessage);
    break;
    case 2: // s项报告not ok
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pForexSymbol->IsUpdateProfileDB());
    strMessage = _T("下载");
    strMessage += m_pForexSymbol->GetSymbol();
    strMessage += _T("日线返回值不为ok\n");
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), strMessage);
    EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), strMessage);
    break;
    case 3: // s项报告 no data
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pForexSymbol->IsUpdateProfileDB());
    break;
    case 4:
    EXPECT_FALSE(fSucceed);
    break;
    case 5:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pForexSymbol->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pForexSymbol->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pForexSymbol->IsUpdateProfileDB());
    break;
    case 6:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pForexSymbol->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pForexSymbol->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pForexSymbol->IsUpdateProfileDB());
    break;
    case 7:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pForexSymbol->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pForexSymbol->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pForexSymbol->IsUpdateProfileDB());
    break;
    case 8:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pForexSymbol->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pForexSymbol->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pForexSymbol->IsUpdateProfileDB());
    break;
    case 9:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pForexSymbol->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pForexSymbol->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pForexSymbol->IsUpdateProfileDB());
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_FALSE(m_pForexSymbol->IsDayLineNeedUpdate());
    EXPECT_TRUE(m_pForexSymbol->IsDayLineNeedSaving());
    EXPECT_TRUE(m_pForexSymbol->IsUpdateProfileDB());
    break;
    default:
    break;
    }
  }
}