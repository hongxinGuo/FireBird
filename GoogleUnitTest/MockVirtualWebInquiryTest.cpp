#include"pch.h"

#include"FinnhubDataSource.h"
#include"MockVirtualWebInquiry.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	class CMockVirtualWebInquiryTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			m_VirtualWebInquiry.SetDataSource(gl_pFinnhubDataSource.get());
			EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();

			m_VirtualWebInquiry.SetInquiringString(_T(""));
			m_VirtualWebInquiry.SetReadingWebData(false);
		}

	public:
		CMockVirtualWebInquiry m_VirtualWebInquiry;
	};

	TEST_F(CMockVirtualWebInquiryTest, TestReadWebData) {
		CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(m_VirtualWebInquiry, OpenFile(strInquiry))
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(m_VirtualWebInquiry, ReadWebFileOneTime())
			.Times(6)
			.WillOnce(Return(1024)) //��һ�η���ֵΪ0
			.WillOnce(Return(1024))
			.WillOnce(Return(1024)) //�����η���ֵΪ����
			.WillOnce(Return(1024))
			.WillOnce(Return(10))
			.WillOnce(Return(0)); // ���Ϊ�㣬����˳������
		m_VirtualWebInquiry.SetReadingWebData(true);
		m_VirtualWebInquiry.SetInquiringString(strInquiry);
		EXPECT_TRUE(m_VirtualWebInquiry.ReadingWebData());
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
		EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
	}

	TEST_F(CMockVirtualWebInquiryTest, TestReadWebData2) {
		CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(m_VirtualWebInquiry, OpenFile(strInquiry))
			.Times(1)
			.WillOnce(Return(false));
		EXPECT_CALL(m_VirtualWebInquiry, ReadWebFileOneTime())
			.Times(0); // ���������ļ�ʧ��ʱ����ȥ��ȡ���ļ�
		m_VirtualWebInquiry.SetReadingWebData(true);
		m_VirtualWebInquiry.SetInquiringString(strInquiry);
		EXPECT_FALSE(m_VirtualWebInquiry.ReadingWebData()) << "�������ļ�ʧ��ʱ����������";
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError()) << "Mock������û�����ô˱�ʶ����ʵ������������˵�";
		EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
	}

	TEST_F(CMockVirtualWebInquiryTest, TestGetWebData) {
		m_VirtualWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_VirtualWebInquiry.GetWebData()) << _T("Ŀǰֻ���Ե����ڶ�ȡ�������ݵ����.�˻��಻�������GetWebData()����");

		m_VirtualWebInquiry.SetReadingWebData(false);
		EXPECT_CALL(m_VirtualWebInquiry, PrepareNextInquiringString())
			.WillOnce(Return(false));
		EXPECT_CALL(m_VirtualWebInquiry, StartReadingThread())
			.Times(0);
		EXPECT_FALSE(m_VirtualWebInquiry.GetWebData());

		m_VirtualWebInquiry.SetReadingWebData(false);
		EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
		EXPECT_CALL(m_VirtualWebInquiry, PrepareNextInquiringString())
			.WillOnce(Return(true));
		EXPECT_CALL(m_VirtualWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(m_VirtualWebInquiry.GetWebData());
		EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << _T("Ԥ�����õĴ˱�ʶ������Mock��û������֮���ʶ�������������״̬\n");
	}

	TEST_F(CMockVirtualWebInquiryTest, TestStartReadingThread) {
		EXPECT_CALL(m_VirtualWebInquiry, StartReadingThread())
			.Times(1);
		m_VirtualWebInquiry.StartReadingThread();
	}
}
