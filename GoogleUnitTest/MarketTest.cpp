#include"stdafx.h"
#include"pch.h"

#include"Market.h"
#include"Stock.h"

#include"SetStockCode.h"

using namespace std;

#include<memory>

namespace StockAnalysisTest {
  class CMarketTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_ChinaStockMarket.__Test_SetResetSystem(false);
    }

    virtual void TearDown(void) override {
      // clearup
    }

    static void SetUpTestCase() { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
      gl_ChinaStockMarket.__Test_SetResetSystem(false);
      gl_ChinaStockMarket.SetUpdateStockCodeDB(false);
    }

    static void TearDownTestCase() {
    }
  };

  TEST_F(CMarketTest, TestInitialize) {
    EXPECT_GT(gl_ChinaStockMarket.GetTotalActiveStock(), 0);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_EQ(gl_ChinaStockMarket.m_pCurrentStock, nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.m_fCurrentEditStockChanged);
    EXPECT_FALSE(gl_ChinaStockMarket.m_fMarketOpened);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalAttackBuyAmount(), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalAttackSellAmount(), 0);
    EXPECT_TRUE(gl_ChinaStockMarket.m_fGetRTStockData);
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
    EXPECT_EQ(gl_ChinaStockMarket.m_iCountDownDayLine, 3);
    EXPECT_EQ(gl_ChinaStockMarket.m_iCountDownSlowReadingRTData, 3);
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.size(), 12000);   // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockMapIndexSize(), 12000);
    EXPECT_STREQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetMarket(), __SHANGHAI_MARKET__);
    EXPECT_STREQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(6000)->GetStockCode(), _T("sz000000"));
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(6000)->GetMarket(), __SHENZHEN_MARKET__);
    for (int i = 0; i < 12000; i++) {
      EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->GetOffset(), i);
      EXPECT_TRUE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->IsDayLineNeedUpdate());
      EXPECT_FALSE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->IsDayLineNeedProcess());
      EXPECT_FALSE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->IsDayLineNeedSaving());
    }
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sh600000")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sz000000")), 6000);

    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetSystem());
    EXPECT_EQ(gl_ChinaStockMarket.m_iDayLineNeedProcess, 0);
    EXPECT_EQ(gl_ChinaStockMarket.m_iDayLineNeedUpdate, 12000);
    EXPECT_EQ(gl_ChinaStockMarket.m_iDayLineNeedSave, 0);
    EXPECT_FALSE(gl_ChinaStockMarket.IsUsingNeteaseRTDataReceiverAsTester());
    EXPECT_TRUE(gl_ChinaStockMarket.IsUsingTengxunRTDataReceiverAsTester());
  }

  TEST_F(CMarketTest, TestGetTengxunInquiringStockStr) {
    gl_ChinaStockMarket.SetSystemReady(true);
    CString str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    CString strCompare, str2;
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(899)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(1798)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(2697)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(3596)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(4495)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(5394)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(6293)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(7192)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8091)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8990)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(9889)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(10788)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(11687)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetTengxunInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
  }

  TEST_F(CMarketTest, TestGetInquiringStockStr2) {
    CString str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    CString strCompare, str2;
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(899)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(1798)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(2697)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(3596)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(4495)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(5394)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(6293)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(7192)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8091)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8990)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(9889)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(10788)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(11687)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = _T("");
    gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
  }

  TEST_F(CMarketTest, TestGetInquiringStockStr) {
    gl_ChinaStockMarket.SetSystemReady(true);
    CString str;
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900), 900);
    str = _T("");
    long l = gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900);
    if (l < 900) {
      str = _T("");
      EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900), 900);
      CString str2 = str.Left(9);
      EXPECT_STREQ(str2, _T("sh600000,"));
    }
    else {
      str = _T("");
      EXPECT_GT(900, gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900)); // 目前不到五千个活跃股票，故而六次即可遍历一次
      str = _T("");
      EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str, 900), 900);
      CString str2 = str.Left(9);
      EXPECT_STREQ(str2, _T("sh600000,"));
    }
  }

  TEST_F(CMarketTest, TestGetNeteaseInquiringStockStr) {
    CString str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    CString strCompare, str2;
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(899)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(1798)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(2697)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(3596)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(4495)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(5394)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(6293)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(7192)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8091)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8990)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(9889)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(10788)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(11687)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = _T("");
    gl_ChinaStockMarket.GetNeteaseInquiringStockStr(str, 900, false);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
  }

  TEST_F(CMarketTest, TestGetNeteaseDayLineInquiringStr) {
    CString str;
    EXPECT_TRUE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(2)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_ChinaStockMarket.CreateNeteaseDayLineInquiringStr(str));
    EXPECT_STREQ(str, _T("0600000"));
    EXPECT_FALSE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->IsDayLineNeedUpdate());
    EXPECT_TRUE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->IsDayLineNeedUpdate());

    // 恢复原状
    for (int i = 0; i < 12000; i++) {
      if (!gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->IsDayLineNeedUpdate()) {
        gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->SetDayLineNeedUpdate(true);
      }
    }
  }

  TEST_F(CMarketTest, TestIsAStock) {
    CStockPtr pstock = make_shared<CStock>();
    pstock->SetStockCode(_T("sh600000"));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(pstock));
    pstock->SetStockCode(_T("ss600000"));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(pstock));
    pstock->SetStockCode(_T("sz000001"));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(pstock));
    pstock->SetStockCode(_T("sz10001"));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(pstock));
  }

  TEST_F(CMarketTest, TestIsAStock2) {
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(_T("sh600000")));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(_T("sh601198")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sh602102")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sh603604")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sh604604")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sh605604")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sh606604")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sh607604")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sh608604")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sh609604")));

    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz600000")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("ss000001")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("az000001")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz200001")));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(_T("sz000001")));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(_T("sz002389")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz003389")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz004389")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz005389")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz006389")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz007389")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz008389")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz009389")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz001389")));
  }

  TEST_F(CMarketTest, TestIsStock) {
    CStockPtr pstock;
    EXPECT_GT(gl_ChinaStockMarket.GetTotalActiveStock(), 1);
    EXPECT_TRUE(gl_ChinaStockMarket.IsStock(_T("sh600000"), pstock));
    EXPECT_NE(pstock, nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.IsStock(_T("sh60000"), pstock));
    EXPECT_EQ(pstock, nullptr);
  }

  TEST_F(CMarketTest, TestGetStockName) {
    //未实现.由于stockName存储时使用的是UniCode制式，而本系统默认是Ansi制式，导致无法进行字符串对比。暂时不进行测试了。
    //EXPECT_STREQ(gl_ChinaStockMarket.GetStockName(_T("sh600000")), _T("浦发银行"));
    EXPECT_STREQ(gl_ChinaStockMarket.GetStockName(_T("sh60000")), _T("")); // 没找到返回空字符串
  }

  TEST_F(CMarketTest, TestGetStockIndex) {
    long lIndex = -2;
    EXPECT_TRUE(gl_ChinaStockMarket.GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_STREQ(gl_ChinaStockMarket.GetStockPtr(lIndex)->GetStockCode(), _T("sh600000"));
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockIndex(_T("sh60000"), lIndex));
    EXPECT_EQ(lIndex, -1);
  }

  TEST_F(CMarketTest, TestGetStockCode) {
    EXPECT_EQ(gl_ChinaStockMarket.GetStockPtr(_T("sh66000")), nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockPtr(_T("sh600001")) == nullptr);

    EXPECT_EQ(gl_ChinaStockMarket.GetStockPtr(-1), nullptr);
    EXPECT_EQ(gl_ChinaStockMarket.GetStockPtr(12000), nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockPtr(0) == nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockPtr(11999) == nullptr);
  }

  TEST_F(CMarketTest, TestGetShowStock) {
    CStockPtr pStock = make_shared<CStock>();
    EXPECT_EQ(gl_ChinaStockMarket.GetShowStock(), nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
    gl_ChinaStockMarket.SetShowStock(pStock);
    EXPECT_EQ(gl_ChinaStockMarket.GetShowStock(), pStock);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCurrentStockChanged());
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
    gl_ChinaStockMarket.SetShowStock(pStock);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
  }

  TEST_F(CMarketTest, TestMarketReady) {
    gl_ChinaStockMarket.SetSystemReady(true);
    EXPECT_TRUE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.SetSystemReady(false);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
  }

  TEST_F(CMarketTest, TestIsTodayStockCompiled) {
    gl_systemTime.CalculateTime();
    if (gl_systemTime.GetTime() > 150000) {
      EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockCompiled());
    }
    else {
      EXPECT_FALSE(gl_ChinaStockMarket.IsTodayStockCompiled());
    }
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockCompiled());
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayStockCompiled());
  }

  TEST_F(CMarketTest, TestIsDayLineNeedUpdate) {
    EXPECT_TRUE(gl_ChinaStockMarket.IsDayLineNeedUpdate());
    for (auto pStock : gl_ChinaStockMarket.m_vChinaMarketAStock) {
      pStock->SetDayLineNeedUpdate(false);
    }
    EXPECT_FALSE(gl_ChinaStockMarket.IsDayLineNeedUpdate());
  }

  TEST_F(CMarketTest, TestIsLoadSelectedStock) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
  }

  TEST_F(CMarketTest, TestTaskResetSystem) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_TRUE(gl_ChinaStockMarket.__Test_IsPermitResetSystem());
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetSystem(91259);
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetSystem(91401);
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetSystem(91300);
    EXPECT_TRUE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
  }

  TEST_F(CMarketTest, TestTaskResetSystem2) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_TRUE(gl_ChinaStockMarket.__Test_IsPermitResetSystem());
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetSystem(91259);
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetSystem(91401);
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetSystem(91400);
    EXPECT_TRUE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
  }

  TEST_F(CMarketTest, TestTaskResetSystemAgain) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_TRUE(gl_ChinaStockMarket.__Test_IsPermitResetSystem());
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetSystemAgain(92459);
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetSystemAgain(93001);
    EXPECT_FALSE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_TRUE(gl_ChinaStockMarket.__Test_IsPermitResetSystem());
    gl_ChinaStockMarket.TaskResetSystemAgain(92500);
    EXPECT_TRUE(gl_fResetSystem);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_FALSE(gl_ChinaStockMarket.__Test_IsPermitResetSystem());
  }

  TEST_F(CMarketTest, TestCheckMarketOpen) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(91459));
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCheckMarketOpen(91500));
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCheckMarketOpen(113500));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(113501));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(125459));
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCheckMarketOpen(125500));
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCheckMarketOpen(150630));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(150631));
    tm_.tm_wday = 0;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(91459));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(91500));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(113459));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(113500));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(125459));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(125500));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(150630));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(150631));
  }

  TEST_F(CMarketTest, TestTaskUpdateStockCodeDB) {
    gl_ChinaStockMarket.m_fSaveDayLine = true;
    gl_ChinaStockMarket.m_iDayLineNeedSave = 1;
    gl_ChinaStockMarket.m_iDayLineNeedProcess = 1;
    gl_ChinaStockMarket.m_iDayLineNeedUpdate = 1;
    gl_ChinaStockMarket.SetUpdateStockCodeDB(false); // 不实际更新数据库
    gl_ChinaStockMarket.TaskUpdateStockCodeDB();
    EXPECT_TRUE(gl_ChinaStockMarket.m_fSaveDayLine);
    gl_ChinaStockMarket.m_iDayLineNeedProcess = 0;
    gl_ChinaStockMarket.TaskUpdateStockCodeDB();
    EXPECT_TRUE(gl_ChinaStockMarket.m_fSaveDayLine);
    gl_ChinaStockMarket.m_iDayLineNeedSave = 0;
    gl_ChinaStockMarket.TaskUpdateStockCodeDB();
    EXPECT_TRUE(gl_ChinaStockMarket.m_fSaveDayLine);
    gl_ChinaStockMarket.m_iDayLineNeedUpdate = 0;
    gl_ChinaStockMarket.TaskUpdateStockCodeDB();
    EXPECT_FALSE(gl_ChinaStockMarket.m_fSaveDayLine);
  }

  TEST_F(CMarketTest, TestIncreaseNeteaseDayLineInquiringIndex) {
    int k = 0;
    EXPECT_EQ(gl_ChinaStockMarket.m_lTotalStock, 12000);
    int i = gl_ChinaStockMarket.IncreaseStockInquiringIndex(k);
    EXPECT_EQ(i, 1);
    EXPECT_EQ(k, 1);
    i = gl_ChinaStockMarket.IncreaseStockInquiringIndex(k);
    EXPECT_EQ(i, 2);
    EXPECT_EQ(k, 2);
    k = 11999;
    i = gl_ChinaStockMarket.IncreaseStockInquiringIndex(k);
    EXPECT_EQ(k, 0);
    EXPECT_EQ(i, 0);
  }

  TEST_F(CMarketTest, TestStepToActiveStockIndex) {
    int i = 0; //
    int k = 0;
    gl_ChinaStockMarket.StepToActiveStockIndex(i);
    EXPECT_EQ(i, 0); // sh600000为活跃股票
    i++;
    gl_ChinaStockMarket.StepToActiveStockIndex(i);
    EXPECT_EQ(i, 4); // sh600004为活跃股票，之间三个是退市股
    for (int j = 1; j < 4; j++) {
      EXPECT_FALSE(gl_ChinaStockMarket.GetStockPtr(j)->IsActive());
    }
  }

  TEST_F(CMarketTest, TestResetSystemFlagAtMidnight) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsPermitResetSystem());
    gl_ChinaStockMarket.ResetSystemFlagAtMidnight(0);
    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetSystem());
    gl_ChinaStockMarket.SetPermitResetSystem(false);
    gl_ChinaStockMarket.ResetSystemFlagAtMidnight(1501);
    EXPECT_FALSE(gl_ChinaStockMarket.IsPermitResetSystem());
    gl_ChinaStockMarket.ResetSystemFlagAtMidnight(1500);
    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetSystem());
  }

  TEST_F(CMarketTest, TestCompileTodayStock) {
    gl_ChinaStockMarket.SetSystemReady(false);
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCompileTodayStock(150259));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCompileTodayStock(150300));
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayStockCompiled());
    gl_ChinaStockMarket.SetSystemReady(true);
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCompileTodayStock(150259));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCompileTodayStock(150300));
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockCompiled());
    gl_ChinaStockMarket.SetSystemReady(true);
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCompileTodayStock(150259));
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCompileTodayStock(150300));
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockCompiled());
  }

  TEST_F(CMarketTest, TestSetCheckActiveStockFlag) {
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(91459);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(91500);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(92859);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(92900);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(113059);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(113100);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(125859);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(125900);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.TaskSetCheckTodayActiveStockFlag(150000);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
  }

  TEST_F(CMarketTest, TestLoadStockCodeDB) {
    // 股票代码数据库在全局环境设置时即已装入测试系统，故而直接测试即可。
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetIPOStatus(), __STOCK_IPOED__);
    EXPECT_STREQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetDayLineStartDay(), 19991110);
    EXPECT_TRUE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->IsActive());
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->GetIPOStatus(), __STOCK_DELISTED__);
    EXPECT_STREQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->GetStockCode(), _T("sh600001"));
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->GetDayLineStartDay(), 19980122);
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->GetDayLineEndDay(), 20091215);
    EXPECT_FALSE(gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->IsActive());
  }

  TEST_F(CMarketTest, TestGetLastLoginDay) {
    gl_ChinaStockMarket.SetLastLoginDay(19900102);
    EXPECT_EQ(gl_ChinaStockMarket.GetLastLoginDay(), 19900102);
  }

  TEST_F(CMarketTest, TestGetRelativeStrongStartDay) {
    gl_ChinaStockMarket.SetRelativeStrongStartDay(19900202);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900202);
  }

  TEST_F(CMarketTest, TestGetRelativeStrongEndDay) {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900302);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900302);
  }

  TEST_F(CMarketTest, TestGetReadingSinaRTDataTime) {
    time_t tt = 1010101010;
    gl_ChinaStockMarket.SetReadingSinaRTDataTime(tt);
    EXPECT_EQ(gl_ChinaStockMarket.GetReadingSinaRTDataTime(), tt);
  }

  TEST_F(CMarketTest, TestGetReadingNeteaseDayLineDataTime) {
    time_t tt = 1010101010;
    gl_ChinaStockMarket.SetReadingNeteaseDayLineDataTime(tt);
    EXPECT_EQ(gl_ChinaStockMarket.GetReadingNeteaseDayLineDataTime(), tt);
  }

  TEST_F(CMarketTest, TestGetReadingTengxunRTDataTime) {
    time_t tt = 1010101010;
    gl_ChinaStockMarket.SetReadingTengxunRTDataTime(tt);
    EXPECT_EQ(gl_ChinaStockMarket.GetReadingTengxunRTDataTime(), tt);
  }

  TEST_F(CMarketTest, TesstIsCalculatingRS) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsCalculatingRS());
    gl_ChinaStockMarket.SetCalculatingRS(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCalculatingRS());
    gl_ChinaStockMarket.SetCalculatingRS(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCalculatingRS());
  }

  TEST_F(CMarketTest, TesstIsTodayTempRTDataLoaded) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
    gl_ChinaStockMarket.SetTodayTempRTDataLoaded(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
    gl_ChinaStockMarket.SetTodayTempRTDataLoaded(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
  }

  TEST_F(CMarketTest, TesstIsCheckTodayActiveStock) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.SetCheckTodayActiveStock(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
    gl_ChinaStockMarket.SetCheckTodayActiveStock(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckTodayActiveStock());
  }

  TEST_F(CMarketTest, TestGetTotalActiveStock) {
    long l = gl_ChinaStockMarket.GetTotalActiveStock();
    gl_ChinaStockMarket.SetTotalActiveStock(4000);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalActiveStock(), 4000);
    gl_ChinaStockMarket.SetTotalActiveStock(l);
  }

  TEST_F(CMarketTest, TestGetStockPtr) {
    CStockPtr pStock = nullptr;
    long lIndex = -1;
    pStock = gl_ChinaStockMarket.GetStockPtr(_T("sh600000"));
    EXPECT_TRUE(gl_ChinaStockMarket.GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_EQ(lIndex, 0);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600000"));
  }
}