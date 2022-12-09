#include"pch.h"

#include"GeneralCheck.h"

#include"TemplateMutexAccessQueue.h"

#include"NeteaseDayLineWebData.h"
#include"WebData.h"


#include<string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	template <typename T>
	class CTemplateMutexAccessQueueTest : public ::testing::Test {
	protected:
		virtual void SetUp(void) override {
			m_data1 = make_shared<T>();
			m_data2 = make_shared<T>();
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			m_qDataTest.Reset();
			GeneralCheck();
		}
		shared_ptr<T> m_data1;
		shared_ptr<T> m_data2;
		CTemplateMutexAccessQueue<T> m_qDataTest;
	};

	TYPED_TEST_SUITE_P(CTemplateMutexAccessQueueTest);

	TYPED_TEST_P(CTemplateMutexAccessQueueTest, TestReset) {
		this->m_qDataTest.PushData(this->m_data1);
		this->m_qDataTest.PushData(this->m_data2);
		EXPECT_EQ(this->m_qDataTest.Size(), 2);
		this->m_qDataTest.Reset();
		EXPECT_EQ(this->m_qDataTest.Size(), 0);
	}

	TYPED_TEST_P(CTemplateMutexAccessQueueTest, TestPushPopData) {
		this->m_qDataTest.PushData(this->m_data1);
		this->m_qDataTest.PushData(this->m_data2);
		EXPECT_EQ(this->m_qDataTest.PopData(), this->m_data1) << "无优先权的队列，与交易时间无关，按进队列的先后顺序出队列";
	}

	REGISTER_TYPED_TEST_SUITE_P(CTemplateMutexAccessQueueTest, TestReset, TestPushPopData);

	using MyTypes = ::testing::Types<string, CWebData, CNeteaseDayLineWebData, int>;
	INSTANTIATE_TYPED_TEST_SUITE_P(MyTestTemplateMutexAccessQueue, CTemplateMutexAccessQueueTest, MyTypes);
}