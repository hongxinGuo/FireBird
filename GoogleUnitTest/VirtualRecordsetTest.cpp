#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"VirtualRecordset.h"

using namespace testing;

namespace FireBirdTest {
	class CVirtualRecordsetTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CVirtualRecordsetTest, TestGetSchemaConnect) {
		CVirtualRecordset recordset(_T("WorldMarket"), _T("defaultTable"));
		EXPECT_FALSE(gl_systemStatus.IsWorkingMode()) << "默认状态下此标识为假。";
		EXPECT_STREQ(recordset.GetSchemaConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(recordset.GetSchemaConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false); // 运行单元测试时，必须将此标识设置为假，故而在运行完此测试函数后，需要再次将其置为假，否则会出错。
		EXPECT_STREQ(recordset.GetSchemaConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	}
}
