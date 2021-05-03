#include"pch.h"

#include"globedef.h"

#include"WebInquirer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  class CWebInquirerTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);
      EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing());
    }

    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
    }

    virtual void TearDown(void) override {
      // clearup
      for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
        gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
      }
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_WebInquirer.GetPotenDailyBriefingDataSize() > 0) gl_WebInquirer.PopPotenDailyBriefingData();
      EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing());
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
    }
  };

  TEST_F(CWebInquirerTest, TestInitialize) {
    EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing());
    EXPECT_FALSE(gl_WebInquirer.IsReadingCrweberIndex());
    EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 0);
    EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 0);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 0);
    EXPECT_EQ(gl_WebInquirer.GetTengxunRTDataSize(), 0);
  }

  TEST_F(CWebInquirerTest, TestPushPop) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    CWebDataPtr pWebData2;
    pWebData->SetTime(101010);
    gl_WebInquirer.PushSinaRTData(pWebData);
    pWebData2 = gl_WebInquirer.PopSinaRTData();
    EXPECT_EQ(pWebData2->GetTime(), 101010);
  }

  TEST_F(CWebInquirerTest, TestPushPopSinaData) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    CWebDataPtr pWebData2;
    pWebData->SetTime(101010);
    gl_WebInquirer.PushSinaRTData(pWebData);
    EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 1);
    pWebData2 = gl_WebInquirer.PopSinaRTData();
    EXPECT_EQ(pWebData2->GetTime(), 101010);
  }

  TEST_F(CWebInquirerTest, TestPushPopNeteaseData) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    CWebDataPtr pWebData2;
    pWebData->SetTime(101010);
    gl_WebInquirer.PushNeteaseRTData(pWebData);
    EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 1);
    pWebData2 = gl_WebInquirer.PopNeteaseRTData();
    EXPECT_EQ(pWebData2->GetTime(), 101010);
  }

  TEST_F(CWebInquirerTest, TestPushPopTengxunData) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    CWebDataPtr pWebData2;
    pWebData->SetTime(101010);
    gl_WebInquirer.PushTengxunRTData(pWebData);
    EXPECT_EQ(gl_WebInquirer.GetTengxunRTDataSize(), 1);
    pWebData2 = gl_WebInquirer.PopTengxunRTData();
    EXPECT_EQ(pWebData2->GetTime(), 101010);
  }

  TEST_F(CWebInquirerTest, TestPushPopCrweberData) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    CWebDataPtr pWebData2;
    pWebData->SetTime(101010);
    gl_WebInquirer.PushCrweberData(pWebData);
    EXPECT_EQ(gl_WebInquirer.GetCrweberDataSize(), 1);
    pWebData2 = gl_WebInquirer.PopCrweberData();
    EXPECT_EQ(pWebData2->GetTime(), 101010);
  }

  TEST_F(CWebInquirerTest, TestPushPopPotenDailyBriefingData) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    CWebDataPtr pWebData2;
    pWebData->SetTime(101010);
    EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing());
    gl_WebInquirer.PushPotenDailyBriefingData(pWebData);
    EXPECT_EQ(gl_WebInquirer.GetPotenDailyBriefingDataSize(), 1);
    pWebData2 = gl_WebInquirer.PopPotenDailyBriefingData();
    EXPECT_EQ(pWebData2->GetTime(), 101010);
  }

  TEST_F(CWebInquirerTest, TestGetNeteaseDayLineData) {
    int iSaved = gl_iMaxSavingOneDayLineThreads;
    EXPECT_FALSE(gl_pNeteaseDayLineWebInquiry2->IsReadingWebData());
    EXPECT_FALSE(gl_pNeteaseDayLineWebInquiry->IsReadingWebData());

    for (gl_iMaxSavingOneDayLineThreads = 2; gl_iMaxSavingOneDayLineThreads < 7; gl_iMaxSavingOneDayLineThreads++) {
      EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry, PrepareNextInquiringStr)
        .Times(1)
        .WillOnce(Return(true))
        .RetiresOnSaturation();
      EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry2, PrepareNextInquiringStr)
        .Times(1)
        .WillOnce(Return(true))
        .RetiresOnSaturation();
      EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry2, StartReadingThread)
        .Times(1)
        .RetiresOnSaturation();
      EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry, StartReadingThread())
        .Times(1)
        .RetiresOnSaturation();
      EXPECT_TRUE(gl_WebInquirer.GetNeteaseDayLineData());
      EXPECT_TRUE(gl_pNeteaseDayLineWebInquiry2->IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
      EXPECT_TRUE(gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
      gl_pNeteaseDayLineWebInquiry2->SetReadingWebData(false);
      gl_pNeteaseDayLineWebInquiry->SetReadingWebData(false);
    }

    gl_iMaxSavingOneDayLineThreads = 1;
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry, PrepareNextInquiringStr)
      .Times(1)
      .WillOnce(Return(true))
      .RetiresOnSaturation();
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry, StartReadingThread())
      .Times(1)
      .RetiresOnSaturation();
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry2, StartReadingThread()).Times(0);
    EXPECT_TRUE(gl_WebInquirer.GetNeteaseDayLineData());
    EXPECT_TRUE(gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
    gl_pNeteaseDayLineWebInquiry->SetReadingWebData(false);

    gl_iMaxSavingOneDayLineThreads = 7;
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry, PrepareNextInquiringStr)
      .Times(1)
      .WillOnce(Return(true))
      .RetiresOnSaturation();
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry, StartReadingThread())
      .Times(1)
      .RetiresOnSaturation();
    EXPECT_CALL(*gl_pNeteaseDayLineWebInquiry2, StartReadingThread()).Times(0); //Ĭ��״̬��ʹ��һ����ȡ��
    EXPECT_TRUE(gl_WebInquirer.GetNeteaseDayLineData());
    EXPECT_TRUE(gl_pNeteaseDayLineWebInquiry->IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
    gl_pNeteaseDayLineWebInquiry->SetReadingWebData(false);

    EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock()) << "����Mock�࣬û�������޸�";

    // �ָ�ԭ̬
    gl_pNeteaseDayLineWebInquiry2->SetReadingWebData(false);
    gl_pNeteaseDayLineWebInquiry->SetReadingWebData(false);
    gl_iMaxSavingOneDayLineThreads = iSaved;
  }
}