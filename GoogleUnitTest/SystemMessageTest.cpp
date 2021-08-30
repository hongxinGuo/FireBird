//////////////////////////////////////////////////////////////////////////////////////////
//
// ϵͳ��Ϣ�еĸ��������Բ��õ�ͬ�������Է�ֹ�߳�֮��������ݲ�ͬ�����⣬�����µĲ��Ժ���û�в�������ͬ�����ƣ�
// ֻ�Ǽ򵥵Ĳ������ڲ��߼���
//
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"SystemMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class SystemMessageTest : public ::testing::Test
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			GeneralCheck();

			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
			while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
			while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
		}
	};

	TEST_F(SystemMessageTest, TestInitialize) {
		ASSERT_FALSE(gl_fNormalMode);
		size_t l = gl_systemMessage.GetInformationDequeSize();
		CSystemMessage systemMessage; // ���ɵڶ���ʵ������һ��Ϊȫ�ֱ�����ϵͳ����ʱ�������ˣ�
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // ϵͳ��������
		for (int i = 0; i < l + 1; i++) {
			CString str = gl_systemMessage.PopInformationMessage(); // �����Ϣ����
		}
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
	}

	TEST_F(SystemMessageTest, TestGetInformationDequeSize) {
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
		gl_systemMessage.PushInformationMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 1);
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetDayLineInfoDequeSize) {
		EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
		gl_systemMessage.PushDayLineInfoMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 1);
		CString str = gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetFindDequeSize) {
		ASSERT_FALSE(gl_fNormalMode);
		EXPECT_EQ(gl_systemMessage.GetTransactionDequeSize(), 0);
		gl_systemMessage.PushTransactionMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetTransactionDequeSize(), 1);
		CString str = gl_systemMessage.PopTransactionMessage();
		EXPECT_EQ(gl_systemMessage.GetTransactionDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetCancelSellDequeSize) {
		EXPECT_EQ(gl_systemMessage.GetCancelSellDequeSize(), 0);
		gl_systemMessage.PushCancelSellMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetCancelSellDequeSize(), 1);
		CString str = gl_systemMessage.PopCancelSellMessage();
		EXPECT_EQ(gl_systemMessage.GetCancelSellDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetCancelBuyDequeSize) {
		EXPECT_EQ(gl_systemMessage.GetCancelBuyDequeSize(), 0);
		gl_systemMessage.PushCancelBuyMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetCancelBuyDequeSize(), 1);
		CString str = gl_systemMessage.PopCancelBuyMessage();
		EXPECT_EQ(gl_systemMessage.GetCancelBuyDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetTrace2DequeSize) {
		EXPECT_EQ(gl_systemMessage.GetTrace2DequeSize(), 0);
		gl_systemMessage.PushTrace2Message(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetTrace2DequeSize(), 1);
		CString str = gl_systemMessage.PopTrace2Message();
		EXPECT_EQ(gl_systemMessage.GetTrace2DequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}
}