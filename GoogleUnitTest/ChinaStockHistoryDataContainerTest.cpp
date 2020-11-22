#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"ChinaStockHistoryData.h"
#include"ChinaStockHistoryDataContainer.h"

namespace StockAnalysisTest {
  class CHistoryDataContainerTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CHistoryDataContainerTest, TestSaveData) {
    CChinaStockHistoryDataContainer id;
    EXPECT_FALSE(id.SaveData(_T("sh600000")));
  }

  TEST_F(CHistoryDataContainerTest, TestLOadData) {
    CChinaStockHistoryDataContainer id;
    EXPECT_FALSE(id.LoadData(_T("sh600000")));
  }

  TEST_F(CHistoryDataContainerTest, TestGetContainer) {
    CChinaStockHistoryDataContainer id;
    CChinaStockHistoryDataPtr pData = make_shared<CChinaStockHistoryData>();
    pData->SetStakeCode(_T("sh600008"));
    pData->SetDate(20202020);
    id.StoreData(pData);

    auto pContainer = id.GetContainer();
    EXPECT_EQ((*pContainer)[0]->GetFormatedMarketDate(), 20202020);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRS1) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->SetRSIndex(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->SetRSIndex(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRS1(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRSIndex) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->SetRSIndex(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->SetRSIndex(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRSIndex1(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRSLogarithm) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->SetRSLogarithm(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->SetRSLogarithm(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRSLogarithm1(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRS3) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->Set3RS(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->Set3RS(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRS3(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRS5) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->Set5RS(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->Set5RS(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRS5(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRS10) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->Set10RS(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->Set10RS(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRS10(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRS30) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->Set30RS(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->Set30RS(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRS30(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRS60) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->Set60RS(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->Set60RS(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRS60(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestGetRS120) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->Set120RS(1.1);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->Set120RS(2.2);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    HistoryDataContainer.StoreData(pHistoryData);
    HistoryDataContainer.StoreData(pHistoryData2);

    vector<double> vRS;
    vRS.resize(HistoryDataContainer.GetDataSize());

    HistoryDataContainer.GetRS120(vRS);
    EXPECT_DOUBLE_EQ(vRS[0], 1.1);
    EXPECT_DOUBLE_EQ(vRS[1], 2.2);
  }

  TEST_F(CHistoryDataContainerTest, TestUpdateData1) {
    CChinaStockHistoryDataPtr pHistoryData = make_shared<CChinaStockHistoryData>();
    pHistoryData->SetStakeCode(_T("sh600000"));
    pHistoryData->SetDate(20200101);
    pHistoryData->SetHigh(1000);
    pHistoryData->SetLow(200);
    CChinaStockHistoryDataPtr pHistoryData2 = make_shared<CChinaStockHistoryData>();
    pHistoryData2->SetStakeCode(_T("sh600001"));
    pHistoryData2->SetDate(20200201);
    pHistoryData2->SetHigh(11000);
    pHistoryData2->SetLow(1200);

    CChinaStockHistoryDataContainer HistoryDataContainer;
    vector<CChinaStockHistoryDataPtr> vHistoryData;

    vHistoryData.push_back(pHistoryData);
    EXPECT_EQ(HistoryDataContainer.GetDataSize(), 0);
    HistoryDataContainer.StoreData(pHistoryData2);
    EXPECT_EQ(HistoryDataContainer.GetDataSize(), 1);
    HistoryDataContainer.UpdateData(vHistoryData);
    EXPECT_EQ(HistoryDataContainer.GetDataSize(), 1);
    pHistoryData2 = HistoryDataContainer.GetData(0);
    EXPECT_EQ(pHistoryData2->GetFormatedMarketDate(), 20200101);
    EXPECT_EQ(pHistoryData2->GetHigh(), 1000);
    EXPECT_EQ(pHistoryData2->GetLow(), 200);
    EXPECT_TRUE(HistoryDataContainer.IsDataLoaded());
  }
}