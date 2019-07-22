#include"stdafx.h"

#include "pch.h"
#include "CppUnitTest.h"

#include"SystemStatus.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockAnalysisUnitTest
{
  TEST_CLASS(SystemTimeTest) {
public:
  CSystemStatus status;

  TEST_METHOD(TestIsCalculateRSInProcess) {
    status.SetCalculateRSInProcess(true);
    Assert::IsTrue(status.IsCalculateRSInProcess());
    status.SetCalculateRSInProcess(false);
    Assert::IsFalse(status.IsCalculateRSInProcess());
  }

  TEST_METHOD(TestReadingInProcess) {
    status.SetReadingInProcess(true);
    Assert::IsTrue(status.IsReadingInProcess());
    status.SetReadingInProcess(false);
    Assert::IsFalse(status.IsReadingInProcess());
  }

  TEST_METHOD(TestIsDataBaseInProcess) {
    status.SetDataBaseInProcess(true);
    Assert::IsTrue(status.IsDataBaseInProcess());
    status.SetDataBaseInProcess(false);
    Assert::IsFalse(status.IsDataBaseInProcess());
  }

  TEST_METHOD(TestIsRTDataReadInProcess) {
    status.SetRTDataReadingInProcess(true);
    Assert::IsTrue(status.IsRTDataReadingInProcess());
    status.SetRTDataReadingInProcess(false);
    Assert::IsFalse(status.IsRTDataReadingInProcess());
  }

  TEST_METHOD(TestIsRTDataReceived) {
    status.SetRTDataReceived(true);
    Assert::IsTrue(status.IsRTDataReceived());
    status.SetRTDataReceived(false);
    Assert::IsFalse(status.IsRTDataReceived());
  }

  TEST_METHOD(TestIsRTDataNeedCalculate) {
    status.SetRTDataNeedCalculate(true);
    Assert::IsTrue(status.IsRTDataNeedCalculate());
    status.SetRTDataNeedCalculate(false);
    Assert::IsFalse(status.IsRTDataNeedCalculate());
  }

  TEST_METHOD(TestIsDayLineDataReady) {
    status.SetDayLineDataReady(true);
    Assert::IsTrue(status.IsDayLineDataReady());
    status.SetDayLineDataReady(false);
    Assert::IsFalse(status.IsDayLineDataReady());
  }

  };

}