#include"pch.h"

#include"afxinet.h"

#include"MockVirtualDataSource.h"

#include"GeneralCheck.h"
#include "ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	static CMockVirtualDataSourcePtr m_pVirtualDataSource; // 为了方便查找出错位置,将mock变量声明为全局的。
	static CVirtualProductWebDataPtr m_pProduct;

	class CMockVirtualDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			m_pVirtualDataSource = nullptr;
		}

		static void TearDownTestSuite() {
			m_pVirtualDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			m_pVirtualDataSource = make_shared<CMockVirtualDataSource>();
			m_pProduct = make_shared<CProductDummy>();
			m_pVirtualDataSource->SetCurrentInquiry(m_pProduct);
			m_pVirtualDataSource->SetContentLength(128 * 1024);
		}

		void TearDown() override {
			// clearUp

			m_pVirtualDataSource->SetInquiringString(_T(""));
			m_pVirtualDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockVirtualDataSourceTest, TestReadWebDataAndProcessIt1) {
		m_pVirtualDataSource->SetWorkingThreadRunning(true);
		EXPECT_CALL(*m_pVirtualDataSource, GetWebData).Times(1)
		.WillOnce(Invoke([] { m_pVirtualDataSource->SetErrorCode(12002); }));
		EXPECT_CALL(*m_pVirtualDataSource, ProcessWebDataReceived).Times(0);

		m_pVirtualDataSource->GetWebDataAndProcessIt();

		// 恢复原状
		m_pVirtualDataSource->SetWorkingThreadRunning(false);
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebDataAndProcessIt2) {
		m_pVirtualDataSource->SetWorkingThreadRunning(true);
		EXPECT_CALL(*m_pVirtualDataSource, GetWebData).Times(1)
		.WillOnce(Invoke([] { m_pVirtualDataSource->SetErrorCode(0); }));
		EXPECT_CALL(*m_pVirtualDataSource, ProcessWebDataReceived).Times(1);

		m_pVirtualDataSource->GetWebDataAndProcessIt();

		// 恢复原状
		m_pVirtualDataSource->SetWorkingThreadRunning(false);
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebDataAndProcessIt3) {
		m_pVirtualDataSource->SetWorkingThreadRunning(true);
		EXPECT_CALL(*m_pVirtualDataSource, GetWebData).Times(1)
		.WillOnce(Invoke([] { m_pVirtualDataSource->SetErrorCode(0); }));
		EXPECT_CALL(*m_pVirtualDataSource, ProcessWebDataReceived).Times(1);

		m_pVirtualDataSource->GetWebDataAndProcessIt();

		// 恢复原状
		m_pVirtualDataSource->SetWorkingThreadRunning(false);
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData1) {
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_pVirtualDataSource->SetInquiring(true);
		m_pVirtualDataSource->SetErrorCode(12002);
		m_pVirtualDataSource->SetByteRead(0);
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(6)
		.WillOnce(Return(1024)) //第一次返回值为0
		.WillOnce(Return(1024))
		.WillOnce(Return(1024)) //第三次返回值为非零
		.WillOnce(Return(1024))
		.WillOnce(Return(10))
		.WillOnce(Return(0)); // 随后为零，函数顺利返回
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_FALSE(m_pVirtualDataSource->IsWebError());
		EXPECT_EQ(m_pVirtualDataSource->GetByteRead(), 1024 * 4 + 10) << "这几次返回值之和";
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData2) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_pVirtualDataSource->SetInquiring(true);
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1)
		.WillOnce(Throw(exception)); // 打开网络文件时出错。
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(0);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(0);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData3) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_pVirtualDataSource->SetInquiring(true);
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(1)
		.WillOnce(Throw(exception)); // 读取网络文件时出错。
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData4) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_pVirtualDataSource->SetInquiring(true);
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(2)
		.WillOnce(Return(1024))
		.WillOnce(Throw(exception)); // 第二次读取网络文件时出错。
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData5) {
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_pVirtualDataSource->SetErrorCode(12002);
		m_pVirtualDataSource->SetInquiring(true);
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(2)
		.WillOnce(Return(1024))
		.WillOnce(DoAll(Invoke([]() { gl_systemConfiguration.SetExitingSystem(true); }), Return(1024))); // 第二次读取时系统要求立即退出。
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError()) << "系统要求立即退出时，会重置网络错误代码";
		EXPECT_EQ(m_pVirtualDataSource->GetErrorCode(), 1) << "系统要求退出时，会设置该错误代码为1";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);

		// restore
		gl_systemMessage.PopErrorMessage();
		gl_systemConfiguration.SetExitingSystem(false);
	}
}
