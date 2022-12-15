#include"pch.h"

#include"GeneralCheck.h"

#include"WebData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CWebDataTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
			EXPECT_EQ(m_id.GetCurrentPos(), 0);
			EXPECT_EQ(m_id.GetTime(), 0);
			EXPECT_EQ(m_id.GetBufferLength(), 1024 * 1024);
			m_id.ResetCurrentPos();
			EXPECT_EQ(m_id.GetCurrentPos(), 0);
		}

		virtual void TearDown(void) override {
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			GeneralCheck();
			m_id.SetBufferLength(1024 * 1024);
		}

	public:
		CWebData m_id;
	};

	TEST_F(CWebDataTest, TestGetTime) {
		EXPECT_EQ(m_id.GetTime(), 0);
		m_id.SetTime(101010101010);
		EXPECT_EQ(m_id.GetTime(), 101010101010);
	}

	TEST_F(CWebDataTest, TestGetStockCode) {
		EXPECT_STREQ(m_id.GetStockCode(), _T(""));
		m_id.SetStockCode(_T("abcdefg"));
		EXPECT_STREQ(m_id.GetStockCode(), _T("abcdefg"));
	}

	TEST_F(CWebDataTest, TestGetCurrentPos) {
		EXPECT_EQ(m_id.GetCurrentPos(), 0);
		m_id.IncreaseCurrentPos(5);
		EXPECT_EQ(m_id.GetCurrentPos(), 5);
	}

	TEST_F(CWebDataTest, TestIncreaseCurrentPos) {
		m_id.IncreaseCurrentPos();
		EXPECT_EQ(m_id.GetCurrentPos(), 1);
		m_id.IncreaseCurrentPos(10);
		EXPECT_EQ(m_id.GetCurrentPos(), 11);
	}

	TEST_F(CWebDataTest, TestGetData1) {
		char buffer[10];

		EXPECT_EQ(m_id.GetBufferLength(), 1024 * 1024);
		EXPECT_EQ(m_id.GetCurrentPos(), 0);
		m_id.SetCurrentPos(1024 * 1024 - 9);
		EXPECT_FALSE(m_id.GetData(buffer, 10)) << "越界";
	}

	TEST_F(CWebDataTest, TestGetData2) {
		m_id.SetData(0, 'a');
		EXPECT_EQ(m_id.GetData(0), 'a');
		m_id.SetData(10, 'a');
		EXPECT_EQ(m_id.GetData(10), 'a');
	}

	TEST_F(CWebDataTest, TestGetData3) {
		CString strTest = "abcdefg";
		EXPECT_TRUE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength(), 0));
		char buffer[8];
		EXPECT_TRUE(m_id.GetData(buffer, 7, 0));
		buffer[7] = 0x000;
		CString strTest2 = buffer;
		EXPECT_STREQ(strTest, strTest2);

		EXPECT_TRUE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength(), 100));
		EXPECT_TRUE(m_id.GetData(buffer, 7, 100));
		buffer[7] = 0x000;
		strTest2 = buffer;
		EXPECT_STREQ(strTest, strTest2);

		m_id.SetBufferLength(6);
		EXPECT_FALSE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength(), 0)) << "数据容器长度为6个字节，但需要存储七个字节的数据，导致函数报错";
	}

	TEST_F(CWebDataTest, TestGetData4) {
		CString strTest = "abcdefg";
		m_id.SetCurrentPos(0);
		EXPECT_TRUE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength()));
		char buffer[8];
		EXPECT_TRUE(m_id.GetData(buffer, 7));
		buffer[7] = 0x000;
		CString strTest2 = buffer;
		EXPECT_STREQ(strTest, strTest2);

		m_id.SetCurrentPos(1000);
		EXPECT_TRUE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength()));
		EXPECT_TRUE(m_id.GetData(buffer, 7));
		buffer[7] = 0x000;
		strTest2 = buffer;
		EXPECT_STREQ(strTest, strTest2);

		m_id.SetBufferLength(6);
		m_id.SetCurrentPos(0);
		EXPECT_FALSE(m_id.SetData(strTest.GetBuffer(), strTest.GetLength())) << "数据容器长度为6个字节，但需要存储七个字节的数据，导致函数报错";
	}

	TEST_F(CWebDataTest, TestGetCurrentPosData) {
		m_id.SetCurrentPos(1);
		m_id.SetData(1, 'a');
		EXPECT_EQ(m_id.GetCurrentPosData(), 'a');
		m_id.SetCurrentPos(2);
		m_id.SetCurrentPosData('b');
		EXPECT_EQ(m_id.GetCurrentPosData(), 'b');
	}

	TEST_F(CWebDataTest, TestIsErrorMessage1) {
		m_id.Test_SetBuffer_("{\"error\":\"You don't have access to this resource.\"}");
		EXPECT_TRUE(m_id.ParseUsingPropertyTree());
		EXPECT_TRUE(m_id.IsParsed());

		EXPECT_TRUE(m_id.CheckNoRightToAccess());
	}

	TEST_F(CWebDataTest, TestIsErrorMessage2) {
		m_id.Test_SetBuffer_("{\"error2\":\"You don't have access to this resource.\"}");
		EXPECT_TRUE(m_id.ParseUsingPropertyTree());
		EXPECT_TRUE(m_id.IsParsed());

		EXPECT_FALSE(m_id.CheckNoRightToAccess());

		m_id.Test_SetBuffer_("{\"error\":\"You don't have access to this resour\"}");
		EXPECT_TRUE(m_id.ParseUsingPropertyTree());
		EXPECT_TRUE(m_id.IsParsed());

		EXPECT_FALSE(m_id.CheckNoRightToAccess());
	}

	TEST_F(CWebDataTest, TestIsErrorMessage3) {
		m_id.Test_SetBuffer_("{\"no error\":\"You don't have access to this resource.\"}");
		EXPECT_TRUE(m_id.ParseUsingPropertyTree());
		EXPECT_TRUE(m_id.IsParsed());

		EXPECT_FALSE(m_id.CheckNoRightToAccess());
		EXPECT_TRUE(m_id.CheckNoRightToAccess(_T("no error"), _T("You don't have access to this resource."))) << "使用参数的模式，内容与参数相符即可";
		EXPECT_FALSE(m_id.CheckNoRightToAccess(_T("no error"), _T("You don't have access to that resource."))) << "使用参数的模式，内容与参数不相符：报错";
	}
}
