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
      ASSERT_FALSE(gl_fNormalMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }

    static void SetUpTestCase() { // ��������ĳ�ʼ������
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestCase() {
    }
  };

  TEST_F(CMarketTest, TestInitialize) {
    EXPECT_EQ(gl_ChinaStockMarket.m_strSinaRTDataInquire.Compare(_T("http://hq.sinajs.cn/list=")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.m_strTengxunRTDataInquire.Compare(_T("http://qt.gtimg.cn/q=")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.m_strNeteaseDayLineInquire.Compare(_T("http://quotes.money.163.com/service/chddata.html?code=")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.m_strNeteaseDayLinePostfix.Compare(_T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP")), 0);
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
    EXPECT_FALSE(gl_ChinaStockMarket.m_fGetDayLineData);
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
    EXPECT_EQ(gl_ChinaStockMarket.m_iCountDownDayLine, 3);
    EXPECT_EQ(gl_ChinaStockMarket.m_iCountDownSlowReadingRTData, 3);
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.size(), 12000);   // ��ȫ�ֱ���gl_ChinaStockMarket��ʼ��ʱ��������ȫ����Ʊ�����
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockMapIndexSize(), 12000);
    EXPECT_STREQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(0)->GetMarket(), __SHANGHAI_MARKET__);
    EXPECT_STREQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(6000)->GetStockCode(), _T("sz000000"));
    EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(6000)->GetMarket(), __SHENZHEN_MARKET__);
    for (int i = 0; i < 12000; i++) {
      EXPECT_EQ(gl_ChinaStockMarket.m_vChinaMarketAStock.at(i)->GetOffset(), i);
    }
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sh600000")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sz000000")), 6000);

    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetSystem());
  }

  TEST_F(CMarketTest, TestCreateRTDataInquiringStr) {
    CString str;
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    CString strCompare, str2;
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(898)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(1796)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(2694)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(3592)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(4490)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(5388)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(6286)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(7184)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8082)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(8980)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(9878)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(10776)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(str);
    strCompare = gl_ChinaStockMarket.m_vChinaMarketAStock.at(11674)->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
  }

  TEST_F(CMarketTest, TestGetInquiringStockStr) {
    gl_ChinaStockMarket.SetSystemReady(true);
    gl_ChinaStockMarket.ResetIT();
    CString str;
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str), 900);
    str = _T("");
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str), 900);
    str = _T("");
    long l = gl_ChinaStockMarket.GetSinaInquiringStockStr(str);
    if (l < 900) {
      str = _T("");
      EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str), 900);
      CString str2 = str.Left(9);
      EXPECT_STREQ(str2, _T("sh600000,"));
    }
    else {
      str = _T("");
      EXPECT_GT(900, gl_ChinaStockMarket.GetSinaInquiringStockStr(str)); // Ŀǰ������ǧ����Ծ��Ʊ���ʶ����μ��ɱ���һ��
      str = _T("");
      EXPECT_EQ(gl_ChinaStockMarket.GetSinaInquiringStockStr(str), 900);
      CString str2 = str.Left(9);
      EXPECT_STREQ(str2, _T("sh600000,"));
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
}