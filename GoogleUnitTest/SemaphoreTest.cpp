#include"pch.h"

#include"Semaphore.h"

using namespace MyLib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class SemaphoreTest : public ::testing::Test {
	protected:
		virtual void SetUp(void) override {
		}
		virtual void TearDown(void) override {
		}
		Semaphore m_TestSemaphore;
	};

	TEST_F(SemaphoreTest, TestSetMaxCount) {
		EXPECT_EQ(m_TestSemaphore.GetMaxCount(), 1);
		m_TestSemaphore.SetMaxCount(5);
		EXPECT_EQ(m_TestSemaphore.GetMaxCount(), 5);
		m_TestSemaphore.SetMaxCount();
		EXPECT_EQ(m_TestSemaphore.GetMaxCount(), 1);
	}
}