#include"pch.h"

#include"TemplateMutexAccessQueueTest.h"

#include"GeneralCheck.h"
#include"DayLineWebData.h"
#include"WebData.h"

using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	// bug ��ʹ��TYPED_Pģʽʱ��Soltenborn��googleTest Framework�޷��������ռ�FireBirdTest���ҵ�Դ���룬�����Ǹÿ����Ҫ���µ�Ե�ʡ�
	// ʹ��Resharper��googleTest����ܹ���ȷ�ҵ�
	//GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(CTemplateMutexAccessQueueTest);
	using MyTypes = ::testing::Types<string, CWebData, CDayLineWebData, int>;
	INSTANTIATE_TYPED_TEST_SUITE_P(TemplateTest, CTemplateMutexAccessQueueTest, MyTypes);
	//INSTANTIATE_TYPED_TEST_SUITE_P(TemplateTest, CTemplateMutexAccessQueueTest, int);

	// ������ʹ��TYPEDģʽ��������ܶ����ҵ���
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

	// bug ��TestPushPop����ΪTestPushPopDataʱ��Soltenborn����������ռ�FireBirdTest���޷��ҵ��ò��Ժ�����
	TYPED_TEST(CMutexAccessQueueTest, TestPushPop) {
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
}
