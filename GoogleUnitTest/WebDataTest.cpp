#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"WebData.h"

namespace StockAnalysisTest {
  class CWebDataTest : public ::testing::Test {
    virtual void SetUp(void) override {
      EXPECT_EQ(m_id.GetCurrentPos(), 0);
      EXPECT_EQ(m_id.GetTime(), 0);
      EXPECT_EQ(m_id.GetBufferLength(), 2048 * 4096);
      m_id.ResetCurrentPos();
      EXPECT_EQ(m_id.GetCurrentPos(), 0);
    }

    virtual void TearDown(void) override {
      m_id.SetBufferLength(2048 * 4096);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }

  public:
    CWebData m_id;
  };

  TEST_F(CWebDataTest, TestGetCurrentPos) {
    EXPECT_EQ(m_id.GetCurrentPos(), 0);
    m_id.IncreaseCurrentPos(5);
    EXPECT_EQ(m_id.GetCurrentPos(), 5);
  }

  TEST_F(CWebDataTest, TestIncreaseCurrentPos) {
    m_id.IncreaseCurrentPos();
    EXPECT_EQ(m_id.GetCurrentPos(), 1);
    m_id.IncreaseCurrentPos(10);
    EXPECT_EQ(m_id.GetCurrentPos(), 11);
  }

  TEST_F(CWebDataTest, TestGetData1) {
    m_id.SetData(0, 'a');
    EXPECT_EQ(m_id.GetData(0), 'a');
    m_id.SetData(10, 'a');
    EXPECT_EQ(m_id.GetData(10), 'a');
  }

  TEST_F(CWebDataTest, TestGetData2) {
    CString strTest = "abcdefg";
    EXPECT_TRUE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength(), 0));
    char buffer[8];
    EXPECT_TRUE(m_id.GetData(buffer, 7, 0));
    buffer[7] = 0x000;
    CString strTest2 = buffer;
    EXPECT_STREQ(strTest, strTest2);

    EXPECT_TRUE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength(), 100));
    EXPECT_TRUE(m_id.GetData(buffer, 7, 100));
    buffer[7] = 0x000;
    strTest2 = buffer;
    EXPECT_STREQ(strTest, strTest2);

    m_id.SetBufferLength(6);
    EXPECT_FALSE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength(), 0)) << "数据容器长度为6个字节，但需要存储七个字节的数据，导致函数报错";
  }

  TEST_F(CWebDataTest, TestGetData3) {
    CString strTest = "abcdefg";
    m_id.SetCurrentPos(0);
    EXPECT_TRUE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength()));
    char buffer[8];
    EXPECT_TRUE(m_id.GetData(buffer, 7));
    buffer[7] = 0x000;
    CString strTest2 = buffer;
    EXPECT_STREQ(strTest, strTest2);

    m_id.SetCurrentPos(1000);
    EXPECT_TRUE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength()));
    EXPECT_TRUE(m_id.GetData(buffer, 7));
    buffer[7] = 0x000;
    strTest2 = buffer;
    EXPECT_STREQ(strTest, strTest2);

    m_id.SetBufferLength(6);
    m_id.SetCurrentPos(0);
    EXPECT_FALSE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength())) << "数据容器长度为6个字节，但需要存储七个字节的数据，导致函数报错";
  }

  TEST_F(CWebDataTest, TestGetCurrentPosData) {
    m_id.SetCurrentPos(1);
    m_id.SetData(1, 'a');
    EXPECT_EQ(m_id.GetCurrentPosData(), 'a');
    m_id.SetCurrentPos(2);
    m_id.SetCurrentPosData('b');
    EXPECT_EQ(m_id.GetCurrentPosData(), 'b');
  }
}