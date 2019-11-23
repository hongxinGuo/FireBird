#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ThreadStatus.h"

namespace StockAnalysisTest {
  TEST(ThreadStatusTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThread());
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingStockCodeData());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());

    long l = gl_systemMessage.GetInformationDequeSize();
    CThreadStatus threadStatus; // ���ɵڶ���ʵ������һ��Ϊȫ�ֱ�����ϵͳ����ʱ�������ˣ�
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // ϵͳ��������
    for (int i = 0; i < l + 1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // �����Ϣ����
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  TEST(ThreadStatusTest, TestIsExitingThread) {
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThread());
    gl_ThreadStatus.SetExitingThread(true);
    EXPECT_TRUE(gl_ThreadStatus.IsExitingThread());
    gl_ThreadStatus.SetExitingThread(false);
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThread());
  }

  TEST(ThereadStatusTest, TestIsCalculatingDayLineRS) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingDayLineRS());
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
  }

  TEST(ThreadStatusTest, TestIsRTDataNeedCalculate) {
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    gl_ThreadStatus.SetRTDataNeedCalculate(true);
    EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
  }

  TEST(ThreadStatusTest, TestIsCalculatingRTData) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
    gl_ThreadStatus.SetCalculatingRTData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRTData());
    gl_ThreadStatus.SetCalculatingRTData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
  }

  TEST(ThreadStatusTest, TestIsSavingTempData) {
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
  }

  TEST(ThreadStatusTest, TestIsSavingStockCodeData) {
    EXPECT_FALSE(gl_ThreadStatus.IsSavingStockCodeData());
    gl_ThreadStatus.SetSavingStockCodeData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingStockCodeData());
    gl_ThreadStatus.SetSavingStockCodeData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingStockCodeData());
  }

  TEST(ThreadStatusTest, TestIsCalculatingRS) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
    for (int i = 0; i < gl_cMaxCalculatingRSThreads; i++) {  // Ŀǰ�������8���߳�
      gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();
    }
    for (int i = 0; i < gl_cMaxCalculatingRSThreads - 1; i++) {
      gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRS());
    }
    gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
  }

  TEST(ThreadStatusTest, TestIsSavingDayLine) {
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
    for (int i = 0; i < gl_cMaxSavingOneDayLineThreads; i++) {  // Ŀǰ�������3���߳�
      gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
    }
    for (int i = 0; i < gl_cMaxSavingOneDayLineThreads - 1; i++) {
      gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsSavingDayLine());
    }
    gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
  }
}