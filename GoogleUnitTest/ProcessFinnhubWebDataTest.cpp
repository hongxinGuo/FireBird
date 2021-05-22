#include"pch.h"
#include"globedef.h"
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
      m_pStock->SetProfileUpdateDate(19700101);
      m_pStock->SetCity(_T(""));
      m_pWebData = pData->m_pData;
      EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0);
      EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
      EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
    }
    virtual void TearDown(void) override {
      // clearup
      EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
      EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
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
    EXPECT_EQ(m_pStock->GetProfileUpdateDate(), 19700101);
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
      m_pStock->SetUpdateProfileDB(false);
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
      m_pStock->SetUpdateProfileDB(false);
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
  // 没有s项
  FinnhubWebData finnhubWebData32_1(11, _T("AAPL"), _T("{\"c\":[118.7,119.11,119.04],\"h\":[118.89,119.18,119.16],\"l\":[118.7,118.69,118.98],\"o\":[118.86,118.69,119.15],\"a\":\"ok\",\"t\":[1615300020,1615300080,1615300140],\"v\":[36665,105291,68286]}"));
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
                           testing::Values(&finnhubWebData31, &finnhubWebData32, &finnhubWebData32_1, &finnhubWebData33, &finnhubWebData34, &finnhubWebData35,
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
    case 11: // 没有s项
    EXPECT_FALSE(fSucceed);
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
      m_pStock->SetUpdateProfileDB(false);
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
  // 没有s项
  FinnhubWebData finnhubWebData62_1(11, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"a\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
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
      m_pForexSymbol->SetUpdateProfileDB(false);
    }

  public:
    long m_lIndex;
    CForexSymbolPtr m_pForexSymbol;
    CWebDataPtr m_pWebData;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexCandle1,
                           ProcessFinnhubForexCandleTest,
                           testing::Values(&finnhubWebData61, &finnhubWebData62_1, &finnhubWebData62, &finnhubWebData63, &finnhubWebData64, &finnhubWebData65,
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
    case 11: // 没有s项
    EXPECT_TRUE(fSucceed);
    EXPECT_TRUE(m_pForexSymbol->IsUpdateProfileDB());
    EXPECT_TRUE(m_pForexSymbol->IsNullStock());
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘[’），无法顺利Parser
  FinnhubWebData finnhubWebData72(2, _T(""), _T("\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
  // 格式不对
  FinnhubWebData finnhubWebData73(3, _T(""), _T("[\"oanda\",fxcm,\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
  // 正确的数据
  FinnhubWebData finnhubWebData80(10, _T(""), _T("[\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));

  class ProcessFinnhubForexExchangeTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pWebData = pData->m_pData;
      m_vExchange.resize(0);
    }
    virtual void TearDown(void) override {
      // clearup
    }

  public:
    long m_lIndex;
    CWebDataPtr m_pWebData;
    vector<CString> m_vExchange;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexExchange1, ProcessFinnhubForexExchangeTest, testing::Values(&finnhubWebData72, &finnhubWebData73,
                                                                                                              &finnhubWebData80));

  TEST_P(ProcessFinnhubForexExchangeTest, TestProcessFinnhubForexExchange0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubForexExchange(m_pWebData, m_vExchange);
    switch (m_lIndex) {
    case 2: // 格式不对
    EXPECT_FALSE(fSucceed);
    break;
    case 3: // 缺乏字符串
    EXPECT_FALSE(fSucceed);
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_STREQ(m_vExchange.at(0), _T("oanda"));
    EXPECT_STREQ(m_vExchange.at(1), _T("fxcm"));
    EXPECT_EQ(m_vExchange.size(), 9);
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘[’），无法顺利Parser
  FinnhubWebData finnhubWebData82(2, _T(""), _T("[\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
  // 数据缺乏description
  FinnhubWebData finnhubWebData83(3, _T(""), _T("[{\"a\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
  // 数据缺乏displaySymbol
  FinnhubWebData finnhubWebData84(4, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"a\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
  // 数据缺乏symbol
  FinnhubWebData finnhubWebData85(5, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"a\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
  // 正确的数据
  FinnhubWebData finnhubWebData90(10, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));

  class ProcessFinnhubForexSymbolTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pWebData = pData->m_pData;
      m_vForexSymbol.resize(0);
    }
    virtual void TearDown(void) override {
      // clearup
    }

  public:
    long m_lIndex;
    CWebDataPtr m_pWebData;
    vector<CForexSymbolPtr> m_vForexSymbol;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexSymbol1, ProcessFinnhubForexSymbolTest,
                           testing::Values(&finnhubWebData82, &finnhubWebData83, &finnhubWebData84,
                                           &finnhubWebData85, &finnhubWebData90));

  TEST_P(ProcessFinnhubForexSymbolTest, TestProcessFinnhubForexSymbol0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubForexSymbol(m_pWebData, m_vForexSymbol);
    switch (m_lIndex) {
    case 2: // 格式不对
    EXPECT_FALSE(fSucceed);
    break;
    case 3: // 缺乏字符串
    EXPECT_FALSE(fSucceed);
    break;
    case 4: // 缺乏字符串
    EXPECT_FALSE(fSucceed);
    break;
    case 5: // 缺乏字符串
    EXPECT_FALSE(fSucceed);
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_STREQ(m_vForexSymbol.at(0)->GetSymbol(), _T("OANDA:SG30_SGD"));
    EXPECT_STREQ(m_vForexSymbol.at(1)->GetSymbol(), _T("OANDA:DE10YB_EUR"));
    EXPECT_EQ(m_vForexSymbol.size(), 2);
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘[’），无法顺利Parser
  FinnhubWebData finnhubWebData92(2, _T(""), _T("{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
  // 第一个数据缺乏CodeNo
  FinnhubWebData finnhubWebData93(3, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"Missing\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
  // 第二个数据缺乏Code2
  FinnhubWebData finnhubWebData94(4, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"Missing\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
  // 数据缺乏symbol
  FinnhubWebData finnhubWebData95(5, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"Missing\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
  // 正确的数据
  FinnhubWebData finnhubWebData100(10, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Nauru\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));

  class ProcessFinnhubCountryListTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pWebData = pData->m_pData;
      m_vCountry.resize(0);
    }
    virtual void TearDown(void) override {
      // clearup
    }

  public:
    long m_lIndex;
    CWebDataPtr m_pWebData;
    vector<CCountryPtr> m_vCountry;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCountryList1, ProcessFinnhubCountryListTest,
                           testing::Values(&finnhubWebData92, &finnhubWebData93, &finnhubWebData94,
                                           &finnhubWebData95, &finnhubWebData100));

  TEST_P(ProcessFinnhubCountryListTest, TestProcessFinnhubCountryList0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubCountryList(m_pWebData, m_vCountry);
    switch (m_lIndex) {
    case 2: // 格式不对
    EXPECT_FALSE(fSucceed);
    break;
    case 3: // 缺乏CodeNo
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vCountry.size(), 0);
    break;
    case 4: // 第二个数据缺Code2
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vCountry.size(), 1);
    EXPECT_STREQ(m_vCountry.at(0)->m_strCode2, _T("NR"));
    EXPECT_STREQ(m_vCountry.at(0)->m_strCurrencyCode, _T("AUD"));
    break;
    case 5: // 第二个数据缺CodeNo
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vCountry.size(), 1);
    EXPECT_STREQ(m_vCountry.at(0)->m_strCode2, _T("NR"));
    EXPECT_STREQ(m_vCountry.at(0)->m_strCurrencyCode, _T("AUD"));
    break;
    case 10:
    EXPECT_EQ(m_vCountry.size(), 2);
    EXPECT_STREQ(m_vCountry.at(0)->m_strCode2, _T("NR"));
    EXPECT_STREQ(m_vCountry.at(0)->m_strCurrencyCode, _T("AUD"));
    EXPECT_STREQ(m_vCountry.at(1)->m_strCode2, _T("MF"));
    EXPECT_STREQ(m_vCountry.at(1)->m_strCurrencyCode, _T("ANG"));
    break;
    default:
    break;
    }
  }

  // 不足三个字符
  FinnhubWebData finnhubWebData102(2, _T("AAPL"), _T("[]"));
  // 格式不对(缺开始的‘[’），无法顺利Parser
  FinnhubWebData finnhubWebData103(3, _T("AAPL"), _T("\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
  // 格式不对
  FinnhubWebData finnhubWebData104(4, _T("AAPL"), _T("[\"AAPL,\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
  // 正确的数据
  FinnhubWebData finnhubWebData110(10, _T("AAPL"), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));

  class ProcessFinnhubStockPeerTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pWebData = pData->m_pData;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pStock->SetPeer(_T(""));
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

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockPeer1, ProcessFinnhubStockPeerTest,
                           testing::Values(&finnhubWebData102, &finnhubWebData103, &finnhubWebData104,
                                           &finnhubWebData110));

  TEST_P(ProcessFinnhubStockPeerTest, TestProcessFinnhubStockPeer0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubStockPeer(m_pWebData, m_pStock);
    switch (m_lIndex) {
    case 2: // 不足三个字符
    EXPECT_TRUE(fSucceed);
    EXPECT_STREQ(m_pStock->GetPeer(), _T(" "));
    break;
    case 3: // 格式不对
    EXPECT_FALSE(fSucceed);
    EXPECT_STREQ(m_pStock->GetPeer(), _T("")) << "没有改变";
    break;
    case 4: // 第二个数据缺Code2
    EXPECT_FALSE(fSucceed);
    EXPECT_STREQ(m_pStock->GetPeer(), _T("")) << "没有改变";
    break;
    case 10:
    EXPECT_TRUE(fSucceed);
    EXPECT_STREQ(m_pStock->GetPeer(), _T("[\"AAPL\",\"DELL\",\"HPQ\",\"WDC\",\"HPE\",\"1337.HK\",\"NTAP\",\"PSTG\",\"XRX\",\"NCR\"]"));
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘{’），无法顺利Parser
  FinnhubWebData finnhubWebData112(2, _T(""), _T("\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
  // 缺乏economicCalendar
  FinnhubWebData finnhubWebData113(3, _T(""), _T("{\"Missing\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
  // 第一个数据缺乏actual
  FinnhubWebData finnhubWebData114(4, _T(""), _T("{\"economicCalendar\":[{\"Missing\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
  // 第二个数据缺乏actual
  FinnhubWebData finnhubWebData115(5, _T(""), _T("{\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"Missing\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
  // 正确的数据
  FinnhubWebData finnhubWebData120(10, _T(""), _T("{\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));

  class ProcessFinnhubEconomicCalendarTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pWebData = pData->m_pData;
      m_vEconomicCalendar.resize(0);
    }
    virtual void TearDown(void) override {
      // clearup
    }

  public:
    long m_lIndex;
    CWebDataPtr m_pWebData;
    vector<CEconomicCalendarPtr> m_vEconomicCalendar;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubEconomicCalendar1, ProcessFinnhubEconomicCalendarTest,
                           testing::Values(&finnhubWebData112, &finnhubWebData113, &finnhubWebData114,
                                           &finnhubWebData115, &finnhubWebData120));

  TEST_P(ProcessFinnhubEconomicCalendarTest, TestProcessFinnhubEconomicCalendar10) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubEconomicCalendar(m_pWebData, m_vEconomicCalendar);
    switch (m_lIndex) {
    case 2: // 格式不对
    EXPECT_FALSE(fSucceed);
    break;
    case 3: // 缺乏economicCalendar
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vEconomicCalendar.size(), 0);
    break;
    case 4: // 第一个数据缺actual
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vEconomicCalendar.size(), 0);
    break;
    case 5: // 第二个数据缺actual
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vEconomicCalendar.size(), 1);
    EXPECT_DOUBLE_EQ(m_vEconomicCalendar.at(0)->m_dActual, 0.6);
    EXPECT_STREQ(m_vEconomicCalendar.at(0)->m_strUnit, _T("%"));
    break;
    case 10:
    EXPECT_EQ(m_vEconomicCalendar.size(), 2);
    EXPECT_DOUBLE_EQ(m_vEconomicCalendar.at(0)->m_dActual, 0.6);
    EXPECT_STREQ(m_vEconomicCalendar.at(0)->m_strUnit, _T("%"));
    EXPECT_DOUBLE_EQ(m_vEconomicCalendar.at(1)->m_dActual, -0.2);
    EXPECT_STREQ(m_vEconomicCalendar.at(1)->m_strUnit, _T("%"));
    EXPECT_DOUBLE_EQ(m_vEconomicCalendar.at(0)->m_dEstimate, 0.6);
    EXPECT_DOUBLE_EQ(m_vEconomicCalendar.at(0)->m_dPrev, 1);
    EXPECT_STREQ(m_vEconomicCalendar.at(0)->m_strCountry, _T("CN"));
    EXPECT_STREQ(m_vEconomicCalendar.at(0)->m_strEvent, _T("CPI MM"));
    EXPECT_STREQ(m_vEconomicCalendar.at(0)->m_strTime, _T("2021-03-10 01:30:00"));
    break;
    default:
    break;
    }
  }

  // 格式不对(缺开始的‘[’），无法顺利Parser
  FinnhubWebData finnhubWebData122(2, _T("AAPL"), _T("{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
  // 第一个数据缺乏actual
  FinnhubWebData finnhubWebData123(3, _T("AAPL"), _T("[{\"Missing\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
  // 第二个数据缺乏actual
  FinnhubWebData finnhubWebData124(4, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"Missing\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
  // 第三个数据缺乏actual
  FinnhubWebData finnhubWebData125(5, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"Missing\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
  // 正确的数据
  FinnhubWebData finnhubWebData130(10, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));

  class ProcessFinnhubEPSSurpriseTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pWebData = pData->m_pData;
      m_vEPSSurprise.resize(0);
    }
    virtual void TearDown(void) override {
      // clearup
      m_pStock->SetUpdateProfileDB(false);
    }

  public:
    long m_lIndex;
    CWorldStockPtr m_pStock;
    CWebDataPtr m_pWebData;
    vector<CEPSSurprisePtr> m_vEPSSurprise;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubEPSSurprise1, ProcessFinnhubEPSSurpriseTest,
                           testing::Values(&finnhubWebData122, &finnhubWebData123, &finnhubWebData124,
                                           &finnhubWebData125, &finnhubWebData130));

  TEST_P(ProcessFinnhubEPSSurpriseTest, TestProcessFinnhubEPSSurprise0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubEPSSurprise(m_pWebData, m_vEPSSurprise);
    switch (m_lIndex) {
    case 2: // 格式不对
    EXPECT_FALSE(fSucceed);
    break;
    case 3: //
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vEPSSurprise.size(), 0);
    break;
    case 4: // 第二个数据缺缺actual
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vEPSSurprise.size(), 1);
    EXPECT_DOUBLE_EQ(m_vEPSSurprise.at(0)->m_dActual, 1.68);
    EXPECT_DOUBLE_EQ(m_vEPSSurprise.at(0)->m_dEstimate, 1.555857);
    EXPECT_EQ(m_vEPSSurprise.at(0)->m_lDate, 20201231);
    EXPECT_STREQ(m_vEPSSurprise.at(0)->m_strSymbol, _T("AAPL"));
    break;
    case 5: // 第三个数据缺CodeNo
    EXPECT_FALSE(fSucceed);
    EXPECT_EQ(m_vEPSSurprise.size(), 2);
    EXPECT_DOUBLE_EQ(m_vEPSSurprise.at(0)->m_dActual, 1.68);
    EXPECT_DOUBLE_EQ(m_vEPSSurprise.at(0)->m_dEstimate, 1.555857);
    EXPECT_EQ(m_vEPSSurprise.at(0)->m_lDate, 20201231);
    EXPECT_STREQ(m_vEPSSurprise.at(0)->m_strSymbol, _T("AAPL"));
    break;
    case 10:
    EXPECT_EQ(m_vEPSSurprise.size(), 4);
    EXPECT_DOUBLE_EQ(m_vEPSSurprise.at(0)->m_dActual, 0.6375);
    EXPECT_DOUBLE_EQ(m_vEPSSurprise.at(0)->m_dEstimate, 0.5765856);
    EXPECT_EQ(m_vEPSSurprise.at(0)->m_lDate, 20200331);
    EXPECT_STREQ(m_vEPSSurprise.at(0)->m_strSymbol, _T("AAPL"));
    EXPECT_DOUBLE_EQ(m_vEPSSurprise.at(3)->m_dActual, 1.68) << "成功处理后，自动按日期排列，导致其被放置于最后";
    EXPECT_DOUBLE_EQ(m_vEPSSurprise.at(3)->m_dEstimate, 1.555857);
    EXPECT_EQ(m_vEPSSurprise.at(3)->m_lDate, 20201231);
    EXPECT_STREQ(m_vEPSSurprise.at(3)->m_strSymbol, _T("AAPL"));
    break;
    default:
    break;
    }
  }

  FinnhubWebData finnhubWebData132(2, _T("AAPL"), _T("{\"data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-02\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"symbol\" : \"RIG\"}"));

  class ProcessFinnhubInsiderTransactionTest : public::testing::TestWithParam<FinnhubWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      FinnhubWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pWebData = pData->m_pData;
      m_vInsiderTransaction.resize(0);
    }
    virtual void TearDown(void) override {
      // clearup
      m_pStock->SetUpdateProfileDB(false);
    }

  public:
    long m_lIndex;
    CWorldStockPtr m_pStock;
    CWebDataPtr m_pWebData;
    vector<CInsiderTransactionPtr> m_vInsiderTransaction;
  };

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubInsiderTransaction1, ProcessFinnhubInsiderTransactionTest,
                           testing::Values(&finnhubWebData132));

  TEST_P(ProcessFinnhubInsiderTransactionTest, TestProcessFinnhubInsiderTransaction0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubStockInsiderTransaction(m_pWebData, m_vInsiderTransaction);
    switch (m_lIndex) {
    case 2: // 格式不对
    EXPECT_EQ(m_vInsiderTransaction.size(), 2);
    EXPECT_STREQ(m_vInsiderTransaction.at(0)->m_strPersonName, _T("Long Brady K"));
    EXPECT_STREQ(m_vInsiderTransaction.at(0)->m_strSymbol, _T("RIG"));
    EXPECT_EQ(m_vInsiderTransaction.at(0)->m_lShare, 269036);
    EXPECT_EQ(m_vInsiderTransaction.at(0)->m_lChange, -14236);
    EXPECT_EQ(m_vInsiderTransaction.at(0)->m_lFilingDate, 20210303);
    EXPECT_EQ(m_vInsiderTransaction.at(0)->m_lTransactionDate, 20210302);
    EXPECT_DOUBLE_EQ(m_vInsiderTransaction.at(0)->m_dTransactionPrice, 3.68);
    EXPECT_TRUE(m_vInsiderTransaction.at(0)->m_lTransactionDate <= m_vInsiderTransaction.at(1)->m_lTransactionDate) << "此序列按交易日期顺序排列";
    break;
    default:
    break;
    }
  }
}