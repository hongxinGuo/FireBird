#include"pch.h"

#include"afxinet.h"
#include "FinnhubForexSymbol.h"

#include"SystemStatus.h"
#include"MockVirtualDataSource.h"

#include"GeneralCheck.h"
#include "ProductTiingoStockSymbol.h"

using namespace testing;

namespace FireBirdTest {
	static CMockVirtualDataSourcePtr m_pVirtualDataSource; // 为了方便使用googleMock,将mock变量声明为全局的。

	class CMockVirtualDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			m_pVirtualDataSource = nullptr;
		}

		static void TearDownTestSuite(void) {
			m_pVirtualDataSource = nullptr;
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			m_pVirtualDataSource = make_shared<CMockVirtualDataSource>();
		}

		void TearDown(void) override {
			// clearUp

			m_pVirtualDataSource->SetInquiringString(_T(""));
			m_pVirtualDataSource = nullptr;
			GeneralCheck();
		}

	public:
	};

	TEST_F(CMockVirtualDataSourceTest, TestRun1) {
		m_pVirtualDataSource->Enable(true);
		EXPECT_FALSE(m_pVirtualDataSource->HaveInquiry());
		m_pVirtualDataSource->SetInquiring(false);

		EXPECT_CALL(*m_pVirtualDataSource, GenerateInquiryMessage).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pVirtualDataSource, CreateThreadGetWebDataAndProcessIt).Times(0);

		m_pVirtualDataSource->Run(100000);

		EXPECT_FALSE(m_pVirtualDataSource->IsInquiring());
	}

	TEST_F(CMockVirtualDataSourceTest, TestRun2) {
		m_pVirtualDataSource->Enable(true);
		EXPECT_FALSE(m_pVirtualDataSource->HaveInquiry());
		m_pVirtualDataSource->SetInquiring(false);
		m_pVirtualDataSource->SetGetWebDataAndProcessItThreadRunning(false);
		CVirtualProductWebDataPtr p = make_shared<CVirtualWebProduct>();

		EXPECT_CALL(*m_pVirtualDataSource, GenerateInquiryMessage).Times(1)
		.WillOnce(DoAll(Invoke([p]() {
			m_pVirtualDataSource->StoreInquiry(p);
			m_pVirtualDataSource->SetInquiring(true);
		}), Return(true)));
		EXPECT_CALL(*m_pVirtualDataSource, CreateThreadGetWebDataAndProcessIt).Times(1)
		.WillOnce(Invoke([]() {
			m_pVirtualDataSource->SetInquiring(false);
			m_pVirtualDataSource->SetGetWebDataAndProcessItThreadRunning(false);
		}));

		m_pVirtualDataSource->Run(100000);

		EXPECT_FALSE(m_pVirtualDataSource->IsInquiring());
		EXPECT_FALSE(m_pVirtualDataSource->IsGetWebDataAndProcessItThreadRunning());
	}

	TEST_F(CMockVirtualDataSourceTest, TestRun3) {
		m_pVirtualDataSource->Enable(true);
		EXPECT_FALSE(m_pVirtualDataSource->HaveInquiry());
		m_pVirtualDataSource->SetInquiring(true);
		m_pVirtualDataSource->SetGetWebDataAndProcessItThreadRunning(false);
		auto p = make_shared<CVirtualWebProduct>();
		m_pVirtualDataSource->StoreInquiry(p);

		EXPECT_CALL(*m_pVirtualDataSource, GenerateInquiryMessage).Times(0);
		EXPECT_CALL(*m_pVirtualDataSource, CreateThreadGetWebDataAndProcessIt).Times(1)
		.WillOnce(Invoke([]() { m_pVirtualDataSource->SetInquiring(false); }));

		m_pVirtualDataSource->Run(100000);

		EXPECT_FALSE(m_pVirtualDataSource->IsInquiring());
	}

	TEST_F(CMockVirtualDataSourceTest, TestRun4) {
		m_pVirtualDataSource->Enable(true);
		EXPECT_FALSE(m_pVirtualDataSource->HaveInquiry());
		m_pVirtualDataSource->SetInquiring(true);
		m_pVirtualDataSource->SetGetWebDataAndProcessItThreadRunning(true);
		auto p = make_shared<CVirtualWebProduct>();
		m_pVirtualDataSource->StoreInquiry(p);

		EXPECT_CALL(*m_pVirtualDataSource, GenerateInquiryMessage).Times(0);
		EXPECT_CALL(*m_pVirtualDataSource, CreateThreadGetWebDataAndProcessIt).Times(0);

		m_pVirtualDataSource->Run(100000);

		EXPECT_TRUE(m_pVirtualDataSource->IsInquiring());
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
		.WillOnce(DoAll(Invoke([]() { gl_systemStatus.SetExitingSystem(true); }), Return(1024))); // 第二次读取时系统要求立即退出。
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_FALSE(m_pVirtualDataSource->IsWebError()) << "系统要求立即退出时，会重置网络错误代码";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);

		// restore
		gl_systemStatus.SetExitingSystem(false);
	}

	TEST_F(CMockVirtualDataSourceTest, TestProcessInquiryMessage) {
		m_pVirtualDataSource->SetInquiring(true);
		EXPECT_CALL(*m_pVirtualDataSource, StartReadingThread()).Times(1);

		m_pVirtualDataSource->ProcessInquiryMessage();
	}

	TEST_F(CMockVirtualDataSourceTest, TestStartReadingThread) {
		EXPECT_CALL(*m_pVirtualDataSource, StartReadingThread()).Times(1);
		m_pVirtualDataSource->StartReadingThread();
	}
}
