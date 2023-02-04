#include"pch.h"

#include"afxinet.h"

#include"MockVirtualDataSource.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	static CMockVirtualDataSourcePtr m_pVirtualDataSource; // 为了方便使用googleMock,将mock变量声明为全局的。

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

			m_pVirtualDataSource = make_shared<CMockVirtualDataSource>();
			EXPECT_FALSE(m_pVirtualDataSource->IsInquiringWebData());
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();

			m_pVirtualDataSource->SetInquiringString(_T(""));
			m_pVirtualDataSource->SetInquiringWebData(false);
			m_pVirtualDataSource = nullptr;
		}

	public:
	};

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData) {
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_pVirtualDataSource->SetByteRead(0);
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry))
		.Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(6)
		.WillOnce(Return(1024)) //第一次返回值为0
		.WillOnce(Return(1024))
		.WillOnce(Return(1024)) //第三次返回值为非零
		.WillOnce(Return(1024))
		.WillOnce(Return(10))
		.WillOnce(Return(0)); // 随后为零，函数顺利返回
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		EXPECT_TRUE(m_pVirtualDataSource->ReadingWebData());

		EXPECT_EQ(m_pVirtualDataSource->GetByteRead(), 1024 * 4 + 10) << "这几次返回值之和";
		EXPECT_FALSE(m_pVirtualDataSource->IsWebError());
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "直到工作线程退出时方重置此标识";
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData2) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1)
		.WillOnce(Throw(exception)); // 打开网络文件时出错。
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(0);
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		EXPECT_FALSE(m_pVirtualDataSource->ReadingWebData()) << "打开网络文件失败时，函数报错";

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError()) << "真实情况下是设置了的，故而Mock函数也仿真设置之";
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "直到工作线程退出时方重置此标识";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData3) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(1)
		.WillOnce(Throw(exception)); // 读取网络文件时出错。
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		EXPECT_FALSE(m_pVirtualDataSource->ReadingWebData()) << "打开网络文件失败时，函数报错";

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError()) << "真实情况下是设置了的，故而Mock函数也仿真设置之";
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "直到工作线程退出时方重置此标识";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData4) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(2)
		.WillOnce(Return(1024))
		.WillOnce(Throw(exception)); // 第二次读取网络文件时出错。
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		EXPECT_FALSE(m_pVirtualDataSource->ReadingWebData()) << "打开网络文件失败时，函数报错";

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError()) << "真实情况下是设置了的，故而Mock函数也仿真设置之";
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "直到工作线程退出时方重置此标识";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestGetWebData) {
		m_pVirtualDataSource->SetInquiringWebData(true);
		EXPECT_FALSE(m_pVirtualDataSource->GetWebData()) << _T("目前只测试当正在读取网络数据的情况.此基类不允许调用GetWebData()函数");

		m_pVirtualDataSource->SetInquiringWebData(false);
		EXPECT_CALL(*m_pVirtualDataSource, PrepareNextInquiringString())
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pVirtualDataSource, StartReadingThread()).Times(0);
		EXPECT_FALSE(m_pVirtualDataSource->GetWebData());

		m_pVirtualDataSource->SetInquiringWebData(false);
		EXPECT_FALSE(m_pVirtualDataSource->IsInquiringWebData());
		EXPECT_CALL(*m_pVirtualDataSource, PrepareNextInquiringString())
		.WillOnce(Return(true));
		EXPECT_CALL(*m_pVirtualDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pVirtualDataSource->GetWebData());
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
	}

	TEST_F(CMockVirtualDataSourceTest, TestStartReadingThread) {
		EXPECT_CALL(*m_pVirtualDataSource, StartReadingThread()).Times(1);
		m_pVirtualDataSource->StartReadingThread();
	}
}
