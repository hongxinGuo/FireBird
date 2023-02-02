#include"pch.h"

#include"MockVirtualDataSource.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	class CMockVirtualDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			EXPECT_FALSE(m_VirtualDataSource.IsInquiringWebData());
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();

			m_VirtualDataSource.SetInquiringString(_T(""));
			m_VirtualDataSource.SetInquiringWebData(false);
		}

	public:
		CMockVirtualDataSource m_VirtualDataSource;
	};

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData) {
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_VirtualDataSource.SetByteRead(0);
		InSequence seq;
		EXPECT_CALL(m_VirtualDataSource, OpenFile(strInquiry))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(m_VirtualDataSource, ReadWebFileOneTime())
			.Times(6)
			.WillOnce(Return(1024)) //��һ�η���ֵΪ0
			.WillOnce(Return(1024))
			.WillOnce(Return(1024)) //�����η���ֵΪ����
			.WillOnce(Return(1024))
			.WillOnce(Return(10))
			.WillOnce(Return(0)); // ���Ϊ�㣬����˳������
		m_VirtualDataSource.SetInquiringWebData(true);
		m_VirtualDataSource.SetInquiringString(strInquiry);

		EXPECT_TRUE(m_VirtualDataSource.ReadingWebData());

		EXPECT_EQ(m_VirtualDataSource.GetByteRead(), 1024 * 4 + 10) << "�⼸�η���ֵ֮��";
		EXPECT_FALSE(m_VirtualDataSource.IsWebError());
		EXPECT_TRUE(m_VirtualDataSource.IsInquiringWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData2) {
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(m_VirtualDataSource, OpenFile(strInquiry))
			.Times(1)
			.WillOnce(Return(false));
		EXPECT_CALL(m_VirtualDataSource, ReadWebFileOneTime())
			.Times(0); // ���������ļ�ʧ��ʱ����ȥ��ȡ���ļ�
		m_VirtualDataSource.SetInquiringWebData(true);
		m_VirtualDataSource.SetInquiringString(strInquiry);

		EXPECT_FALSE(m_VirtualDataSource.ReadingWebData()) << "�������ļ�ʧ��ʱ����������";

		EXPECT_FALSE(m_VirtualDataSource.IsWebError()) << "Mock������û�����ô˱�ʶ����ʵ������������˵�";
		EXPECT_TRUE(m_VirtualDataSource.IsInquiringWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
	}

	TEST_F(CMockVirtualDataSourceTest, TestGetWebData) {
		m_VirtualDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_VirtualDataSource.GetWebData()) << _T("Ŀǰֻ���Ե����ڶ�ȡ�������ݵ����.�˻��಻�������GetWebData()����");

		m_VirtualDataSource.SetInquiringWebData(false);
		EXPECT_CALL(m_VirtualDataSource, PrepareNextInquiringString())
			.WillOnce(Return(false));
		EXPECT_CALL(m_VirtualDataSource, StartReadingThread())
			.Times(0);
		EXPECT_FALSE(m_VirtualDataSource.GetWebData());

		m_VirtualDataSource.SetInquiringWebData(false);
		EXPECT_FALSE(m_VirtualDataSource.IsInquiringWebData());
		EXPECT_CALL(m_VirtualDataSource, PrepareNextInquiringString())
			.WillOnce(Return(true));
		EXPECT_CALL(m_VirtualDataSource, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_VirtualDataSource.GetWebData());
		EXPECT_TRUE(m_VirtualDataSource.IsInquiringWebData()) << _T("Ԥ�����õĴ˱�ʶ������Mock��û������֮���ʶ�������������״̬\n");
	}

	TEST_F(CMockVirtualDataSourceTest, TestStartReadingThread) {
		EXPECT_CALL(m_VirtualDataSource, StartReadingThread())
			.Times(1);
		m_VirtualDataSource.StartReadingThread();
	}
}
