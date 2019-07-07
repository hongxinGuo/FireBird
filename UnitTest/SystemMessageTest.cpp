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
}