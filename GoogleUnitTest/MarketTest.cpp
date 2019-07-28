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
    EXPECT_EQ(gl_ChinaStockMarket.m_strRTStockSource.Compare(_T("http://hq.sinajs.cn/list=")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.m_strDayLineStockSource.Compare(_T("http://quotes.money.163.com/service/chddata.html?code=")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.m_strDayLinePostfix.Compare(_T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP")), 0);
    EXPECT_GT(gl_ChinaStockMarket.GetTotalStock(), 0);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_EQ(gl_ChinaStockMarket.m_pCurrentStock, nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.m_fCurrentEditStockChanged);
    EXPECT_FALSE(gl_ChinaStockMarket.m_fMarketOpened);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalAttackBuyAmount(), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalAttackSellAmount(), 0);
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T(""));
    EXPECT_TRUE(gl_ChinaStockMarket.m_fGetRTStockData);
    EXPECT_TRUE(gl_ChinaStockMarket.m_fGetDayLineData);
    EXPECT_EQ(gl_ChinaStockMarket.m_iCountDownDayLine, 2);
    EXPECT_EQ(gl_ChinaStockMarket.m_iCountDownSlowReadingRTData, 1);
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.size(), 10000);   // 在全局变量gl_ChinaStockMarket初始化时就生成了全部股票代码池
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockMapIndexSize(), 10000);
    EXPECT_STREQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetMarket(), 1);
    EXPECT_STREQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(5000)->GetStockCode(), _T("sz000000"));
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(5000)->GetMarket(), 2);
    for (int i = 0; i < 10000; i++) {
      EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->GetIndex(), i);
    }
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sh600000")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sz000000")), 5000);
  }

  TEST_F(CMarketTest, TestCreateRTDataInquiringStr) {
    CString str;
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    CString strCompare, str2;
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(898)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(1796)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(2694)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(3592)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(4490)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(5388)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(6286)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(7184)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8082)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8980)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(9878)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
  }

  TEST_F(CMarketTest, TestGetInquiringStockStr) {
    CString str;
    EXPECT_EQ(gl_ChinaStockMarket.GetInquiringStockStr(str), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetInquiringStockStr(str), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetInquiringStockStr(str), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetInquiringStockStr(str), 900);
    str = _T("");
    long l = 0;
    EXPECT_GT(900, gl_ChinaStockMarket.GetInquiringStockStr(str));
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetInquiringStockStr(str), 900);
    CString str2 = str.Left(9);
    EXPECT_STREQ(str2, _T(",sh600000"));
  }

  TEST_F(CMarketTest, TestCreateDayLineInquiringStr) {
    CString str;
    CString strStartDay;
    strStartDay = _T("20190101");
    gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay);
    EXPECT_STREQ(str, _T("0600000"));
    EXPECT_STREQ(strStartDay, _T("19700102")); //最初日期为19700101，这是第二天
    // 一下四个股票由于不同的原因不查日线历史数据
    gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->SetDayLineNeedUpdate(false);
    gl_ChinaStockMarket.m_vChinaMarketAStock.at(5)->SetDayLineNeedUpdate(false);
    gl_ChinaStockMarket.m_vChinaMarketAStock.at(7)->SetIPOStatus(__STOCK_NULL__);
    gl_ChinaStockMarket.m_vChinaMarketAStock.at(6)->SetDayLineEndDay(20190101);
    for (int i = 0; i < (12 - 4); i++) { // 跨过其他的八个股票，到达随后的两个无效代码处
      strStartDay = _T("");
      str = _T("");
      gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay);
      EXPECT_STREQ(strStartDay, _T("19700102"));
    }
    strStartDay = _T("");
    str = _T("");
    EXPECT_TRUE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay)); // 找到股票代码时返回真值
    EXPECT_STREQ(str, _T("0600015")); // 600013、600014为无效代码，故而不去提取其日线数据
    EXPECT_STREQ(strStartDay, _T("19700102")); //最初日期为19700101，这是第二天

    // 设置随后的2000个为无需查询状态
    for (int i = 16; i < 2016; i++) {
      gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->SetDayLineNeedUpdate(false);
    }
    EXPECT_FALSE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay)); // 此处跨过1000个，返回假值
    EXPECT_FALSE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay));
    str = _T("");
    strStartDay = _T("");
    EXPECT_TRUE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay));
    EXPECT_STREQ(str, _T("0603016"));
    EXPECT_STREQ(strStartDay, _T("19700102"));

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
    EXPECT_GT(gl_ChinaStockMarket.GetTotalStock(), 1);
    EXPECT_TRUE(gl_ChinaStockMarket.IsStock(_T("sh600000"), pstock));
    EXPECT_NE(pstock, nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.IsStock(_T("sh60000"), pstock));
    EXPECT_EQ(pstock, nullptr);
  }

  TEST_F(CMarketTest, TestGetStockName) {
    // 未实现.由于stockName存储时使用的是UniCode制式，而本系统默认是Ansi制式，导致无法进行字符串对比。暂时不进行测试了。
    //EXPECT_STREQ(gl_ChinaStockMarket.GetStockName(_T("sh600000")), _T("浦发银行"));
    //EXPECT_STREQ(gl_ChinaStockMarket.GetStockName(_T("sh60000")), _T("")); // 没找到返回空字符串

  }

  TEST_F(CMarketTest, TestGetStockIndex) {
    long lIndex = -2;
    EXPECT_TRUE(gl_ChinaStockMarket.GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_STREQ(gl_ChinaStockMarket.GetStockPtr(lIndex)->GetStockCode(), _T("sh600000"));
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockIndex(_T("sh60000"), lIndex));
    EXPECT_EQ(lIndex, -1);
  }

  TEST_F(CMarketTest, TestGetStockIDPtr) {
    StockIDPtr pID = nullptr;
    EXPECT_TRUE(gl_ChinaStockMarket.GetStockIDPtr(_T("sh600000"), pID));
    EXPECT_STREQ(pID->GetStockCode(), _T("sh600000"));
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockIDPtr(_T("sh600"), pID));
    EXPECT_EQ(pID, nullptr);
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
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
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

  TEST_F(CMarketTest, TestIsTotalStockDayLineChecked) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsTotalStockDayLineChecked());
    for (auto pID : gl_ChinaStockMarket.m_vChinaMarketAStock) {
      pID->SetDayLineNeedUpdate(false);
    }
    EXPECT_TRUE(gl_ChinaStockMarket.IsTotalStockDayLineChecked());
  }
  TEST_F(CMarketTest, TestIsLoadSelectedStock) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
  }

  TEST_F(CMarketTest, TestGetDownLoadingStockCodeStr) {
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T("sh603016")); // 前面测试时查询到这个代码处
    gl_ChinaStockMarket.SetDownLoadingStockCodeStr(_T("abcd"));
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T("abcd"));
  }
  TEST_F(CMarketTest, TestGetRelativeStartDay) {
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900101);
    gl_ChinaStockMarket.SetRelativeStrongStartDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900103);
  }

  TEST_F(CMarketTest, TestGetRelativeEndDay) {
    //EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 20190727); // 此数据会不时变化，因为是从真实数据库读取的，系统会不时更新。暂时不测试初始态了
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900103);
  }

  TEST_F(CMarketTest, TestSchedulingTest) { // 测试这个函数，一定要小心，不能导致出现改变系统实际数据的事情
    gl_ChinaStockMarket.SetSystemReady(false); // 禁止测试时执行实际操作

    gl_systemTime.SetTime(90999);
    gl_ChinaStockMarket.SchedulingTaskPerSecond();
    EXPECT_FALSE(gl_ChinaStockMarket.m_fMarketOpened);
    gl_systemTime.SetTime(91000);
    gl_ChinaStockMarket.SchedulingTaskPerSecond();
    if ((gl_systemTime.GetDayOfWeek() == 0) || (gl_systemTime.GetDayOfWeek() == 6)) {
      EXPECT_FALSE(gl_ChinaStockMarket.m_fMarketOpened);
    }
    else EXPECT_TRUE(gl_ChinaStockMarket.m_fMarketOpened);
    gl_systemTime.SetTime(150129);
    gl_ChinaStockMarket.SchedulingTaskPerSecond();
    if ((gl_systemTime.GetDayOfWeek() == 0) || (gl_systemTime.GetDayOfWeek() == 6)) {
      EXPECT_FALSE(gl_ChinaStockMarket.m_fMarketOpened);
    }
    else EXPECT_TRUE(gl_ChinaStockMarket.m_fMarketOpened);
    gl_systemTime.SetTime(150130);
    gl_ChinaStockMarket.SchedulingTaskPerSecond();
    EXPECT_FALSE(gl_ChinaStockMarket.m_fMarketOpened);



  }

}