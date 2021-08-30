#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"QueueString.h"

using namespace std;
#include<string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CQueueString gl_QueueString;

	class CQueueStringTest : public ::testing::Test
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			gl_QueueString.Reset();
			GeneralCheck();
		}
	};

	TEST_F(CQueueStringTest, TestReset) {
		shared_ptr<string> pString = make_shared<string>();
		gl_QueueString.PushData(pString);
		EXPECT_EQ(gl_QueueString.GetDataSize(), 1);
		gl_QueueString.Reset();
		EXPECT_EQ(gl_QueueString.GetDataSize(), 0);
	}

	TEST_F(CQueueStringTest, TestPushPopData1) {
		shared_ptr<string> pString = make_shared<string>(_T("abc"));
		shared_ptr<string> pString2 = make_shared<string>(_T("def"));
		gl_QueueString.PushData(pString);
		gl_QueueString.PushData(pString2);
		shared_ptr<string> pString3 = gl_QueueString.PopData();
		CString str = pString3->c_str();
		EXPECT_STREQ(str, _T("abc")) << "无优先权的队列，与交易时间无关，按进队列的先后顺序出队列";
	}

	TEST_F(CQueueStringTest, TestPushPopData2) {
		string String = _T("abc");
		string String2 = _T("def");
		gl_QueueString.PushData(String);
		gl_QueueString.PushData(String2);
		shared_ptr<string> pString3 = gl_QueueString.PopData();
		CString str = pString3->c_str();
		EXPECT_STREQ(str, _T("abc")) << "无优先权的队列，与交易时间无关，按进队列的先后顺序出队列";
	}
}