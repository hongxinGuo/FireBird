#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"PriorityQueueWebRTData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CPriorityQueueWebRTData gl_PriorityQueueRTData;

	class CPriorityQueueRTDataTest : public ::testing::Test
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			gl_PriorityQueueRTData.Reset();
			GeneralCheck();
		}
	};
}