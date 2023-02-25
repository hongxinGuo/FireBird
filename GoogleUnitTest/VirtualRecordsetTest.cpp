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
		EXPECT_FALSE(gl_systemStatus.IsWorkingMode()) << "Ĭ��״̬�´˱�ʶΪ�١�";
		EXPECT_STREQ(recordset.GetSchemaConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(recordset.GetSchemaConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false); // ���е�Ԫ����ʱ�����뽫�˱�ʶ����Ϊ�٣��ʶ���������˲��Ժ�������Ҫ�ٴν�����Ϊ�٣���������
		EXPECT_STREQ(recordset.GetSchemaConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	}
}
