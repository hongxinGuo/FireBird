#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ThreadStatus.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(SystemStatusTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThread());
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingStockCodeData());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());

    long l = gl_systemMessage.GetInformationDequeSize();
    CThreadStatus threadStatus; // ���ɵڶ���ʵ������һ��Ϊȫ�ֱ�����ϵͳ����ʱ�������ˣ�
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // ϵͳ��������
    for (int i = 0; i < l + 1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // �����Ϣ����
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  TEST(SystemStatusTest, TestIsExitingThread) {
    gl_ThreadStatus.SetExitingThread(true);
    EXPECT_TRUE(gl_ThreadStatus.IsExitingThread());
    gl_ThreadStatus.SetExitingThread(false);
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThread());
  }

  TEST(SystemStatusTest, TestIsDataBase) {
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
    for (int i = 0; i < gl_cMaxSavingDayLineThreads; i++) {  // Ŀǰ�������8���߳�
      EXPECT_TRUE(gl_ThreadStatus.IsSavingDayLineThreadAvailable());
      gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
    }
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLineThreadAvailable());
    for (int i = 0; i < gl_cMaxSavingDayLineThreads - 1; i++) {
      gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsSavingDayLineThreadAvailable());
      EXPECT_TRUE(gl_ThreadStatus.IsSavingDayLine());
    }
    gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
  }

  TEST(SystemStatusTest, TestIsRTDataNeedCalculate) {
    gl_ThreadStatus.SetRTDataNeedCalculate(true);
    EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
  }

  TEST(SystemStatusTest, TestIsCalculatingRTData) {
    gl_ThreadStatus.SetCalculatingRTData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRTData());
    gl_ThreadStatus.SetCalculatingRTData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
  }

  TEST(SystemStatusTest, TestIsSavingTempData) {
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
  }

  TEST(SystemStatusTest, TestIsSavingStockCodeData) {
    gl_ThreadStatus.SetSavingStockCodeData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingStockCodeData());
    gl_ThreadStatus.SetSavingStockCodeData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingStockCodeData());
  }

  TEST(SystemStatusTest, TestIsCalculatingRS) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
    for (int i = 0; i < gl_cMaxCalculatingRSThreads; i++) {  // Ŀǰ�������8���߳�
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
      gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();
    }
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
    for (int i = 0; i < gl_cMaxCalculatingRSThreads - 1; i++) {
      gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRS());
    }
    gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
  }
}