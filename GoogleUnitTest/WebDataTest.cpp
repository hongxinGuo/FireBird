#include"pch.h"

#include"GeneralCheck.h"

#include"WebData.h"

namespace FireBirdTest {
	class CWebDataTest : public ::testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(webData.GetCurrentPos(), 0);
			EXPECT_EQ(webData.GetTime(), 0);
			EXPECT_EQ(webData.GetBufferLength(), 1024 * 1024);
			webData.ResetCurrentPos();
			EXPECT_EQ(webData.GetCurrentPos(), 0);
		}

		void TearDown() override {
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		CWebData webData;
	};

	TEST_F(CWebDataTest, TestOutOfRange) {
		webData.SetBufferLength(100);
		webData.SetCurrentPos(0);
		EXPECT_FALSE(webData.OutOfRange());
		webData.SetCurrentPos(99);
		EXPECT_FALSE(webData.OutOfRange());
		webData.SetCurrentPos(100);
		EXPECT_TRUE(webData.OutOfRange());
	}

	TEST_F(CWebDataTest, TestIsLastParagraph) {
		webData.SetBufferLength(100);
		webData.SetCurrentPos(0);
		EXPECT_FALSE(webData.IsLastParagraph());
		webData.SetCurrentPos(97);
		EXPECT_FALSE(webData.IsLastParagraph());
		webData.SetCurrentPos(98);
		EXPECT_TRUE(webData.IsLastParagraph());
		webData.SetCurrentPos(99);
		EXPECT_TRUE(webData.IsLastParagraph());
		webData.SetCurrentPos(100);
		EXPECT_TRUE(webData.IsLastParagraph());
	}

	TEST_F(CWebDataTest, TestGetTime) {
		EXPECT_EQ(webData.GetTime(), 0);
		webData.SetTime(101010101010);
		EXPECT_EQ(webData.GetTime(), 101010101010);
	}

	TEST_F(CWebDataTest, TestGetStockCode) {
		EXPECT_STREQ(webData.GetStockCode(), _T(""));
		webData.SetStockCode(_T("abcdefg"));
		EXPECT_STREQ(webData.GetStockCode(), _T("abcdefg"));
	}

	TEST_F(CWebDataTest, TestIsProcessedAllTheData) {
		webData.SetBufferLength(10000);
		webData.SetCurrentPos(0);

		EXPECT_FALSE(webData.IsProcessedAllTheData());

		webData.SetCurrentPos(9999);
		EXPECT_FALSE(webData.IsProcessedAllTheData());

		webData.SetCurrentPos(10000);
		EXPECT_TRUE(webData.IsProcessedAllTheData());
	}

	TEST_F(CWebDataTest, TestGetCurrentPos) {
		EXPECT_EQ(webData.GetCurrentPos(), 0);
		webData.IncreaseCurrentPos(5);
		EXPECT_EQ(webData.GetCurrentPos(), 5);
	}

	TEST_F(CWebDataTest, TestIncreaseCurrentPos) {
		webData.IncreaseCurrentPos();
		EXPECT_EQ(webData.GetCurrentPos(), 1);
		webData.IncreaseCurrentPos(10);
		EXPECT_EQ(webData.GetCurrentPos(), 11);
	}

	TEST_F(CWebDataTest, TestGetData1) {
		char buffer[10];

		EXPECT_EQ(webData.GetBufferLength(), 1024 * 1024);
		EXPECT_EQ(webData.GetCurrentPos(), 0);
		webData.SetCurrentPos(1024 * 1024 - 9);
		EXPECT_FALSE(webData.GetData(buffer, 10)) << "越界";
	}

	TEST_F(CWebDataTest, TestGetData2) {
		webData.SetData(0, 'a');
		EXPECT_EQ(webData.GetData(0), 'a');
		webData.SetData(10, 'a');
		EXPECT_EQ(webData.GetData(10), 'a');
	}

	TEST_F(CWebDataTest, TestGetData4) {
		CString strTest = "abcdefg";
		webData.SetCurrentPos(0);
		EXPECT_TRUE(webData.SetData(strTest.GetBuffer(), strTest.GetLength()));
		char buffer[8];
		EXPECT_TRUE(webData.GetData(buffer, 7));
		buffer[7] = 0x000;
		CString strTest2 = buffer;
		EXPECT_STREQ(strTest, strTest2);

		webData.SetCurrentPos(1000);
		EXPECT_TRUE(webData.SetData(strTest.GetBuffer(), strTest.GetLength()));
		EXPECT_TRUE(webData.GetData(buffer, 7));
		buffer[7] = 0x000;
		strTest2 = buffer;
		EXPECT_STREQ(strTest, strTest2);

		webData.SetBufferLength(6);
		webData.SetCurrentPos(0);
		EXPECT_FALSE(webData.SetData(strTest.GetBuffer(), strTest.GetLength())) << "数据容器长度为6个字节，但需要存储七个字节的数据，导致函数报错";
	}

	TEST_F(CWebDataTest, TestGetCurrentPosData) {
		webData.SetCurrentPos(1);
		webData.SetData(1, 'a');
		EXPECT_EQ(webData.GetCurrentPosData(), 'a');
		webData.SetCurrentPos(2);
		webData.SetCurrentPosData('b');
		EXPECT_EQ(webData.GetCurrentPosData(), 'b');
	}

	TEST_F(CWebDataTest, TestIsErrorMessage1) {
		webData.Test_SetBuffer_("{\"error\":\"You don't have access to this resource.\"}");
		EXPECT_TRUE(webData.CreateJson());
		EXPECT_TRUE(webData.IsParsed());

		EXPECT_TRUE(webData.CheckNoRightToAccess());
	}

	TEST_F(CWebDataTest, TestIsErrorMessage2) {
		webData.Test_SetBuffer_("{\"error2\":\"You don't have access to this resource.\"}");
		EXPECT_TRUE(webData.CreateJson());
		EXPECT_TRUE(webData.IsParsed());

		EXPECT_FALSE(webData.CheckNoRightToAccess());

		webData.Test_SetBuffer_("{\"error\":\"You don't have access to this resour\"}");
		EXPECT_TRUE(webData.CreateJson());
		EXPECT_TRUE(webData.IsParsed());

		EXPECT_FALSE(webData.CheckNoRightToAccess());
	}

	TEST_F(CWebDataTest, TestIsErrorMessage3) {
		webData.Test_SetBuffer_("{\"no error\":\"You don't have access to this resource.\"}");
		EXPECT_TRUE(webData.CreateJson());
		EXPECT_TRUE(webData.IsParsed());

		EXPECT_FALSE(webData.CheckNoRightToAccess());
		EXPECT_TRUE(webData.CheckNoRightToAccess(_T("no error"), _T("You don't have access to this resource."))) << "使用参数的模式，内容与参数相符即可";
		EXPECT_FALSE(webData.CheckNoRightToAccess(_T("no error"), _T("You don't have access to that resource."))) << "使用参数的模式，内容与参数不相符：报错";
	}
}
