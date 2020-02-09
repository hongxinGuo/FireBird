#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"WebData.h"

namespace StockAnalysisTest {
  class CReceivedDataTest : public ::testing::Test {
    virtual void SetUp(void) override {
      EXPECT_EQ(m_id.m_pDataBuffer, nullptr);
      EXPECT_EQ(m_id.m_pCurrentPos, nullptr);
      EXPECT_EQ(m_id.m_lCurrentPos, 0);
      EXPECT_EQ(m_id.m_lBufferLength, 0);
      EXPECT_EQ(m_id.m_lTime, 0);
      EXPECT_EQ(m_id.m_lBufferLength, 0);
      char* buffer = new char[100];
      m_id.m_pDataBuffer = buffer;
      m_id.SetBufferLength(100);
      m_id.ResetCurrentPos();
      EXPECT_EQ(m_id.m_pCurrentPos, buffer);
      EXPECT_EQ(m_id.m_lCurrentPos, 0);
      EXPECT_EQ(m_id.GetBufferAddr(), buffer);
      EXPECT_EQ(m_id.GetBufferLength(), 100);
    }

    virtual void TearDown(void) override {
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }

  public:
    CWebData m_id;
  };

  TEST_F(CReceivedDataTest, TestGetCurrentPos) {
    EXPECT_EQ(m_id.GetCurrentPos(), 0);
    m_id.SetCurrentPos(5);
    EXPECT_EQ(m_id.GetCurrentPos(), 5);
    EXPECT_EQ(m_id.m_pCurrentPos, m_id.m_pDataBuffer + 5);
  }

  TEST_F(CReceivedDataTest, TestIncreaseCurrentPos) {
    m_id.IncreaseCurrentPos();
    EXPECT_EQ(m_id.m_pCurrentPos, m_id.m_pDataBuffer + 1);
    EXPECT_EQ(m_id.m_lCurrentPos, 1);
    m_id.IncreaseCurrentPos(10);
    EXPECT_EQ(m_id.m_pCurrentPos, m_id.m_pDataBuffer + 11);
    EXPECT_EQ(m_id.m_lCurrentPos, 11);
  }
}