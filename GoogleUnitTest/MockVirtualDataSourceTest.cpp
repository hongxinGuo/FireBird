#include"pch.h"

#include"afxinet.h"

#include"SystemStatus.h"
#include"MockVirtualDataSource.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	static CMockVirtualDataSourcePtr m_pVirtualDataSource; // Ϊ�˷���ʹ��googleMock,��mock��������Ϊȫ�ֵġ�

	class CMockVirtualDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			m_pVirtualDataSource = nullptr;
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			m_pVirtualDataSource = nullptr;
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

	TEST_F(CMockVirtualDataSourceTest, TestRun1) {
		m_pVirtualDataSource->Enable(true);
		EXPECT_FALSE(m_pVirtualDataSource->HaveInquiry());
		auto p = make_shared<CVirtualWebProduct>();
		m_pVirtualDataSource->StoreInquiry(p);
		EXPECT_TRUE(m_pVirtualDataSource->HaveInquiry());
		m_pVirtualDataSource->SetInquiring(false);

		EXPECT_CALL(*m_pVirtualDataSource, GenerateInquiryMessage).Times(0);
		EXPECT_CALL(*m_pVirtualDataSource, ProcessWebDataReceived).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pVirtualDataSource, UpdateStatus).Times(0);
		EXPECT_CALL(*m_pVirtualDataSource, GetWebData).Times(1);

		m_pVirtualDataSource->Run(100000);

		EXPECT_TRUE(m_pVirtualDataSource->IsInquiring());
	}

	TEST_F(CMockVirtualDataSourceTest, TestRun2) {
		m_pVirtualDataSource->Enable(true);
		EXPECT_FALSE(m_pVirtualDataSource->HaveInquiry());
		m_pVirtualDataSource->SetInquiring(false);

		EXPECT_CALL(*m_pVirtualDataSource, GenerateInquiryMessage).Times(1)
		.WillOnce(DoAll(Invoke([]() { m_pVirtualDataSource->SetInquiring(true); }), Return(true)));
		EXPECT_CALL(*m_pVirtualDataSource, ProcessWebDataReceived).Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*m_pVirtualDataSource, UpdateStatus).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, GetWebData).Times(1);

		m_pVirtualDataSource->Run(100000);

		EXPECT_TRUE(m_pVirtualDataSource->IsInquiring());
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData1) {
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
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
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_FALSE(m_pVirtualDataSource->IsWebError());
		EXPECT_EQ(m_pVirtualDataSource->GetByteRead(), 1024 * 4 + 10) << "�⼸�η���ֵ֮��";
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData2) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1)
		.WillOnce(Throw(exception)); // �������ļ�ʱ����
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(0);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(0);
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError());
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData3) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(1)
		.WillOnce(Throw(exception)); // ��ȡ�����ļ�ʱ����
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError());
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData4) {
		CInternetException* exception = new CInternetException(12002);
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(2)
		.WillOnce(Return(1024))
		.WillOnce(Throw(exception)); // �ڶ��ζ�ȡ�����ļ�ʱ����
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_TRUE(m_pVirtualDataSource->IsWebError());
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Net Error #12002 message: http://hq.sinajs.cn/list=sh600000"));
	}

	TEST_F(CMockVirtualDataSourceTest, TestReadWebData5) {
		const CString strInquiry = _T("http://hq.sinajs.cn/list=sh600000");
		m_pVirtualDataSource->SetErrorCode(12002);
		InSequence seq;
		EXPECT_CALL(*m_pVirtualDataSource, OpenFile(strInquiry)).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, GetFileHeaderInformation).Times(1);
		EXPECT_CALL(*m_pVirtualDataSource, ReadWebFileOneTime()).Times(2)
		.WillOnce(Return(1024))
		.WillOnce(DoAll(Invoke([]() { gl_systemStatus.SetExitingSystem(true); }), Return(1024))); // �ڶ��ζ�ȡʱϵͳҪ�������˳���
		m_pVirtualDataSource->SetInquiringWebData(true);
		m_pVirtualDataSource->SetInquiringString(strInquiry);

		m_pVirtualDataSource->ReadWebData();

		EXPECT_FALSE(m_pVirtualDataSource->IsWebError()) << "ϵͳҪ�������˳�ʱ������������������";
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);

		// restore
		gl_systemStatus.SetExitingSystem(false);
	}

	TEST_F(CMockVirtualDataSourceTest, TestGetWebData) {
		m_pVirtualDataSource->SetInquiringWebData(false);
		EXPECT_FALSE(m_pVirtualDataSource->IsInquiringWebData());
		EXPECT_CALL(*m_pVirtualDataSource, StartReadingThread()).Times(1);
		m_pVirtualDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pVirtualDataSource->IsInquiringWebData()) << _T("Ԥ�����õĴ˱�ʶ������Mock��û������֮���ʶ�������������״̬\n");
	}

	TEST_F(CMockVirtualDataSourceTest, TestStartReadingThread) {
		EXPECT_CALL(*m_pVirtualDataSource, StartReadingThread()).Times(1);
		m_pVirtualDataSource->StartReadingThread();
	}
}
