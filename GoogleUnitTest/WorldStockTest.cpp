#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

namespace StockAnalysisTest {
  class CWorldStockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }
    static void TearDownTestSuite(void) {
    }
    virtual void SetUp(void) override {
      pStock = nullptr;
    }

    virtual void TearDown(void) override {
      // clearu
    }

  protected:
    CWorldStockPtr pStock;
  };

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus1) {
    CWorldStock stock;

    stock.m_fDayLineNeedUpdate = true;
    stock.m_fIsActive = false;
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "�ǻ�Ծ��Ʊ�����ղ���������\n";
      stock.m_fDayLineNeedUpdate = true;
    }
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus2) {
    CWorldStock stock;

    stock.m_fDayLineNeedUpdate = true;
    stock.SetIPOStatus(__STAKE_NULL__);
    EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "��Ч��Ʊ���������\n";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus3) {
    CWorldStock stock;

    stock.m_fDayLineNeedUpdate = true;
    stock.SetIPOStatus(__STAKE_DELISTED__);
    for (int i = 0; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(0, 0, 0, i)) << "ժ�ƹ�Ʊֻ���������������\n";
      stock.m_fDayLineNeedUpdate = true;
    }
    EXPECT_TRUE(stock.CheckDayLineUpdateStatus(0, 0, 0, 6)) << "ժ�ƹ�Ʊֻ���������������\n";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus4) {
    CWorldStock stock;

    stock.m_fDayLineNeedUpdate = true;
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.m_fIsActive = true;
    stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(gl_pWorldMarket->GetFormatedMarketDate(), 100));
    EXPECT_TRUE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetFormatedMarketDate(), 20210108, 0, 1));
    stock.SetDayLineEndDate(gl_pWorldMarket->GetPrevDay(stock.GetDayLineEndDate()));
    EXPECT_FALSE(stock.CheckDayLineUpdateStatus(gl_pWorldMarket->GetFormatedMarketDate(), 20210108, 0, 1)) << "����100��Ĺ�Ʊ���ٸ�������";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus5) {
    CWorldStock stock;

    stock.m_fDayLineNeedUpdate = true;
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.m_fIsActive = true;
    stock.SetDayLineEndDate(20210108); // ���ս����������ѽ���
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(20210108, 20210107, 170001, i)) << "ʱ������17ʱ�󣬼�鵱������";
      stock.m_fDayLineNeedUpdate = true; // ����״̬
    }
    stock.SetDayLineEndDate(20210107); // ���ս�����������δ����
    for (int i = 1; i < 6; i++) {
      EXPECT_TRUE(stock.CheckDayLineUpdateStatus(20210108, 20210107, 170001, i)) << "ʱ������17ʱ�󣬼�鵱������";
    }
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus6) {
    CWorldStock stock;

    stock.m_fDayLineNeedUpdate = true;
    stock.SetIPOStatus(__STAKE_IPOED__);
    stock.m_fIsActive = true;
    stock.SetDayLineEndDate(20210107); // ��һ���������������ѽ���
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stock.CheckDayLineUpdateStatus(20210108, 20210107, 170000, i)) << "ʱ�䲻����17ʱ�������һ����������";
      stock.m_fDayLineNeedUpdate = true; // ����֮
    }
    stock.SetDayLineEndDate(20210106); // ��һ��������������δ����
    for (int i = 1; i < 6; i++) {
      EXPECT_TRUE(stock.CheckDayLineUpdateStatus(20210108, 20210107, 170000, i)) << "ʱ�䲻����17ʱ�������һ����������";
    }
  }

  TEST_F(CWorldStockTest, TestSaveDayLine) {
    CWorldStock stock;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    CSetWorldStockDayLine setDayLine;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockCode(_T("A"));
    pDayLine->SetDate(20200101); // �����Ҫ��ӽ����ݿ�
    pDayLine->SetClose(10010);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockCode(_T("A"));
    pDayLine->SetDate(20210101); // �����Ҫ��ӽ����ݿ�
    pDayLine->SetClose(12345);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockCode(_T("A"));
    pDayLine->SetDate(20210107); // ������ݿ����У��������
    pDayLine->SetClose(10020);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStockCode(_T("A"));
    pDayLine->SetDate(20210123); // �����Ҫ��ӽ����ݿ�
    pDayLine->SetClose(10030);
    vDayLine.push_back(pDayLine);

    stock.m_strSymbol = _T("A");
    stock.SetDayLineEndDate(20210107);
    stock.UpdateDayLine(vDayLine);

    stock.SaveDayLine();

    setDayLine.m_strFilter = _T("[Symbol] = 'A'");
    setDayLine.m_strSort = _T("[Date]");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    EXPECT_TRUE(setDayLine.m_Date == 20200101);
    EXPECT_STREQ(setDayLine.m_Close, _T("10.010"));
    setDayLine.Delete();
    while (setDayLine.m_Date != 20210101) setDayLine.MoveNext();
    EXPECT_STREQ(setDayLine.m_Close, _T("12.345"));
    setDayLine.Delete();
    setDayLine.MoveLast();
    EXPECT_TRUE(setDayLine.m_Date = 20210123);
    EXPECT_STREQ(setDayLine.m_Close, _T("10.030"));
    setDayLine.Delete();
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }
}