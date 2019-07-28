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
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.size(), 10000);   // ��ȫ�ֱ���gl_ChinaStockMarket��ʼ��ʱ��������ȫ����Ʊ�����
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
    EXPECT_STREQ(strStartDay, _T("19700102")); //�������Ϊ19700101�����ǵڶ���
    // һ���ĸ���Ʊ���ڲ�ͬ��ԭ�򲻲�������ʷ����
    gl_ChinaStockMarket.m_vChinaMarketAStock.at(1)->SetDayLineNeedUpdate(false);
    gl_ChinaStockMarket.m_vChinaMarketAStock.at(5)->SetDayLineNeedUpdate(false);
    gl_ChinaStockMarket.m_vChinaMarketAStock.at(7)->SetIPOStatus(__STOCK_NULL__);
    gl_ChinaStockMarket.m_vChinaMarketAStock.at(6)->SetDayLineEndDay(20190101);
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
      gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->SetDayLineNeedUpdate(false);
    }
    EXPECT_FALSE(gl_ChinaStockMarket.CreateDayLineInquiringStr(str, strStartDay)); // �˴����1000�������ؼ�ֵ
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
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T("sh603016")); // ǰ�����ʱ��ѯ��������봦
    gl_ChinaStockMarket.SetDownLoadingStockCodeStr(_T("abcd"));
    EXPECT_STREQ(gl_ChinaStockMarket.GetDownLoadingStockCodeStr(), _T("abcd"));
  }
  TEST_F(CMarketTest, TestGetRelativeStartDay) {
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900101);
    gl_ChinaStockMarket.SetRelativeStrongStartDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900103);
  }

  TEST_F(CMarketTest, TestGetRelativeEndDay) {
    //EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 20190727); // �����ݻ᲻ʱ�仯����Ϊ�Ǵ���ʵ���ݿ��ȡ�ģ�ϵͳ�᲻ʱ���¡���ʱ�����Գ�ʼ̬��
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900103);
  }

  TEST_F(CMarketTest, TestSchedulingTest) { // �������������һ��ҪС�ģ����ܵ��³��ָı�ϵͳʵ�����ݵ�����
    gl_ChinaStockMarket.SetSystemReady(false); // ��ֹ����ʱִ��ʵ�ʲ���

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