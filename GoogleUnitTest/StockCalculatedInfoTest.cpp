#include"stdafx.h"
#include "pch.h"

#include"globedef.h"
#include"accessory.h"

#include"StockCalculatedInfo.h"
#include"Stock.h"

namespace StockAnalysisTest {
  TEST(StockCalculatedInfoTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionTime(), 0);
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_EQ(id.GetCurrentAttackBuy(), 0);
    EXPECT_EQ(id.GetCurrentAttackSell(), 0);
    EXPECT_EQ(id.GetUnknownVolume(), 0);
    EXPECT_EQ(id.GetCurrentStrongBuy(), 0);
    EXPECT_EQ(id.GetCurrentStrongSell(), 0);
    EXPECT_EQ(id.GetCurrentUnknown(), 0);

    EXPECT_EQ(id.GetCancelBuyVolume(), 0);
    EXPECT_EQ(id.GetCancelSellVolume(), 0);

    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
    EXPECT_EQ(id.GetAttackBuyAmount(), 0);
    EXPECT_EQ(id.GetAttackSellAmount(), 0);

    EXPECT_EQ(id.GetAttackBuyBelow50000(), 0);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 0);
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 0);
    EXPECT_EQ(id.GetAttackSellBelow50000(), 0);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 0);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 0);

    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 0);
    EXPECT_EQ(id.GetOrdinarySellVolume(), 0);

    EXPECT_EQ(id.GetStrongBuyVolume(), 0);
    EXPECT_EQ(id.GetStrongSellVolume(), 0);

    EXPECT_EQ(id.GetTransactionNumber(), 0);
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), 0);
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 0);
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 0);
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 0);
  }
  TEST(CStockCalculatedInfoTest, TestGetTransactionTime) {
    ASSERT_FALSE(gl_fNormalMode);
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionTime(), 0);
    id.SetTransactionTime(1234567890);
    EXPECT_EQ(id.GetTransactionTime(), 1234567890);
  }

  TEST(StockCalculatedInfoTest, TestGetAttackBuyAmount) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyAmount(), 0);
    id.SetAttackBuyAmount(10101010);
    EXPECT_EQ(id.GetAttackBuyAmount(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetAttackSellAmount) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellAmount(), 0);
    id.SetAttackSellAmount(10101010);
    EXPECT_EQ(id.GetAttackSellAmount(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetOrdinaryBuyVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 0);
    id.SetOrdinaryBuyVolume(10101010);
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetOrdinarySellVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetOrdinarySellVolume(), 0);
    id.SetOrdinarySellVolume(10101010);
    EXPECT_EQ(id.GetOrdinarySellVolume(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetAttackBuyVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    id.SetAttackBuyVolume(10101010);
    EXPECT_EQ(id.GetAttackBuyVolume(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetStrongBuyVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetStrongBuyVolume(), 0);
    id.SetStrongBuyVolume(10101010);
    EXPECT_EQ(id.GetStrongBuyVolume(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetCurrentAttackBuy) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentAttackBuy(), 0);
    id.SetCurrentAttackBuy(10101010);
    EXPECT_EQ(id.GetCurrentAttackBuy(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetCurrentStrongBuy) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentStrongBuy(), 0);
    id.SetCurrentStrongBuy(10101010);
    EXPECT_EQ(id.GetCurrentStrongBuy(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetAttackSellVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
    id.SetAttackSellVolume(10101010);
    EXPECT_EQ(id.GetAttackSellVolume(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetStrongSellVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetStrongSellVolume(), 0);
    id.SetStrongSellVolume(10101010);
    EXPECT_EQ(id.GetStrongSellVolume(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetCurrentAttackSell) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentAttackSell(), 0);
    id.SetCurrentAttackSell(10101010);
    EXPECT_EQ(id.GetCurrentAttackSell(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetCurrentStrongSell) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentStrongSell(), 0);
    id.SetCurrentStrongSell(10101010);
    EXPECT_EQ(id.GetCurrentStrongSell(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetUnknownVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetUnknownVolume(), 0);
    id.SetUnknownVolume(10101010);
    EXPECT_EQ(id.GetUnknownVolume(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetCurrentUnknown) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentUnknown(), 0);
    id.SetCurrentUnknown(10101010);
    EXPECT_EQ(id.GetCurrentUnknown(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGetCancelBuyVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCancelBuyVolume(), 0);
    id.SetCancelBuyVolume(10101010);
    EXPECT_EQ(id.GetCancelBuyVolume(), 10101010);
  }
  TEST(StockCalculatedInfoTest, TestGetCancelSellVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCancelSellVolume(), 0);
    id.SetCancelSellVolume(10101010);
    EXPECT_EQ(id.GetCancelSellVolume(), 10101010);
  }

  TEST(StockCalculatedInfoTest, TestGeTransactionNumber) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumber(), 0);
    id.SetTransactionNumber(10101010);
    EXPECT_EQ(id.GetTransactionNumber(), 10101010);
    id.IncreaseTransactionNumber(101);
    EXPECT_EQ(id.GetTransactionNumber(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestGetTransactionNumberBelow5000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), 0);
    id.SetTransactionNumberBelow5000(10101010);
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), 10101010);
    id.IncreaseTransactionNumberBelow5000(101);
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestGetTransactionNumberBelow50000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 0);
    id.SetTransactionNumberBelow50000(10101010);
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 10101010);
    id.IncreaseTransactionNumberBelow50000(101);
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestGetTransactionNumberBelow200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 0);
    id.SetTransactionNumberBelow200000(10101010);
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 10101010);
    id.IncreaseTransactionNumberBelow200000(101);
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestGetTransactionNumberAbove200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 0);
    id.SetTransactionNumberAbove200000(10101010);
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 10101010);
    id.IncreaseTransactionNumberAbove200000(101);
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestGetAttackBuyBelow50000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 0);
    id.SetAttackBuyBelow50000(10101010);
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10101010);
    id.IncreaseAttackBuyBelow50000(101);
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestGetAttackBuyBelow200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 0);
    id.SetAttackBuyBelow200000(10101010);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 10101010);
    id.IncreaseAttackBuyBelow200000(101);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestGetAttackBuyAbove200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 0);
    id.SetAttackBuyAbove200000(10101010);
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 10101010);
    id.IncreaseAttackBuyAbove200000(101);
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestGetAttackSellBelow50000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellBelow50000(), 0);
    id.SetAttackSellBelow50000(10101010);
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10101010);
    id.IncreaseAttackSellBelow50000(101);
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestAttackSellBelow200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellBelow200000(), 0);
    id.SetAttackSellBelow200000(10101010);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 10101010);
    id.IncreaseAttackSellBelow200000(101);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestAttackSellAbove200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellAbove200000(), 0);
    id.SetAttackSellAbove200000(10101010);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 10101010);
    id.IncreaseAttackSellAbove200000(101);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 10101010 + 101);
  }

  TEST(StockCalculatedInfoTest, TestReset) {
    CStockCalculatedInfo id;

    id.SetAttackBuyAbove200000(3452345);
    id.SetAttackBuyAmount(45345);
    id.SetAttackBuyBelow200000(435345);
    id.SetAttackBuyBelow50000(435345);
    id.SetAttackBuyVolume(345345);
    id.SetAttackSellAbove200000(34598);
    id.SetAttackSellAmount(45345);
    id.SetAttackSellBelow200000(60596);
    id.SetAttackSellBelow50000(435);
    id.SetAttackSellVolume(3452);
    id.SetCancelBuyVolume(5560);
    id.SetCancelSellVolume(546);
    id.SetCurrentAttackBuy(45654);
    id.SetCurrentAttackSell(3455);
    id.SetCurrentStrongBuy(56);
    id.SetCurrentStrongSell(88686);
    id.SetCurrentUnknown(3454);
    id.SetOrdinaryBuyVolume(45);
    id.SetOrdinarySellVolume(3454);
    id.SetStrongBuyVolume(4654656);
    id.SetStrongSellVolume(4543);
    id.SetTransactionNumber(3454);
    id.SetTransactionNumberAbove200000(345345);
    id.SetTransactionNumberBelow200000(345);
    id.SetTransactionNumberBelow5000(4352);
    id.SetTransactionNumberBelow50000(354);
    id.Reset();
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 0);
    EXPECT_EQ(id.GetAttackBuyAmount(), 0);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 0);
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 0);
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 0);
    EXPECT_EQ(id.GetAttackSellAmount(), 0);
  }

  TEST(StockCalculatedInfoTest, TestSaveTempInfo) {
    CSetDayLineToday setDayLineToday;
    CStockCalculatedInfo id, id2;
    long lVolumeBegin = 10000000; //预设成交量为一亿股

    id.SetTransactionNumber(123435);
    id.SetTransactionNumberBelow5000(45346456);
    id.SetTransactionNumberBelow50000(5698);
    id.SetTransactionNumberBelow200000(67687);
    id.SetTransactionNumberAbove200000(6876);
    id.SetOrdinaryBuyVolume(435);
    id.SetOrdinarySellVolume(560985);
    id.SetAttackBuyBelow50000(54509);
    id.SetAttackBuyBelow200000(45049);
    id.SetAttackBuyAbove200000(34508);
    id.SetAttackSellBelow50000(45896);
    id.SetAttackSellBelow200000(56457);
    id.SetAttackSellAbove200000(8767);
    id.SetAttackBuyAmount(123456612);
    id.SetAttackBuyVolume(23423534);
    id.SetAttackSellAmount(4353454);
    id.SetAttackSellVolume(94589489);
    id.SetCancelBuyVolume(435245);
    id.SetCancelSellVolume(45648698);
    id.SetUnknownVolume(4895747);
    id.SetStrongBuyVolume(453456);
    id.SetStrongSellVolume(98976);
    id2 = id;

    ASSERT(!gl_fNormalMode);
    ASSERT(gl_fTestMode);
    setDayLineToday.Open();
    setDayLineToday.m_pDatabase->BeginTrans();
    while (!setDayLineToday.IsEOF()) {
      setDayLineToday.Delete();
      setDayLineToday.MoveNext();
    }
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.m_pDatabase->BeginTrans();
    setDayLineToday.AddNew();
    setDayLineToday.m_Volume = ConvertValueToString(lVolumeBegin);
    id.SaveTempInfo(setDayLineToday, true, 0);
    setDayLineToday.Update();
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.Close();

    setDayLineToday.Open();
    id.LoadAndCalculateTempInfo(setDayLineToday);
    setDayLineToday.Close();
    EXPECT_EQ(id.GetTransactionTime(), 0); //SaveTempInfo不存储交易时间
    EXPECT_EQ(id2.GetAttackBuyAbove200000(), id.GetAttackBuyAbove200000());
    EXPECT_EQ(id2.GetAttackBuyBelow200000(), id.GetAttackBuyBelow200000());
    EXPECT_EQ(id2.GetAttackBuyBelow50000(), id.GetAttackBuyBelow50000());
    EXPECT_EQ(id2.GetAttackBuyVolume(), id.GetAttackBuyVolume());
    EXPECT_EQ(id2.GetAttackSellAbove200000(), id.GetAttackSellAbove200000());
    EXPECT_EQ(id2.GetAttackSellBelow200000(), id.GetAttackSellBelow200000());
    EXPECT_EQ(id2.GetAttackSellBelow50000(), id.GetAttackSellBelow50000());
    EXPECT_EQ(id2.GetAttackSellVolume(), id.GetAttackSellVolume());

    EXPECT_EQ(id2.GetOrdinaryBuyVolume(), id.GetOrdinaryBuyVolume());
    EXPECT_EQ(id2.GetOrdinarySellVolume(), id.GetOrdinarySellVolume());
    EXPECT_EQ(id2.GetCancelBuyVolume(), id.GetCancelBuyVolume());
    EXPECT_EQ(id2.GetCancelSellVolume(), id.GetCancelSellVolume());
    EXPECT_EQ(id2.GetStrongBuyVolume(), id.GetStrongBuyVolume());
    EXPECT_EQ(id2.GetStrongSellVolume(), id.GetStrongSellVolume());

    EXPECT_EQ(id2.GetTransactionNumber(), id.GetTransactionNumber());
    EXPECT_EQ(id2.GetTransactionNumberAbove200000(), id.GetTransactionNumberAbove200000());
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), id.GetTransactionNumberBelow200000());
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), id.GetTransactionNumberBelow50000());
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), id.GetTransactionNumberBelow5000());

    EXPECT_EQ(id2.GetUnknownVolume() - lVolumeBegin, id.GetUnknownVolume()); //装入数据后要预设UnkownVolume，故而与存入数值相差一总股数
  }

  TEST(StockCalculatedInfoTest, TestSaveTodayInfo) {
    CSetDayLineInfo setDayLineInfo;
    CStockCalculatedInfo id;
    long lDay = 21090201;
    time_t tt = FormatToTTime(lDay);
    CString strDay = _T("21090201"); // 最好设置此日期为未来，以防止误操作实际数据库

    id.SetTransactionTime(tt);
    id.SetStockCode(_T("sh600008"));
    id.SetMarket(__SHANGHAI_MARKET__);
    id.SetTransactionNumber(123213);
    id.SetTransactionNumberBelow5000(453466456);
    id.SetTransactionNumberBelow50000(56698);
    id.SetTransactionNumberBelow200000(676687);
    id.SetTransactionNumberAbove200000(66876);
    id.SetOrdinaryBuyVolume(4635);
    id.SetOrdinarySellVolume(5606985);
    id.SetAttackBuyBelow50000(546509);
    id.SetAttackBuyBelow200000(456049);
    id.SetAttackBuyAbove200000(345608);
    id.SetAttackSellBelow50000(458696);
    id.SetAttackSellBelow200000(566457);
    id.SetAttackSellAbove200000(87667);
    id.SetAttackBuyAmount(12364566);
    id.SetAttackBuyVolume(234263534);
    id.SetAttackSellAmount(43563454);
    id.SetAttackSellVolume(945689489);
    id.SetCancelBuyVolume(4356245);
    id.SetCancelSellVolume(456648698);
    id.SetUnknownVolume(48695747);
    id.SetStrongBuyVolume(4563456);
    id.SetStrongSellVolume(989676);

    setDayLineInfo.m_strFilter = _T("[Day] =");
    setDayLineInfo.m_strFilter += strDay;
    setDayLineInfo.Open();
    setDayLineInfo.m_pDatabase->BeginTrans();
    setDayLineInfo.AddNew();
    setDayLineInfo.m_Day = lDay;
    id.SaveTodayInfo(setDayLineInfo);
    setDayLineInfo.Update();
    setDayLineInfo.m_pDatabase->CommitTrans();
    setDayLineInfo.Close();

    setDayLineInfo.m_strFilter = _T("[Day] =");
    setDayLineInfo.m_strFilter += strDay;
    setDayLineInfo.Open();
    EXPECT_EQ(setDayLineInfo.m_Day, lDay);
    EXPECT_STREQ(setDayLineInfo.m_StockCode, id.GetStockCode());
    EXPECT_EQ(setDayLineInfo.m_Market, id.GetMarket());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackBuyAbove200000), id.GetAttackBuyAbove200000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackBuyBelow200000), id.GetAttackBuyBelow200000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackBuyBelow50000), id.GetAttackBuyBelow50000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackBuyVolume), id.GetAttackBuyVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackSellAbove200000), id.GetAttackSellAbove200000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackSellBelow200000), id.GetAttackSellBelow200000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackSellBelow50000), id.GetAttackSellBelow50000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackSellVolume), id.GetAttackSellVolume());

    EXPECT_EQ(atol(setDayLineInfo.m_OrdinaryBuyVolume), id.GetOrdinaryBuyVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_OrdinarySellVolume), id.GetOrdinarySellVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_CancelBuyVolume), id.GetCancelBuyVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_CancelSellVolume), id.GetCancelSellVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_StrongBuyVolume), id.GetStrongBuyVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_StrongSellVolume), id.GetStrongSellVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_UnknownVolume), id.GetUnknownVolume());

    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumber), id.GetTransactionNumber());
    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumberAbove200000), id.GetTransactionNumberAbove200000());
    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumberBelow200000), id.GetTransactionNumberBelow200000());
    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumberBelow50000), id.GetTransactionNumberBelow50000());
    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumberBelow5000), id.GetTransactionNumberBelow5000());
    setDayLineInfo.Close();
    setDayLineInfo.m_strFilter = _T("[Day] =");
    setDayLineInfo.m_strFilter += strDay;
    setDayLineInfo.Open();
    setDayLineInfo.m_pDatabase->BeginTrans();
    while (!setDayLineInfo.IsEOF()) {
      setDayLineInfo.Delete();
      setDayLineInfo.MoveNext();
    }
    setDayLineInfo.m_pDatabase->CommitTrans();
    setDayLineInfo.Close();
  }
}