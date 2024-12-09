#include"pch.h"

 

#include"GeneralCheck.h"

#include"MockSystemMessage.h"
import FireBird.WindowInterface.OutputWnd;

using namespace testing;
using namespace testing;

namespace FireBirdTest {
	class CMockSystemMessageTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

		CMockSystemDeque systemDeque;
	};

	TEST_F(CMockSystemMessageTest, TestDisplay) {
		COutputList outputList;
		const CString strCmp = _T("20200101: first time test");
		systemDeque.PushMessage(_T("first time test"));
		EXPECT_EQ(systemDeque.Size(), 1);
		EXPECT_CALL(systemDeque, SysCallOutputListAddString(&outputList, strCmp)).Times(1);
		systemDeque.Display(&outputList, _T("20200101"));
		EXPECT_EQ(systemDeque.Size(), 0);
	}
}
