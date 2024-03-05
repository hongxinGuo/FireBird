#include"pch.h"

#include"TemplateMutexAccessQueueTest.h"

#include"GeneralCheck.h"
#include"DayLineWebData.h"
#include"WebData.h"

using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	// bug 当使用TYPED_P模式时，Soltenborn的googleTest Framework无法在命名空间FireBirdTest中找到源代码，估计是该框架需要更新的缘故。
	// 使用Resharper的googleTest框架能够正确找到
	//GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(CTemplateMutexAccessQueueTest);
	using MyTypes = ::testing::Types<string, CWebData, CDayLineWebData, int>;
	INSTANTIATE_TYPED_TEST_SUITE_P(TemplateTest, CTemplateMutexAccessQueueTest, MyTypes);
	//INSTANTIATE_TYPED_TEST_SUITE_P(TemplateTest, CTemplateMutexAccessQueueTest, int);

	// 下面是使用TYPED模式，两个框架都能找到。
	template <typename T>
	class CMutexAccessQueueTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			m_data1 = make_shared<T>();
			m_data2 = make_shared<T>();
		}

		void TearDown() override {
			// clearUp
			m_qDataTest.Reset();
			SCOPED_TRACE("");
			GeneralCheck();
		}

		shared_ptr<T> m_data1;
		shared_ptr<T> m_data2;
		CTemplateMutexAccessQueue<T> m_qDataTest;
	};

	using MyTypes = ::testing::Types<string, CWebData, CDayLineWebData, int>;
	TYPED_TEST_SUITE(CMutexAccessQueueTest, MyTypes);

	TYPED_TEST(CMutexAccessQueueTest, TestReset) {
		this->m_qDataTest.PushData(this->m_data1);
		this->m_qDataTest.PushData(this->m_data2);
		EXPECT_EQ(this->m_qDataTest.Size(), 2);
		this->m_qDataTest.Reset();
		EXPECT_EQ(this->m_qDataTest.Size(), 0);
	}

	// bug 将TestPushPop改名为TestPushPopData时，Soltenborn框架在命名空间FireBirdTest中无法找到该测试函数。
	TYPED_TEST(CMutexAccessQueueTest, TestPushPop) {
		EXPECT_TRUE(this->m_qDataTest.Empty()) << "初始时队列是空的";
		this->m_qDataTest.PushData(this->m_data1);
		EXPECT_FALSE(this->m_qDataTest.Empty()) << "此时队列中有一个数据";
		EXPECT_EQ(this->m_qDataTest.Size(), 1);
		this->m_qDataTest.PushData(this->m_data2);
		EXPECT_FALSE(this->m_qDataTest.Empty()) << "此时队列中有两个数据";
		EXPECT_EQ(this->m_qDataTest.Size(), 2);
		EXPECT_EQ(this->m_qDataTest.PopData(), this->m_data1) << "无优先权的队列，与交易时间无关，按进队列的先后顺序出队列";

		EXPECT_EQ(this->m_qDataTest.Size(), 1);
		EXPECT_FALSE(this->m_qDataTest.Empty()) << "此时队列中有一个数据";
		this->m_qDataTest.PopData();
		EXPECT_TRUE(this->m_qDataTest.Empty()) << "此时队列是空的";
	}
}
