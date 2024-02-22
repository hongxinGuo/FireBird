#include"pch.h"

#include"afxinet.h"

#include"MockVirtualDataSource.h"

#include"GeneralCheck.h"
#include "ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	static CMockVirtualDataSourcePtr m_pVirtualDataSource; // Ϊ�˷�����ҳ���λ��,��mock��������Ϊȫ�ֵġ�
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

		// �ָ�ԭ״
		m_pVirtualDataSource->SetWorkingThreadRunning(false);
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebDataAndProcessIt2) {
		m_pVirtualDataSource->SetWorkingThreadRunning(true);
		EXPECT_CALL(*m_pVirtualDataSource, GetWebData).Times(1)
		.WillOnce(Invoke([] { m_pVirtualDataSource->SetErrorCode(0); }));
		EXPECT_CALL(*m_pVirtualDataSource, ProcessWebDataReceived).Times(1);

		m_pVirtualDataSource->GetWebDataAndProcessIt();

		// �ָ�ԭ״
		m_pVirtualDataSource->SetWorkingThreadRunning(false);
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebDataAndProcessIt3) {
		m_pVirtualDataSource->SetWorkingThreadRunning(true);
		EXPECT_CALL(*m_pVirtualDataSource, GetWebData).Times(1)
		.WillOnce(Invoke([] { m_pVirtualDataSource->SetErrorCode(0); }));
		EXPECT_CALL(*m_pVirtualDataSource, ProcessWebDataReceived).Times(1);

		m_pVirtualDataSource->GetWebDataAndProcessIt();

		// �ָ�ԭ״
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
		.WillOnce(Return(1024)) //��һ�η���ֵΪ0
		.WillOnce(Return(1024))
		.WillOnce(Return(1024)) //�����η���ֵΪ����
		.WillOnce(Return(1024))
		.WillOnce(Return(10))
		.WillOnce(Return(0)); // ���Ϊ�㣬����˳������
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_FALSE(m_pVirtualDataSource->IsWebError());
		EXPECT_EQ(m_pVirtualDataSource->GetByteRead(), 1024 * 4 + 10) << "�⼸�η���ֵ֮��";
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData2) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_pVirtualDataSource->SetInquiring(true);
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1)
		.WillOnce(Throw(exception)); // �������ļ�ʱ����
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
		.WillOnce(Throw(exception)); // ��ȡ�����ļ�ʱ����
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
		.WillOnce(Throw(exception)); // �ڶ��ζ�ȡ�����ļ�ʱ����
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
		.WillOnce(DoAll(Invoke([]() { gl_systemConfiguration.SetExitingSystem(true); }), Return(1024))); // �ڶ��ζ�ȡʱϵͳҪ�������˳���
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError()) << "ϵͳҪ�������˳�ʱ������������������";
		EXPECT_EQ(m_pVirtualDataSource->GetErrorCode(), 1) << "ϵͳҪ���˳�ʱ�������øô������Ϊ1";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);

		// restore
		gl_systemMessage.PopErrorMessage();
		gl_systemConfiguration.SetExitingSystem(false);
	}
}
