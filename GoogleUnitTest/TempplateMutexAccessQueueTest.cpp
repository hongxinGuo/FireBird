#include"pch.h"

#include"GeneralCheck.h"

#include"TemplateMutexAccessQueue.h"

#include"DayLineWebData.h"
#include"WebData.h"

#include<memory>
using std::make_shared;

namespace FireBirdTest {
	template <typename T>
	class CTemplateMutexAccessQueueTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
			m_data1 = make_shared<T>();
			m_data2 = make_shared<T>();
		}

		void TearDown() override {
			// clearUp
			m_qDataTest.Reset();
			SCOPED_TRACE(""); GeneralCheck();
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
		EXPECT_TRUE(this->m_qDataTest.Empty()) << "��ʼʱ�����ǿյ�";
		this->m_qDataTest.PushData(this->m_data1);
		EXPECT_FALSE(this->m_qDataTest.Empty()) << "��ʱ��������һ������";
		EXPECT_EQ(this->m_qDataTest.Size(), 1);
		this->m_qDataTest.PushData(this->m_data2);
		EXPECT_FALSE(this->m_qDataTest.Empty()) << "��ʱ����������������";
		EXPECT_EQ(this->m_qDataTest.Size(), 2);
		EXPECT_EQ(this->m_qDataTest.PopData(), this->m_data1) << "������Ȩ�Ķ��У��뽻��ʱ���޹أ��������е��Ⱥ�˳�������";

		EXPECT_EQ(this->m_qDataTest.Size(), 1);
		EXPECT_FALSE(this->m_qDataTest.Empty()) << "��ʱ��������һ������";
		this->m_qDataTest.PopData();
		EXPECT_TRUE(this->m_qDataTest.Empty()) << "��ʱ�����ǿյ�";
	}

	REGISTER_TYPED_TEST_SUITE_P(CTemplateMutexAccessQueueTest, TestReset, TestPushPopData);

	using MyTypes = ::testing::Types<string, CWebData, CDayLineWebData, int>;
	INSTANTIATE_TYPED_TEST_SUITE_P(MyTestTemplateMutexAccessQueue, CTemplateMutexAccessQueueTest, MyTypes);
}
