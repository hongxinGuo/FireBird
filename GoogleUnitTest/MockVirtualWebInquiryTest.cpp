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
			.WillOnce(Return(1024)) //第一次返回值为0
			.WillOnce(Return(1024))
			.WillOnce(Return(1024)) //第三次返回值为非零
			.WillOnce(Return(1024))
			.WillOnce(Return(10))
			.WillOnce(Return(0)); // 随后为零，函数顺利返回
		m_VirtualWebInquiry.SetReadingWebData(true);
		m_VirtualWebInquiry.SetInquiringString(strInquiry);
		EXPECT_TRUE(m_VirtualWebInquiry.ReadingWebData());
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
		EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << "直到工作线程退出时方重置此标识";
	}

	TEST_F(CMockVirtualWebInquiryTest, TestReadWebData2) {
		CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(m_VirtualWebInquiry, OpenFile(strInquiry))
			.Times(1)
			.WillOnce(Return(false));
		EXPECT_CALL(m_VirtualWebInquiry, ReadWebFileOneTime())
			.Times(0); // 当打开网络文件失败时，不去读取该文件
		m_VirtualWebInquiry.SetReadingWebData(true);
		m_VirtualWebInquiry.SetInquiringString(strInquiry);
		EXPECT_FALSE(m_VirtualWebInquiry.ReadingWebData()) << "打开网络文件失败时，函数报错";
		EXPECT_FALSE(m_VirtualWebInquiry.IsWebError()) << "Mock函数并没有设置此标识，真实情况下是设置了的";
		EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << "直到工作线程退出时方重置此标识";
	}

	TEST_F(CMockVirtualWebInquiryTest, TestGetWebData) {
		m_VirtualWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_VirtualWebInquiry.GetWebData()) << _T("目前只测试当正在读取网络数据的情况.此基类不允许调用GetWebData()函数");

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
		EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
	}

	TEST_F(CMockVirtualWebInquiryTest, TestStartReadingThread) {
		EXPECT_CALL(m_VirtualWebInquiry, StartReadingThread())
			.Times(1);
		m_VirtualWebInquiry.StartReadingThread();
	}
}
