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

    static void SetUpTestCase() { // ��������ĳ�ʼ������
      
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
    EXPECT_EQ(gl_ChinaStockMarket.gl_vTotalStock.size(), 5000);   // ��ȫ�ֱ���gl_ChinaStockMarket��ʼ��ʱ��������ȫ����Ʊ�����
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockMapIndexSize(), 5000);
    EXPECT_STREQ(gl_ChinaStockMarket.gl_vTotalStock.at(0)->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(gl_ChinaStockMarket.gl_vTotalStock.at(0)->GetMarket(), 1);
    EXPECT_STREQ(gl_ChinaStockMarket.gl_vTotalStock.at(2000)->GetStockCode(), _T("sz000000"));
    EXPECT_EQ(gl_ChinaStockMarket.gl_vTotalStock.at(2000)->GetMarket(), 2);
    for (int i = 0; i < 5000; i++) {
      EXPECT_EQ(gl_ChinaStockMarket.gl_vTotalStock.at(i)->GetIndex(), i);
    }
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sh600000")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sz000000")), 2000);
  }
  
  TEST_F(CMarketTest, TestCreateRTDataInquiringStr) {
    CString str;
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    CString strCompare, str2;
    strCompare = gl_ChinaStockMarket.gl_vTotalStock.at(898)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.gl_vTotalStock.at(1796)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.gl_vTotalStock.at(2694)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.gl_vTotalStock.at(3592)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.gl_vTotalStock.at(4490)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.gl_vTotalStock.at(0)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
  }

  TEST_F(CMarketTest, TestGetInquiringStockStr) {
    CString str;
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
    EXPECT_STREQ(strStartDay, _T("19700102")); //�������Ϊ19700101�����ǵڶ���
    // һ���ĸ���Ʊ���ڲ�ͬ��ԭ�򲻲�������ʷ����
    gl_ChinaStockMarket.gl_vTotalStock.at(1)->SetDayLineNeedUpdate(false);
    gl_ChinaStockMarket.gl_vTotalStock.at(5)->SetDayLineNeedUpdate(false);
    gl_ChinaStockMarket.gl_vTotalStock.at(7)->SetIPOStatus(__STOCK_NULL__);
    gl_ChinaStockMarket.gl_vTotalStock.at(6)->SetDayLineEndDay(20190101);
    for (int i = 0; i < (12 - 4); i++) { // ��������İ˸���Ʊ����������������Ч���봦
      strStartDay = _T("");
      str = _T("");
      gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay);
      EXPECT_STREQ(strStartDay, _T("19700102"));
    }
    strStartDay = _T("");
    str = _T("");
    EXPECT_TRUE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay)); // �ҵ���Ʊ����ʱ������ֵ
    EXPECT_STREQ(str, _T("0600015")); // 600013��600014Ϊ��Ч���룬�ʶ���ȥ��ȡ����������
    EXPECT_STREQ(strStartDay, _T("19700102")); //�������Ϊ19700101�����ǵڶ���

    // ��������2000��Ϊ�����ѯ״̬
    for (int i = 16; i < 2016; i++) {
      gl_ChinaStockMarket.gl_vTotalStock.at(i)->SetDayLineNeedUpdate(false);
    }
    EXPECT_FALSE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay)); // �˴����1000�������ؼ�ֵ
    EXPECT_FALSE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay));
    str = _T("");
    strStartDay = _T("");
    EXPECT_TRUE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay));
    EXPECT_STREQ(str, _T("1000016"));
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
    EXPECT_FALSE(gl_ChinaStockMarket.IsStock(_T("sh60000"), pstock));
    EXPECT_EQ(pstock, nullptr);
  }

  TEST_F(CMarketTest, TestGetStockName) {
    // δʵ��.����stockName�洢ʱʹ�õ���UniCode��ʽ������ϵͳĬ����Ansi��ʽ�������޷������ַ����Աȡ���ʱ�����в����ˡ�
    //EXPECT_STREQ(gl_ChinaStockMarket.GetStockName(_T("sh600000")), _T("�ַ�����"));
    //EXPECT_STREQ(gl_ChinaStockMarket.GetStockName(_T("sh60000")), _T("")); // û�ҵ����ؿ��ַ���

  }

  TEST_F(CMarketTest, TestGetStockIndex) {
    long lIndex = -2;
    EXPECT_TRUE(gl_ChinaStockMarket.GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_STREQ(gl_ChinaStockMarket.GetStockPtr(lIndex)->GetStockCode(), _T("sh600000"));
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockIndex(_T("sh60000"), lIndex));
    EXPECT_EQ(lIndex, -1);
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
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T("sz000016")); // ǰ�����ʱ��ѯ��������봦
    gl_ChinaStockMarket.SetDownLoadingStockCodeStr(_T("abcd"));
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T("abcd"));
  }
  TEST_F(CMarketTest, TestGetRelativeStartDay) {
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900101);
    gl_ChinaStockMarket.SetRelativeStrongStartDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900103);
  }

  TEST_F(CMarketTest, TestGetRelativeEndDay) {
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 20190721); // �����ݻ᲻ʱ�仯����Ϊ�Ǵ���ʵ���ݿ��ȡ�ģ�ϵͳ�᲻ʱ���¡�
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900103);
  }

}