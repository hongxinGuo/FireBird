#include"pch.h"

#include"GeneralCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class InfoReportTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};
}