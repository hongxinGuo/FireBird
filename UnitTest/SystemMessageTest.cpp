//////////////////////////////////////////////////////////////////////////////////////////
//
// ϵͳ��Ϣ�еĸ��������Բ��õ�ͬ�������Է�ֹ�߳�֮��������ݲ�ͬ�����⣬�����µĲ��Ժ���û�в�������ͬ�����ƣ�
// ֻ�Ǽ򵥵Ĳ������ڲ��߼���
//
//
///////////////////////////////////////////////////////////////////////////////////////////

#include"stdafx.h"
#include"pch.h"

#include"SystemMessage.h"

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