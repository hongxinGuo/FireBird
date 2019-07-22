#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StockAnalysisUnitTest
{
	TEST_CLASS(SystemTimeTest)
	{
	public:
    CSystemTime systemTime;

		TEST_METHOD(TestGett_time) {
      Assert::AreEqual(systemTime.Gett_time(), 0);
		}
	};
}
