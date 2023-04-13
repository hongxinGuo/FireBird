#include"pch.h"

#include"WorldMarket.h"

#include"GeneralCheck.h"

#include"Accessory.h"

namespace FireBirdTest {
	class AccessoryTest : public testing::Test {
		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(AccessoryTest, TestIsAStock2) {
		EXPECT_TRUE(IsShareA(_T("600000.SS")));
		EXPECT_TRUE(IsShareA(_T("601198.SS")));
		EXPECT_FALSE(IsShareA(_T("602102.SS")));
		EXPECT_FALSE(IsShareA(_T("603604.SS")));
		EXPECT_FALSE(IsShareA(_T("604604.SS")));
		EXPECT_FALSE(IsShareA(_T("605604.SS")));
		EXPECT_FALSE(IsShareA(_T("606604.SS")));
		EXPECT_FALSE(IsShareA(_T("607604.SS")));
		EXPECT_FALSE(IsShareA(_T("608604.SS")));
		EXPECT_FALSE(IsShareA(_T("609604.SS")));

		EXPECT_FALSE(IsShareA(_T("600000.SZ")));
		EXPECT_FALSE(IsShareA(_T("000001.SA")));
		EXPECT_FALSE(IsShareA(_T("000001.AZ")));
		EXPECT_FALSE(IsShareA(_T("200001.SZ")));
		EXPECT_TRUE(IsShareA(_T("000001.SZ")));
		EXPECT_TRUE(IsShareA(_T("002389.SZ")));
		EXPECT_FALSE(IsShareA(_T("003389.SZ")));
		EXPECT_FALSE(IsShareA(_T("004389.SZ")));
		EXPECT_FALSE(IsShareA(_T("005389.SZ")));
		EXPECT_FALSE(IsShareA(_T("006389.SZ")));
		EXPECT_FALSE(IsShareA(_T("007389.SZ")));
		EXPECT_FALSE(IsShareA(_T("008389.SZ")));
		EXPECT_FALSE(IsShareA(_T("009389.SZ")));
		EXPECT_FALSE(IsShareA(_T("001389.SZ")));
	}
}
