#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"TemplateMutexAccessQueue.h"
#include"NeteaseDayLineWebData.h"
#include"WebData.h"

using namespace std;
#include<string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	template <typename T>class CTemplateMutexAccessQueueTest : public ::testing::Test
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			m_qData.Reset();
			GeneralCheck();
		}
		T m_data1;
		T m_data2;
		CTemplateMutexAccessQueue<T> m_qData;
	};

	TYPED_TEST_SUITE_P(CTemplateMutexAccessQueueTest);

	TYPED_TEST_P(CTemplateMutexAccessQueueTest, TestReset) {
		m_qData.PushData(m_data1);
		m_qData.PushData(m_data2);
		EXPECT_EQ(m_qData.GetDataSize(), 2);
		m_qData.Reset();
		EXPECT_EQ(m_qData.GetDataSize(), 0);
	}

	TYPED_TEST_P(CTemplateMutexAccessQueueTest, TestPushPopData) {
		m_qData.PushData(m_data1);
		m_qData.PushData(m_data2);
		TypeParam m_data3 = m_qData.PopData();
		EXPECT_EQ(m_data3, m_data1) << "无优先权的队列，与交易时间无关，按进队列的先后顺序出队列";
	}

	REGISTER_TYPED_TEST_SUITE_P(CTemplateMutexAccessQueueTest, TestReset, TestPushPopData);

	using MyTypes = ::testing::Types<shared_ptr<string>, CWebDataPtr, CNeteaseDayLineWebDataPtr, int>;
	INSTANTIATE_TYPED_TEST_SUITE_P(My, CTemplateMutexAccessQueueTest, MyTypes);
}