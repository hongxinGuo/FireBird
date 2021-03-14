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

  // 数据少于20个字符
  TiingoWebData tiingoWebData1(1, _T("AAPL"), _T("{\"acoy\"}"));
  // 格式不对(缺开始的‘{’），无法顺利Parser
  TiingoWebData tiingoWebData2(2, _T("AAPL"), _T("\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
  // 数据缺乏address项
  TiingoWebData tiingoWebData3(3, _T("AAPL"), _T("{\"adss\":\"contctively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));
  // 正确的数据
  TiingoWebData tiingoWebData10(10, _T("AAPL"), _T("{\"address\":\"contentious  selectively\",\"city\":\"slaughterer\",\"country\":\"miscuing\",\"currency\":\"inveigles\",\"cusip\":\"Grable's\",\"description\":\"crooked ng Odis tint's\",\"employeeTotal\":\"jalopies\",\"exchange\":\"sieves abominating cuff's hesitation's debilitating\",\"finnhubIndustry\":\"culottes\",\"ggroup\":\"Ziegler's tendrils\",\"gind\":\"prairies  catalysis\",\"gsector\":\"habituate Scandinavians\",\"gsubind\":\"checkout  cherished\",\"ipo\":\"1980-12-12\",\"isin\":\"rapport\",\"logo\":\"freelancer's\",\"marketCapitalization\":8790583.5,\"naics\":\"mishmAlisha\",\"naicsNationalIndustry\":\"pollen jay's flops\",\"naicsSector\":\"smuggest\",\"naicsSubsector\":\"apprenticeship's Kringle\",\"name\":\"impediment's gondolier\",\"phone\":\"shootout's\",\"sedol\":\"decrescendi\",\"shareOutstanding\":75546.432,\"state\":\"Tweedledee\",\"ticker\":\"AAPL\",\"weburl\":\"gestated\"}"));

  class ProcessTiingoStockProfileTest : public::testing::TestWithParam<TiingoWebData*>
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      TiingoWebData* pData = GetParam();
      m_lIndex = pData->m_lIndex;
      m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
      EXPECT_TRUE(m_pStock != nullptr);
      m_pStock->SetProfileUpdated(false);
      m_pStock->SetProfileUpdateDate(000000);
      m_pStock->SetCity(_T(""));
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

  INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStockProfile1,
                           ProcessTiingoStockProfileTest,
                           testing::Values(&tiingoWebData1, &tiingoWebData2,
                                           &tiingoWebData3, &tiingoWebData10));

  TEST_P(ProcessTiingoStockProfileTest, TestProcessStockProfile0) {
    bool fSucceed = false;
    fSucceed = gl_pWorldMarket->ProcessTiingoStockSymbol(m_pWebData, m_vStock);
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
    break;
    }
  }
}