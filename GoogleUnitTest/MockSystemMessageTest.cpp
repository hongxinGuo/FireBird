#include"pch.h"

#include"GeneralCheck.h"

#include"MockSystemMessage.h"
#include"OutputWnd.h"

using namespace testing;
using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CMockSystemMessageTest : public ::testing::Test
	{
	protected:
		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearup
		}
		CMockSystemDeque systemDeque;
	};

	TEST_F(CMockSystemMessageTest, TestDisplay) {
		COutputList outputList;
		CString strCmp = _T("20200101: first time test");
		systemDeque.PushMessage(_T("first time test"));
		EXPECT_EQ(systemDeque.Size(), 1);
		EXPECT_CALL(systemDeque, SysCallOutputListAddString(&outputList, strCmp)).Times(1);
		systemDeque.Display(&outputList, _T("20200101"));
		EXPECT_EQ(systemDeque.Size(), 0);
	}
}