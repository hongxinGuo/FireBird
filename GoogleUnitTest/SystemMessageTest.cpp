//////////////////////////////////////////////////////////////////////////////////////////
//
// 系统信息中的各个函数皆采用的同步机制以防止线程之间出现数据不同步问题，但此下的测试函数没有测试这种同步机制，
// 只是简单的测试了内部逻辑。
//
//
///////////////////////////////////////////////////////////////////////////////////////////

#include"stdafx.h"
#include"pch.h"

#include"SystemMessage.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(SystemMessageTest, TestGetInformationDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetInformationDequeSize(), 0);
    message.PushInformationMessage(_T("TEST"));
    EXPECT_EQ(message.GetInformationDequeSize(), 1);
    CString str = message.PopInformationMessage();
    EXPECT_EQ(message.GetInformationDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

  TEST(SystemMessageTest, TestGetDayLineInfoDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetDayLineInfoDequeSize(), 0);
    message.PushDayLineInfoMessage(_T("TEST"));
    EXPECT_EQ(message.GetDayLineInfoDequeSize(), 1);
    CString str = message.PopDayLineInfoMessage();
    EXPECT_EQ(message.GetDayLineInfoDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

  TEST(SystemMessageTest, TestGetFindDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetTransactionDequeSize(), 0);
    message.PushTransactionMessage(_T("TEST"));
    EXPECT_EQ(message.GetTransactionDequeSize(), 1);
    CString str = message.PopTransactionMessage();
    EXPECT_EQ(message.GetTransactionDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

  TEST(SystemMessageTest, TestGetCancelSellDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetCancelSellDequeSize(), 0);
    message.PushCancelSellMessage(_T("TEST"));
    EXPECT_EQ(message.GetCancelSellDequeSize(), 1);
    CString str = message.PopCancelSellMessage();
    EXPECT_EQ(message.GetCancelSellDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

  TEST(SystemMessageTest, TestGetCancelBuyDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetCancelBuyDequeSize(), 0);
    message.PushCancelBuyMessage(_T("TEST"));
    EXPECT_EQ(message.GetCancelBuyDequeSize(), 1);
    CString str = message.PopCancelBuyMessage();
    EXPECT_EQ(message.GetCancelBuyDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

}