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
			.WillOnce(Return(1024)) //第一次返回值为0
			.WillOnce(Return(1024))
			.WillOnce(Return(1024)) //第三次返回值为非零
			.WillOnce(Return(1024))
			.WillOnce(Return(10))
			.WillOnce(Return(0)); // 随后为零，函数顺利返回
		m_VirtualDataSource.SetInquiringWebData(true);
		m_VirtualDataSource.SetInquiringString(strInquiry);

		EXPECT_TRUE(m_VirtualDataSource.ReadingWebData());

		EXPECT_EQ(m_VirtualDataSource.GetByteRead(), 1024 * 4 + 10) << "这几次返回值之和";
		EXPECT_FALSE(m_VirtualDataSource.IsWebError());
		EXPECT_TRUE(m_VirtualDataSource.IsInquiringWebData()) << "直到工作线程退出时方重置此标识";
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData2) {
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(m_VirtualDataSource, OpenFile(strInquiry))
			.Times(1)
			.WillOnce(Return(false));
		EXPECT_CALL(m_VirtualDataSource, ReadWebFileOneTime())
			.Times(0); // 当打开网络文件失败时，不去读取该文件
		m_VirtualDataSource.SetInquiringWebData(true);
		m_VirtualDataSource.SetInquiringString(strInquiry);

		EXPECT_FALSE(m_VirtualDataSource.ReadingWebData()) << "打开网络文件失败时，函数报错";

		EXPECT_FALSE(m_VirtualDataSource.IsWebError()) << "Mock函数并没有设置此标识，真实情况下是设置了的";
		EXPECT_TRUE(m_VirtualDataSource.IsInquiringWebData()) << "直到工作线程退出时方重置此标识";
	}

	TEST_F(CMockVirtualDataSourceTest, TestGetWebData) {
		m_VirtualDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_VirtualDataSource.GetWebData()) << _T("目前只测试当正在读取网络数据的情况.此基类不允许调用GetWebData()函数");

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
		EXPECT_TRUE(m_VirtualDataSource.IsInquiringWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
	}

	TEST_F(CMockVirtualDataSourceTest, TestStartReadingThread) {
		EXPECT_CALL(m_VirtualDataSource, StartReadingThread())
			.Times(1);
		m_VirtualDataSource.StartReadingThread();
	}
}
