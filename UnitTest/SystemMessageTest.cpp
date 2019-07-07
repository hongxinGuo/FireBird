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

namespace StockAnalysisTest {
  TEST(SystemMessageTest, TestGetOutputDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetOutputDequeSize(), 0);
    message.PushOutputMessage(_T("TEST"));
    EXPECT_EQ(message.GetOutputDequeSize(), 1);
    CString str = message.PopOutputMessage();
    EXPECT_EQ(message.GetOutputDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

  TEST(SystemMessageTest, TestGetDataBaseDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetDataBaseDequeSize(), 0);
    message.PushDataBaseMessage(_T("TEST"));
    EXPECT_EQ(message.GetDataBaseDequeSize(), 1);
    CString str = message.PopDataBaseMessage();
    EXPECT_EQ(message.GetDataBaseDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

  TEST(SystemMessageTest, TestGetFindDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetFindDequeSize(), 0);
    message.PushFindMessage(_T("TEST"));
    EXPECT_EQ(message.GetFindDequeSize(), 1);
    CString str = message.PopFindMessage();
    EXPECT_EQ(message.GetFindDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

  TEST(SystemMessageTest, TestGetWarningDequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetWarningDequeSize(), 0);
    message.PushWarningMessage(_T("TEST"));
    EXPECT_EQ(message.GetWarningDequeSize(), 1);
    CString str = message.PopWarningMessage();
    EXPECT_EQ(message.GetWarningDequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

  TEST(SystemMessageTest, TestGetTrace1DequeSize) {
    CSystemMessage message;
    EXPECT_EQ(message.GetTrace1DequeSize(), 0);
    message.PushTrace1Message(_T("TEST"));
    EXPECT_EQ(message.GetTrace1DequeSize(), 1);
    CString str = message.PopTrace1Message();
    EXPECT_EQ(message.GetTrace1DequeSize(), 0);
    EXPECT_STREQ(str, _T("TEST"));
  }

}