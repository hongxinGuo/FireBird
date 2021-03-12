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
  // 格式不对(缺开始的‘{’），无法顺利Parser
  FinnhubWebData finnhubWebData2(2, _T("AAPL"), _T("\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
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

  INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockProfile1, ProcessFinnhubStockProfileTest, testing::Values(&finnhubWebData10, &finnhubWebData2));

  TEST_P(ProcessFinnhubStockProfileTest, TestProcessStockProfile0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessFinnhubStockProfile(m_pWebData, m_pStock);
    switch (m_lIndex) {
    case 2:
    EXPECT_FALSE(fSucceed);
    EXPECT_FALSE(m_pStock->IsProfileUpdated());
    EXPECT_EQ(m_pStock->GetProfileUpdateDate(), 000000);
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
}