#include "pch.h"

#include"globedef.h"

#include"StockCalculatedInfo.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(StockCalculatedInfoTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    CStockCalculatedInfo stock;
  }

  TEST(StockBasicInfoTest, TestGetAttackBuyAmount) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyAmount(), 0);
    id.SetAttackBuyAmount(10101010);
    EXPECT_EQ(id.GetAttackBuyAmount(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetAttackSellAmount) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellAmount(), 0);
    id.SetAttackSellAmount(10101010);
    EXPECT_EQ(id.GetAttackSellAmount(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetOrdinaryBuyVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 0);
    id.SetOrdinaryBuyVolume(10101010);
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetOrdinarySellVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetOrdinarySellVolume(), 0);
    id.SetOrdinarySellVolume(10101010);
    EXPECT_EQ(id.GetOrdinarySellVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetAttackBuyVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    id.SetAttackBuyVolume(10101010);
    EXPECT_EQ(id.GetAttackBuyVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetStrongBuyVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetStrongBuyVolume(), 0);
    id.SetStrongBuyVolume(10101010);
    EXPECT_EQ(id.GetStrongBuyVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentAttackBuy) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentAttackBuy(), 0);
    id.SetCurrentAttackBuy(10101010);
    EXPECT_EQ(id.GetCurrentAttackBuy(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentStrongBuy) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentStrongBuy(), 0);
    id.SetCurrentStrongBuy(10101010);
    EXPECT_EQ(id.GetCurrentStrongBuy(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetAttackSellVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
    id.SetAttackSellVolume(10101010);
    EXPECT_EQ(id.GetAttackSellVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetStrongSellVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetStrongSellVolume(), 0);
    id.SetStrongSellVolume(10101010);
    EXPECT_EQ(id.GetStrongSellVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentAttackSell) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentAttackSell(), 0);
    id.SetCurrentAttackSell(10101010);
    EXPECT_EQ(id.GetCurrentAttackSell(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentStrongSell) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentStrongSell(), 0);
    id.SetCurrentStrongSell(10101010);
    EXPECT_EQ(id.GetCurrentStrongSell(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetUnknownVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetUnknownVolume(), 0);
    id.SetUnknownVolume(10101010);
    EXPECT_EQ(id.GetUnknownVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentUnknown) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCurrentUnknown(), 0);
    id.SetCurrentUnknown(10101010);
    EXPECT_EQ(id.GetCurrentUnknown(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCancelBuyVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCancelBuyVolume(), 0);
    id.SetCancelBuyVolume(10101010);
    EXPECT_EQ(id.GetCancelBuyVolume(), 10101010);
  }
  TEST(StockBasicInfoTest, TestGetCancelSellVolume) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetCancelSellVolume(), 0);
    id.SetCancelSellVolume(10101010);
    EXPECT_EQ(id.GetCancelSellVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGeTransactionNumber) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumber(), 0);
    id.SetTransactionNumber(10101010);
    EXPECT_EQ(id.GetTransactionNumber(), 10101010);
    id.IncreaseTransactionNumber(101);
    EXPECT_EQ(id.GetTransactionNumber(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestGetTransactionNumberBelow5000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), 0);
    id.SetTransactionNumberBelow5000(10101010);
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), 10101010);
    id.IncreaseTransactionNumberBelow5000(101);
    EXPECT_EQ(id.GetTransactionNumberBelow5000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestGetTransactionNumberBelow50000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 0);
    id.SetTransactionNumberBelow50000(10101010);
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 10101010);
    id.IncreaseTransactionNumberBelow50000(101);
    EXPECT_EQ(id.GetTransactionNumberBelow50000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestGetTransactionNumberBelow200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 0);
    id.SetTransactionNumberBelow200000(10101010);
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 10101010);
    id.IncreaseTransactionNumberBelow200000(101);
    EXPECT_EQ(id.GetTransactionNumberBelow200000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestGetTransactionNumberAbove200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 0);
    id.SetTransactionNumberAbove200000(10101010);
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 10101010);
    id.IncreaseTransactionNumberAbove200000(101);
    EXPECT_EQ(id.GetTransactionNumberAbove200000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestGetAttackBuyBelow50000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 0);
    id.SetAttackBuyBelow50000(10101010);
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10101010);
    id.IncreaseAttackBuyBelow50000(101);
    EXPECT_EQ(id.GetAttackBuyBelow50000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestGetAttackBuyBelow200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 0);
    id.SetAttackBuyBelow200000(10101010);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 10101010);
    id.IncreaseAttackBuyBelow200000(101);
    EXPECT_EQ(id.GetAttackBuyBelow200000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestGetAttackBuyAbove200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 0);
    id.SetAttackBuyAbove200000(10101010);
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 10101010);
    id.IncreaseAttackBuyAbove200000(101);
    EXPECT_EQ(id.GetAttackBuyAbove200000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestGetAttackSellBelow50000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellBelow50000(), 0);
    id.SetAttackSellBelow50000(10101010);
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10101010);
    id.IncreaseAttackSellBelow50000(101);
    EXPECT_EQ(id.GetAttackSellBelow50000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestAttackSellBelow200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellBelow200000(), 0);
    id.SetAttackSellBelow200000(10101010);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 10101010);
    id.IncreaseAttackSellBelow200000(101);
    EXPECT_EQ(id.GetAttackSellBelow200000(), 10101010 + 101);
  }

  TEST(StockBasicInfoTest, TestAttackSellAbove200000) {
    CStockCalculatedInfo id;
    EXPECT_EQ(id.GetAttackSellAbove200000(), 0);
    id.SetAttackSellAbove200000(10101010);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 10101010);
    id.IncreaseAttackSellAbove200000(101);
    EXPECT_EQ(id.GetAttackSellAbove200000(), 10101010 + 101);
  }
}