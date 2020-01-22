#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"
#include"Stock.h"

#include"SetStockCode.h"

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  class CChinaMarket : public ::testing::Test
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
      gl_ChinaStockMarket.ResetNeteaseRTDataInquiringIndex();
      gl_ChinaStockMarket.ResetNeteaseDayLineDataInquiringIndex();
      gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
      gl_ChinaStockMarket.ResetTengxunRTDataInquiringIndex();
    }

    static void TearDownTestCase() {
      gl_ChinaStockMarket.ResetNeteaseRTDataInquiringIndex();
      gl_ChinaStockMarket.ResetNeteaseDayLineDataInquiringIndex();
      gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
      gl_ChinaStockMarket.ResetTengxunRTDataInquiringIndex();
    }
  };

  TEST_F(CChinaMarket, TestInitialize) {
    CStockPtr pStock = nullptr;
    EXPECT_GT(gl_ChinaStockMarket.GetTotalActiveStock(), 0);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_EQ(gl_ChinaStockMarket.GetCurrentStockPtr(), nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentEditStockChanged());
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalAttackBuyAmount(), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalAttackSellAmount(), 0);
    EXPECT_TRUE(gl_ChinaStockMarket.IsGetRTData());
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
    EXPECT_EQ(gl_ChinaStockMarket.GetCountDownSlowReadingRTData(), 3);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStock(), 12000);   // ��ȫ�ֱ���gl_ChinaStockMarket��ʼ��ʱ��������ȫ����Ʊ�����
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockMapIndexSize(), 12000);
    pStock = gl_ChinaStockMarket.GetStockPtr(0);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(pStock->GetMarket(), __SHANGHAI_MARKET__);
    pStock = gl_ChinaStockMarket.GetStockPtr(6000);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sz000000"));
    EXPECT_EQ(pStock->GetMarket(), __SHENZHEN_MARKET__);
    for (int i = 0; i < 12000; i++) {
      pStock = gl_ChinaStockMarket.GetStockPtr(i);
      EXPECT_EQ(pStock->GetOffset(), i);
      EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
      EXPECT_FALSE(pStock->IsDayLineNeedProcess());
      EXPECT_FALSE(pStock->IsDayLineNeedSaving());
      if ((pStock->GetStockCode() >= _T("sh000000")) && (pStock->GetStockCode() <= _T("sh000999"))) {
        EXPECT_FALSE(pStock->IsNeedProcessRTData());
      }
      else if ((pStock->GetStockCode() >= _T("sz399000")) && (pStock->GetStockCode() <= _T("sz399999"))) {
        EXPECT_FALSE(pStock->IsNeedProcessRTData());
      }
      else EXPECT_TRUE(pStock->IsNeedProcessRTData());
    }
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sh600000")), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStockIndex(_T("sz000000")), 6000);

    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetMarket());
    EXPECT_EQ(gl_ChinaStockMarket.GetDayLineNeedProcessNumber(), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetDayLineNeedUpdateNumber(), 12000);
    EXPECT_EQ(gl_ChinaStockMarket.GetDayLineNeedSaveNumber(), 0);
    EXPECT_TRUE(gl_ChinaStockMarket.IsUsingSinaRTDataReceiver());
    EXPECT_TRUE(gl_ChinaStockMarket.IsUsingTengxunRTDataReceiver());
    EXPECT_FALSE(gl_ChinaStockMarket.IsUsingNeteaseRTDataReceiver());

    EXPECT_EQ(gl_ChinaStockMarket.GetNeteaseDayLineDataInquiringIndex(), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetNeteaseRTDataInquiringIndex(), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaRTDataInquiringIndex(), 0);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 0);
  }

  TEST_F(CChinaMarket, TestClearUpdateStockCodeDBFlag) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsUpdateStockCodeDB());
    for (int i = 0; i < gl_ChinaStockMarket.GetTotalStock(); i++) {
      CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(i);
      pStock->SetDayLineDBUpdated(true);
    }
    EXPECT_TRUE(gl_ChinaStockMarket.IsUpdateStockCodeDB());
    gl_ChinaStockMarket.ClearUpdateStockCodeDBFlag();
    EXPECT_FALSE(gl_ChinaStockMarket.IsUpdateStockCodeDB());
  }

  TEST_F(CChinaMarket, TestGetTengxunInquiringStockStr) {
    CStockPtr pStock = nullptr;
    gl_ChinaStockMarket.SetSystemReady(true);
    gl_ChinaStockMarket.ResetTengxunRTDataInquiringIndex();
    CString str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 899);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    CString strCompare, str2;
    pStock = gl_ChinaStockMarket.GetStockPtr(899);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 2697);
    pStock = gl_ChinaStockMarket.GetStockPtr(1798);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 3596);
    pStock = gl_ChinaStockMarket.GetStockPtr(2697);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 4495);
    pStock = gl_ChinaStockMarket.GetStockPtr(3596);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 5394);
    pStock = gl_ChinaStockMarket.GetStockPtr(4495);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 6293);
    pStock = gl_ChinaStockMarket.GetStockPtr(5394);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 7192);
    pStock = gl_ChinaStockMarket.GetStockPtr(6293);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 8091);
    pStock = gl_ChinaStockMarket.GetStockPtr(7192);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(8091);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(8990);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 10788);
    pStock = gl_ChinaStockMarket.GetStockPtr(9889);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 11687);
    pStock = gl_ChinaStockMarket.GetStockPtr(10788);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 0);
    pStock = gl_ChinaStockMarket.GetStockPtr(11687);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetTengxunInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetTengxunRTDataInquiringIndex(), 899);
    pStock = gl_ChinaStockMarket.GetStockPtr(0);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    gl_ChinaStockMarket.ResetTengxunRTDataInquiringIndex();
  }

  TEST_F(CChinaMarket, TestGetSinaInquiringStockStr2) {
    CStockPtr pStock = nullptr;
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaRTDataInquiringIndex(), 899);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaRTDataInquiringIndex(), 1798);
    CString strCompare, str2;
    pStock = gl_ChinaStockMarket.GetStockPtr(899);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(1798);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(2697);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(3596);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(4495);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(5394);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(6293);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(7192);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(8091);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(8990);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(9889);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(10788);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetSinaRTDataInquiringIndex(), 0);
    pStock = gl_ChinaStockMarket.GetStockPtr(11687);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(0);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(8);
    EXPECT_EQ(str2.Compare(strCompare), 0);
  }

  TEST_F(CChinaMarket, TestGetInquiringStockStr) {
    CString str2;

    gl_ChinaStockMarket.SetSystemReady(true);
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900);
    str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900);
    if (gl_ChinaStockMarket.GetSinaRTDataInquiringIndex() == 0) {
      str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900); // Ŀǰ������ǧ����Ծ��Ʊ���ʶ����μ��ɱ���һ��
      str2 = str.Left(9);
      EXPECT_EQ(str2, _T("sh600000,"));
    }
    else {
      str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900);
      str = gl_ChinaStockMarket.GetSinaInquiringStockStr(900);
      CString str2 = str.Left(9);
      EXPECT_STREQ(str2, _T("sh600000,"));
    }
  }

  TEST_F(CChinaMarket, TestGetNeteaseInquiringStockStr) {
    CStockPtr pStock = nullptr;
    gl_ChinaStockMarket.ResetNeteaseRTDataInquiringIndex();
    CString str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetNeteaseRTDataInquiringIndex(), 899);
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    CString strCompare, str2;
    pStock = gl_ChinaStockMarket.GetStockPtr(899);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(1798);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(2697);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(3596);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(4495);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(5394);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(6293);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(7192);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(8091);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(8990);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(9889);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(10788);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    EXPECT_EQ(gl_ChinaStockMarket.GetNeteaseRTDataInquiringIndex(), 0);
    pStock = gl_ChinaStockMarket.GetStockPtr(11687);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    str = gl_ChinaStockMarket.GetNeteaseInquiringStockStr(900, false);
    pStock = gl_ChinaStockMarket.GetStockPtr(0);
    strCompare = pStock->GetStockCode();
    str2 = str.Left(7);
    EXPECT_STREQ(str2.Right(6), strCompare.Right(6));
    gl_ChinaStockMarket.ResetNeteaseRTDataInquiringIndex();
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
  }

  TEST_F(CChinaMarket, TestGetNeteaseDayLineInquiringStr) {
    CString str;
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(0);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    pStock = gl_ChinaStockMarket.GetStockPtr(1);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    pStock = gl_ChinaStockMarket.GetStockPtr(2);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
    str = gl_ChinaStockMarket.CreateNeteaseDayLineInquiringStr();
    EXPECT_TRUE(str.GetLength() > 0);
    EXPECT_STREQ(str, _T("0600000")); // ����ʱʹ��teststock���ݿ⣬�����ݿ�ȽϾɣ�������ʱ�䲻�����գ��ʶ���Ծ��ƱҲ��Ҫ��������
    pStock = gl_ChinaStockMarket.GetStockPtr(0);
    EXPECT_FALSE(pStock->IsDayLineNeedUpdate());
    pStock = gl_ChinaStockMarket.GetStockPtr(1);
    EXPECT_TRUE(pStock->IsDayLineNeedUpdate());

    // �ָ�ԭ״
    for (int i = 0; i < 12000; i++) {
      pStock = gl_ChinaStockMarket.GetStockPtr(i);
      if (!pStock->IsDayLineNeedUpdate()) {
        pStock->SetDayLineNeedUpdate(true);
      }
    }
  }

  TEST_F(CChinaMarket, TestIsAStock) {
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

  TEST_F(CChinaMarket, TestIsAStock2) {
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

  TEST_F(CChinaMarket, TestIsStock) {
    EXPECT_GT(gl_ChinaStockMarket.GetTotalActiveStock(), 1);
    EXPECT_TRUE(gl_ChinaStockMarket.IsStock(_T("sh600000")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsStock(_T("sh60000")));
  }

  TEST_F(CChinaMarket, TestGetStockName) {
    //δʵ��.����stockName�洢ʱʹ�õ���UniCode��ʽ������ϵͳĬ����Ansi��ʽ�������޷������ַ����Աȡ���ʱ�����в����ˡ�
    //EXPECT_STREQ(gl_ChinaStockMarket.GetStockName(_T("sh600000")), _T("�ַ�����"));
    EXPECT_STREQ(gl_ChinaStockMarket.GetStockName(_T("sh60000")), _T("")); // û�ҵ����ؿ��ַ���
  }

  TEST_F(CChinaMarket, TestGetStockIndex) {
    long lIndex = -2;
    EXPECT_TRUE(gl_ChinaStockMarket.GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_STREQ(gl_ChinaStockMarket.GetStockPtr(lIndex)->GetStockCode(), _T("sh600000"));
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockIndex(_T("sh60000"), lIndex));
    EXPECT_EQ(lIndex, -1);
  }

  TEST_F(CChinaMarket, TestIncreaseTotalActiveStock) {
    long l = gl_ChinaStockMarket.GetTotalActiveStock();
    gl_ChinaStockMarket.IncreaseActiveStockNumber();
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalActiveStock(), l + 1);
  }

  TEST_F(CChinaMarket, TestGetStockCode) {
    EXPECT_EQ(gl_ChinaStockMarket.GetStockPtr(_T("sh66000")), nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockPtr(_T("sh600001")) == nullptr);

    EXPECT_EQ(gl_ChinaStockMarket.GetStockPtr(-1), nullptr);
    EXPECT_EQ(gl_ChinaStockMarket.GetStockPtr(12000), nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockPtr(0) == nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.GetStockPtr(11999) == nullptr);
  }

  TEST_F(CChinaMarket, TestGetShowStock) {
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

  TEST_F(CChinaMarket, TestMarketReady) {
    gl_ChinaStockMarket.SetSystemReady(true);
    EXPECT_TRUE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.SetSystemReady(false);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
  }

  TEST_F(CChinaMarket, TestIsTodayStockProcessed) {
    gl_systemTime.CalculateTime();
    if (gl_systemTime.GetTime() > 150000) {
      EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockProcessed());
    }
    else {
      EXPECT_FALSE(gl_ChinaStockMarket.IsTodayStockProcessed());
    }
    gl_ChinaStockMarket.SetTodayStockProcessedFlag(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockProcessed());
    gl_ChinaStockMarket.SetTodayStockProcessedFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayStockProcessed());
  }

  TEST_F(CChinaMarket, TestIsDayLineNeedUpdate) {
    CStockPtr pStock = nullptr;
    EXPECT_TRUE(gl_ChinaStockMarket.IsDayLineNeedUpdate());
    for (int i = 0; i < gl_ChinaStockMarket.GetTotalStock(); i++) {
      pStock = gl_ChinaStockMarket.GetStockPtr(i);
      pStock->SetDayLineNeedUpdate(false);
    }
    EXPECT_FALSE(gl_ChinaStockMarket.IsDayLineNeedUpdate());
  }

  TEST_F(CChinaMarket, TestIsLoadSelectedStock) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
  }

  TEST_F(CChinaMarket, TestTaskResetMarket) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetMarket());
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetMarket(91259);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetMarket(91401);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    gl_ChinaStockMarket.TaskResetMarket(91300);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
  }

  TEST_F(CChinaMarket, TestTaskResetMarket2) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetMarket());
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
    gl_ChinaStockMarket.TaskResetMarket(91259);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
    gl_ChinaStockMarket.TaskResetMarket(91401);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
    gl_ChinaStockMarket.TaskResetMarket(91400);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
  }

  TEST_F(CChinaMarket, TestTaskResetMarketAgain) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetMarket());
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
    gl_ChinaStockMarket.TaskResetMarketAgain(92459);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
    gl_ChinaStockMarket.TaskResetMarketAgain(93001);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
    gl_ChinaStockMarket.TaskResetMarketAgain(92500);
    EXPECT_FALSE(gl_ChinaStockMarket.SystemReady());
    EXPECT_FALSE(gl_ChinaStockMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
  }

  TEST_F(CChinaMarket, TestCheckMarketOpen) {
    tm tm_;
    tm_.tm_wday = 1;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(91359));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCheckMarketOpen(91400));
    EXPECT_TRUE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCheckMarketOpen(113500));
    EXPECT_TRUE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(113501));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(125459));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCheckMarketOpen(125500));
    EXPECT_TRUE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_TRUE(gl_ChinaStockMarket.TaskCheckMarketOpen(150630));
    EXPECT_TRUE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(150631));
    tm_.tm_wday = 0;
    gl_systemTime.__Test_SetTM(tm_);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(91459));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(91500));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(113459));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(113500));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(125459));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(125500));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(150630));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
    EXPECT_FALSE(gl_ChinaStockMarket.TaskCheckMarketOpen(150631));
    EXPECT_FALSE(gl_ChinaStockMarket.IsMarketOpened());
  }

  TEST_F(CChinaMarket, TestTaskUpdateStockCodeDB) {
    gl_ChinaStockMarket.SetSaveDayLine(true);
    gl_ChinaStockMarket.SetDayLineNeedSaveNumber(1);
    gl_ChinaStockMarket.SetDayLineNeedProcessNumber(1);
    gl_ChinaStockMarket.SetDayLineNeedUpdateNumber(1);
    gl_ChinaStockMarket.TaskUpdateStockCodeDB();
    EXPECT_TRUE(gl_ChinaStockMarket.IsSaveDayLine());
    gl_ChinaStockMarket.SetDayLineNeedProcessNumber(0);
    gl_ChinaStockMarket.TaskUpdateStockCodeDB();
    EXPECT_TRUE(gl_ChinaStockMarket.IsSaveDayLine());
    gl_ChinaStockMarket.SetDayLineNeedSaveNumber(0);
    gl_ChinaStockMarket.TaskUpdateStockCodeDB();
    EXPECT_TRUE(gl_ChinaStockMarket.IsSaveDayLine());
    gl_ChinaStockMarket.SetDayLineNeedUpdateNumber(0);
    gl_ChinaStockMarket.TaskUpdateStockCodeDB();
    EXPECT_FALSE(gl_ChinaStockMarket.IsSaveDayLine());
  }

  TEST_F(CChinaMarket, TestIncreaseNeteaseDayLineInquiringIndex) {
    long k = 0;
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalStock(), 12000);
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

  TEST_F(CChinaMarket, TestStepToActiveStockIndex) {
    long i = 0; //
    int k = 0;
    gl_ChinaStockMarket.StepToActiveStockIndex(i);
    EXPECT_EQ(i, 0); // sh600000Ϊ��Ծ��Ʊ
    i++;
    gl_ChinaStockMarket.StepToActiveStockIndex(i);
    EXPECT_EQ(i, 4); // sh600004Ϊ��Ծ��Ʊ��֮�����������й�
    for (int j = 1; j < 4; j++) {
      EXPECT_FALSE(gl_ChinaStockMarket.GetStockPtr(j)->IsActive());
    }
  }

  TEST_F(CChinaMarket, TestResetMarketFlagAtMidnight) { // �����ʵ�ǲ��Ե�CVirtualMarket���еĺ�����
    EXPECT_FALSE(gl_ChinaStockMarket.IsPermitResetMarket());
    gl_ChinaStockMarket.ResetMarketFlagAtMidnight(0);
    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetMarket());
    gl_ChinaStockMarket.SetPermitResetMarket(false);
    gl_ChinaStockMarket.ResetMarketFlagAtMidnight(1501);
    EXPECT_FALSE(gl_ChinaStockMarket.IsPermitResetMarket());
    gl_ChinaStockMarket.ResetMarketFlagAtMidnight(1500);
    EXPECT_TRUE(gl_ChinaStockMarket.IsPermitResetMarket());
  }

  TEST_F(CChinaMarket, TestProcessTodayStock) {
    gl_ChinaStockMarket.SetSystemReady(false);
    gl_ChinaStockMarket.SetTodayStockProcessedFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskProcessTodayStock(150259));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskProcessTodayStock(150300));
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayStockProcessed());
    gl_ChinaStockMarket.SetSystemReady(true);
    gl_ChinaStockMarket.SetTodayStockProcessedFlag(true);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskProcessTodayStock(150259));
    EXPECT_FALSE(gl_ChinaStockMarket.TaskProcessTodayStock(150300));
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockProcessed());
    gl_ChinaStockMarket.SetSystemReady(true);
    gl_ChinaStockMarket.SetTodayStockProcessedFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.TaskProcessTodayStock(150259));
    EXPECT_TRUE(gl_ChinaStockMarket.TaskProcessTodayStock(150300));
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockProcessed());
  }

  TEST_F(CChinaMarket, TestSetCheckActiveStockFlag) {
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(91459);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(91500);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(92859);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(92900);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(113059);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(113100);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(125859);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(125900);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.TaskSetCheckActiveStockFlag(150000);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckActiveStock());
  }

  TEST_F(CChinaMarket, TestLoadStockCodeDB) {
    // ��Ʊ�������ݿ���ȫ�ֻ�������ʱ����װ�����ϵͳ���ʶ�ֱ�Ӳ��Լ��ɡ�
    CStockPtr pStock = nullptr;
    pStock = gl_ChinaStockMarket.GetStockPtr(0);
    EXPECT_EQ(pStock->GetIPOStatus(), __STOCK_IPOED__);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600000"));
    EXPECT_EQ(pStock->GetDayLineStartDay(), 19991110);
    EXPECT_TRUE(pStock->IsActive());
    pStock = gl_ChinaStockMarket.GetStockPtr(1);
    EXPECT_EQ(pStock->GetIPOStatus(), __STOCK_DELISTED__);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600001"));
    EXPECT_EQ(pStock->GetDayLineStartDay(), 19980122);
    EXPECT_EQ(pStock->GetDayLineEndDay(), 20091215);
    EXPECT_FALSE(pStock->IsActive());
  }

  TEST_F(CChinaMarket, TestGetLastLoginDay) {
    gl_ChinaStockMarket.SetLastLoginDay(19900102);
    EXPECT_EQ(gl_ChinaStockMarket.GetLastLoginDay(), 19900102);
  }

  TEST_F(CChinaMarket, TestGetRelativeStrongStartDay) {
    gl_ChinaStockMarket.SetRelativeStrongStartDay(19900202);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900202);
  }

  TEST_F(CChinaMarket, TestGetRelativeStrongEndDay) {
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900302);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900302);
  }

  TEST_F(CChinaMarket, TestGetReadingSinaRTDataTime) {
    time_t tt = 1010101010;
    gl_ChinaStockMarket.SetReadingSinaRTDataTime(tt);
    EXPECT_EQ(gl_ChinaStockMarket.GetReadingSinaRTDataTime(), tt);
  }

  TEST_F(CChinaMarket, TestGetReadingNeteaseDayLineDataTime) {
    time_t tt = 1010101010;
    gl_ChinaStockMarket.SetReadingNeteaseDayLineDataTime(tt);
    EXPECT_EQ(gl_ChinaStockMarket.GetReadingNeteaseDayLineDataTime(), tt);
  }

  TEST_F(CChinaMarket, TestGetReadingTengxunRTDataTime) {
    time_t tt = 1010101010;
    gl_ChinaStockMarket.SetReadingTengxunRTDataTime(tt);
    EXPECT_EQ(gl_ChinaStockMarket.GetReadingTengxunRTDataTime(), tt);
  }

  TEST_F(CChinaMarket, TesstIsTodayTempRTDataLoaded) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
    gl_ChinaStockMarket.SetTodayTempRTDataLoaded(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
    gl_ChinaStockMarket.SetTodayTempRTDataLoaded(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayTempRTDataLoaded());
  }

  TEST_F(CChinaMarket, TesstIsCheckActiveStock) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.SetCheckActiveStock(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCheckActiveStock());
    gl_ChinaStockMarket.SetCheckActiveStock(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCheckActiveStock());
  }

  TEST_F(CChinaMarket, TestGetTotalActiveStock) {
    long l = gl_ChinaStockMarket.GetTotalActiveStock();
    gl_ChinaStockMarket.SetTotalActiveStock(4000);
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalActiveStock(), 4000);
    gl_ChinaStockMarket.SetTotalActiveStock(l);
  }

  TEST_F(CChinaMarket, TestGetStockPtr) {
    CStockPtr pStock = nullptr;
    long lIndex = -1;
    pStock = gl_ChinaStockMarket.GetStockPtr(_T("sh600000"));
    EXPECT_TRUE(gl_ChinaStockMarket.GetStockIndex(_T("sh600000"), lIndex));
    EXPECT_EQ(lIndex, 0);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600000"));
  }

  TEST_F(CChinaMarket, TestIsValidNeteaseRTDataPrefix) {
    CWebDataReceivedPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebDataReceived>();
    CString str = _T("_ntes_quote_callback({\"");
    pWebDataReceived->m_pDataBuffer = new char[50];
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_ChinaStockMarket.IsValidNeteaseRTDataPrefix(pWebDataReceived));
    str = _T("_ntes_quo_callback({\"");
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_ChinaStockMarket.IsValidNeteaseRTDataPrefix(pWebDataReceived));
  }

  TEST_F(CChinaMarket, TestIsValidTengxunRTDataPrefix) {
    CWebDataReceivedPtr pWebDataReceived;
    pWebDataReceived = make_shared<CWebDataReceived>();
    CString str = _T("v_pv_none_match=\"1\";\n");
    pWebDataReceived->m_pDataBuffer = new char[50];
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_TRUE(gl_ChinaStockMarket.IsInvalidTengxunRTData(pWebDataReceived));
    EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
    str = _T("v_pv_none_mtch=\"1\";\n");
    strcpy_s(pWebDataReceived->m_pDataBuffer, 30, (LPSTR)str.GetBuffer());
    pWebDataReceived->m_lBufferLength = str.GetLength();
    pWebDataReceived->ResetCurrentPos();
    EXPECT_FALSE(gl_ChinaStockMarket.IsInvalidTengxunRTData(pWebDataReceived));
    EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
  }
}