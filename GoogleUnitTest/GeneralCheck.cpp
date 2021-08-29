#include"pch.h"

using namespace testing;

#include"globedef.h"
#include"SystemMessage.h"
#include "GeneralCheck.h"

namespace StockAnalysisTest {
	void SystemMessageCheck(void) {
		EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
		EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();

		while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
		while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
		while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
	}

	void EnvironmentCheck(void) {
		ASSERT_FALSE(gl_fNormalMode);
		ASSERT_TRUE(gl_fTestMode);

		EXPECT_FALSE(gl_fExitingSystem);
	}

	void ChinaMarketCheck(void) {
		if (gl_pChinaMarket != nullptr) {
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
		}
	}

	void WorldMarketCheck(void) {
	}

	void GeneralCheck(void) {
		EnvironmentCheck();
		SystemMessageCheck();
		ChinaMarketCheck();
		WorldMarketCheck();
	}
}