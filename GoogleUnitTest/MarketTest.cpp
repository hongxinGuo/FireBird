#include"stdafx.h"
#include"pch.h"

#include"Market.h"
#include"Stock.h"

#include"SetStockCode.h"

using namespace std;
using namespace testing;

#include<memory>

namespace StockAnalysisTest {

  class CMarketTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {

    }

    virtual void TearDown(void) override {
      // clearup
    }

    static void SetUpTestCase() { // 本测试类的初始化函数
      
    }

    static void TearDownTestCase() {

    }
  };
 
  TEST_F(CMarketTest, TestInitialize) {
    EXPECT_EQ(gl_ChinaStockMarket.gl_strRTStockSource.Compare(_T("http://hq.sinajs.cn/list=")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.gl_strDayLineStockSource.Compare("http://quotes.money.163.com/service/chddata.html?code="), 0);
    EXPECT_EQ(gl_ChinaStockMarket.gl_strDayLinePostfix.Compare("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"), 0);
    EXPECT_GT(gl_ChinaStockMarket.GetTotalStock(), 0);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    EXPECT_FALSE(gl_ChinaStockMarket.MarketReady());
    EXPECT_EQ(gl_ChinaStockMarket.m_pCurrentStock, nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.m_fCurrentEditStockChanged);
    EXPECT_FALSE(gl_ChinaStockMarket.m_fMarketOpened);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalAttackBuyAmount(), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalAttackSellAmount(), 0);
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T(""));
    EXPECT_TRUE(gl_ChinaStockMarket.m_fGetRTStockData);
    EXPECT_TRUE(gl_ChinaStockMarket.m_fGetDayLineData);
    EXPECT_TRUE(gl_ChinaStockMarket.m_fCountDownRT);
    EXPECT_EQ(gl_ChinaStockMarket.m_iCountDownDayLine, 2);
    EXPECT_EQ(gl_ChinaStockMarket.m_iCountDownRT, 1);
    EXPECT_EQ(gl_vTotalStock.size(), 5000);   // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
    EXPECT_EQ(gl_mapTotalStockToIndex.size(), 5000);
    EXPECT_STREQ(gl_vTotalStock.at(0)->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(gl_vTotalStock.at(0)->GetMarket(), 1);
    EXPECT_STREQ(gl_vTotalStock.at(2000)->GetStockCode(), _T("sz000000"));
    EXPECT_EQ(gl_vTotalStock.at(2000)->GetMarket(), 2);
    for (int i = 0; i < 5000; i++) {
      EXPECT_EQ(gl_vTotalStock.at(i)->GetIndex(), i);
    }
    EXPECT_EQ(gl_mapTotalStockToIndex.at(_T("sh600000")), 0);
    EXPECT_EQ(gl_mapTotalStockToIndex.at(_T("sz000000")), 2000);
  }
  
  TEST_F(CMarketTest, TestCreateRTDataInquiringStr) {
    CString str;
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    CString strCompare, str2;
    strCompare = gl_vTotalStock.at(898)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_vTotalStock.at(1796)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_vTotalStock.at(2694)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_vTotalStock.at(3592)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_vTotalStock.at(4490)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_vTotalStock.at(0)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
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
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz600000")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("ss000001")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("az000001")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz200001")));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(_T("sz000001")));
  }

  TEST_F(CMarketTest, TestIsStock) {
    CStockPtr pstock;
    EXPECT_GT(gl_ChinaStockMarket.GetTotalStock(), 1);
    EXPECT_TRUE(gl_ChinaStockMarket.IsStock(_T("sh600000"), pstock));
    EXPECT_NE(pstock, nullptr);
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
    EXPECT_FALSE(gl_ChinaStockMarket.MarketReady());
    gl_ChinaStockMarket.SetMarketReadyFlag(true);
    EXPECT_TRUE(gl_ChinaStockMarket.MarketReady());
    gl_ChinaStockMarket.SetMarketReadyFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.MarketReady());
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

  TEST_F(CMarketTest, TestIsLoadSelectedStock) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
  }

  TEST_F(CMarketTest, TestGetDownLoadingStockCodeStr) {
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T(""));
    gl_ChinaStockMarket.SetDownLoadingStockCodeStr(_T("abcd"));
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T("abcd"));
  }
  TEST_F(CMarketTest, TestGetRelativeStartDay) {
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900101);
    gl_ChinaStockMarket.SetRelativeStrongStartDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900103);
  }

  TEST_F(CMarketTest, TestGetRelativeEndDay) {
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900101);
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900103);
  }



}