#include"pch.h"

#include"globedef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	struct UpdateWeekLineDataStr {
		UpdateWeekLineDataStr() {
		}
	public:
		int m_iCount;
		CString m_strData;
	};

	UpdateWeekLineDataStr Data1;

	class UpdateWeekLineDataStrTest : public::testing::TestWithParam<UpdateWeekLineDataStr*> {
	protected:
		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			UpdateWeekLineDataStr* pData = GetParam();
			m_iCount = pData->m_iCount;
			m_strCode = pData->m_strData;
		}

		virtual void TearDown(void) override {
			// clearup
		}

	public:
		int m_iCount;
		CString m_strCode;
	};

	INSTANTIATE_TEST_SUITE_P(TestUpdateWeekLine, UpdateWeekLineDataStrTest,
		testing::Values(&Data1
		));

	TEST_P(UpdateWeekLineDataStrTest, TestUpdateWeekLine) {
	}
}