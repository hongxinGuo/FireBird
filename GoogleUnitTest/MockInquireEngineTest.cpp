#include"pch.h"

////#include"gtest/gtest.h"

#include"globedef.h"

import FireBird.System.Configuration;
#include"SystemMessage.h"

#include"afxinet.h"

#include"MockInquireEngine.h"

#include"GeneralCheck.h"
#include "ProductDummy.h"

using namespace testing;
class CVirtualWebProduct;
typedef std::shared_ptr<CVirtualWebProduct> CVirtualProductWebDataPtr;

namespace FireBirdTest {
	static CMockInquireEnginePtr m_pInquireEngine; // Ϊ�˷�����ҳ���λ��,��mock��������Ϊȫ�ֵġ�
	static CVirtualProductWebDataPtr m_pProduct;

	class CMockInquireEngineTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			m_pInquireEngine = nullptr;
		}

		static void TearDownTestSuite() {
			m_pInquireEngine = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			m_pInquireEngine = make_shared<CMockInquireEngine>();
			m_pProduct = make_shared<CProductDummy>();
		}

		void TearDown() override {
			// clearUp

			m_pInquireEngine = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockInquireEngineTest, TestGetWebData1) {
		EXPECT_FALSE(m_pInquireEngine->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);

		EXPECT_CALL(*m_pInquireEngine, OpenFile).Times(1)
		.WillOnce(Invoke([] {
			auto e = new CInternetException(1);
			throw e;
		}));
		EXPECT_CALL(*m_pInquireEngine, GetFileHeaderInformation).Times(0);

		EXPECT_TRUE(m_pInquireEngine->GetWebData() == nullptr);

		EXPECT_TRUE(m_pInquireEngine->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1) << "����exceptionʱ���������Ϣ";

		// �ָ�ԭ״
		gl_systemMessage.PopErrorMessage();
	}

	TEST_F(CMockInquireEngineTest, TestGetWebData2) {
		EXPECT_FALSE(m_pInquireEngine->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);
		gl_systemConfiguration.SetExitingSystem(true); // �˳�ϵͳ

		EXPECT_CALL(*m_pInquireEngine, OpenFile).Times(1);
		EXPECT_CALL(*m_pInquireEngine, GetFileHeaderInformation).Times(1)
		.WillOnce(Invoke([] {
			m_pInquireEngine->SetContentLength(0);
		}));
		EXPECT_CALL(*m_pInquireEngine, ReadWebFileOneTime).Times(0);

		EXPECT_TRUE(m_pInquireEngine->GetWebData() == nullptr);

		EXPECT_EQ(m_pInquireEngine->GetBufferSize(), 1024 * 1024) << "��֪�����ݳ���ʱ�����ó�ʼ������Ϊ1M";

		EXPECT_TRUE(m_pInquireEngine->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1) << "ϵͳ�˳�ʱ���������Ϣ";

		// �ָ�ԭ״
		gl_systemConfiguration.SetExitingSystem(false);
		gl_systemMessage.PopErrorMessage();
	}

	TEST_F(CMockInquireEngineTest, TestGetWebData3) {
		EXPECT_FALSE(m_pInquireEngine->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);

		EXPECT_CALL(*m_pInquireEngine, OpenFile).Times(1);
		EXPECT_CALL(*m_pInquireEngine, GetFileHeaderInformation).Times(1)
		.WillOnce(Invoke([] {
			m_pInquireEngine->SetContentLength(2048);
		}));
		EXPECT_CALL(*m_pInquireEngine, ReadWebFileOneTime).Times(0);
		EXPECT_CALL(*m_pInquireEngine, ReadWebFileOneTime).Times(2)
		.WillOnce(Return(1024))
		.WillOnce(Return(0));

		CWebDataPtr pWebData = nullptr;
		EXPECT_TRUE((pWebData = m_pInquireEngine->GetWebData()) != nullptr);

		EXPECT_FALSE(m_pInquireEngine->IsWebError());
		EXPECT_EQ(m_pInquireEngine->GetBufferSize(), 0) << "����WebData��������std::move��WebData�У��ʶ�Ϊ��";
		EXPECT_EQ(m_pInquireEngine->GetByteRead(), 1024);

		EXPECT_EQ(pWebData->GetBufferLength(), 1024) << "���������˴�";
		EXPECT_EQ(pWebData->GetTime(), GetUTCTime());

		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 2) << "m_lContentLength != m_lByteRead, ϵͳ�������ݳ��Ȳ�������";

		// �ָ�ԭ״
		gl_systemMessage.PopErrorMessage();
		gl_systemMessage.PopErrorMessage();
	}

	TEST_F(CMockInquireEngineTest, TestGetWebData4) {
		EXPECT_FALSE(m_pInquireEngine->IsWebError());
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0);

		EXPECT_CALL(*m_pInquireEngine, OpenFile).Times(1);
		EXPECT_CALL(*m_pInquireEngine, GetFileHeaderInformation).Times(1)
		.WillOnce(Invoke([] {
			m_pInquireEngine->SetContentLength(1024 * 17);
		}));
		EXPECT_CALL(*m_pInquireEngine, ReadWebFileOneTime).Times(0);
		EXPECT_CALL(*m_pInquireEngine, ReadWebFileOneTime).Times(3)
		.WillOnce(Return(1024 * 16))
		.WillOnce(Return(1024))
		.WillOnce(Return(0));

		CWebDataPtr pWebData = nullptr;
		EXPECT_TRUE((pWebData = m_pInquireEngine->GetWebData()) != nullptr);

		EXPECT_FALSE(m_pInquireEngine->IsWebError());
		EXPECT_EQ(m_pInquireEngine->GetBufferSize(), 0) << "����WebData��������std::move��WebData�У��ʶ�Ϊ��";
		EXPECT_EQ(m_pInquireEngine->GetByteRead(), 1024 * 17);

		EXPECT_EQ(pWebData->GetBufferLength(), 1024 * 17) << "���������˴�";
		EXPECT_EQ(pWebData->GetTime(), GetUTCTime());

		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 0) << "m_lContentLength == m_lByteRead";

		// �ָ�ԭ״
	}
}
