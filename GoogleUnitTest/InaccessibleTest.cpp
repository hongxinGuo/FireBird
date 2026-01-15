#include"pch.h"

#include"GeneralCheck.h"
#include"Inaccessible.h"

namespace FireBirdTest {
	class CInaccessibleTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			vSymbols.emplace_back("US");
			vSymbols.emplace_back("SS");
			vSymbols.emplace_back("SZ");
			inaccessible.Assign("COMPANY_NEWS", 1, vSymbols);
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		vector<string> vSymbols;
		CInaccessible inaccessible;
	};

	TEST_F(CInaccessibleTest, TestAssign) {
		EXPECT_TRUE(inaccessible.HaveSymbol("US")) << "默认包括US";
		EXPECT_TRUE(inaccessible.HaveSymbol("SZ")) << "默认包括US";
		EXPECT_TRUE(inaccessible.HaveSymbol("SS")) << "默认包括US";
		EXPECT_FALSE(inaccessible.HaveSymbol("FA")) << "默认不包括FA";
		const vector<string> v{ "US2", "SZ2", "SS2" };
		inaccessible.Assign("Another", 2, v);

		EXPECT_TRUE(inaccessible.HaveSymbol("US2"));
		EXPECT_TRUE(inaccessible.HaveSymbol("SZ2"));
		EXPECT_TRUE(inaccessible.HaveSymbol("SS2"));
		EXPECT_FALSE(inaccessible.HaveSymbol("US"));
		EXPECT_FALSE(inaccessible.HaveSymbol("SZ"));
		EXPECT_FALSE(inaccessible.HaveSymbol("SS"));
	}

	TEST_F(CInaccessibleTest, TestAddSymbol) {
		EXPECT_FALSE(inaccessible.HaveSymbol("FA")) << "默认不包括FA";
		inaccessible.AddSymbol("FA");
		EXPECT_TRUE(inaccessible.HaveSymbol("FA")) << "添加后就包括FA了";
	}

	TEST_F(CInaccessibleTest, TestDeleteSymbol) {
		EXPECT_TRUE(inaccessible.HaveSymbol("US")) << "默认包括US";
		inaccessible.DeleteSymbol("US");
		EXPECT_FALSE(inaccessible.HaveSymbol("US")) << "删除后就不包括US了";
	}

	TEST_F(CInaccessibleTest, TestHaveSymbol) {
		EXPECT_TRUE(inaccessible.HaveSymbol("US")) << "默认包括US";
		EXPECT_TRUE(inaccessible.HaveSymbol("SZ")) << "默认包括US";
		EXPECT_TRUE(inaccessible.HaveSymbol("SS")) << "默认包括US";
		EXPECT_FALSE(inaccessible.HaveSymbol("FA")) << "默认不包括FA";
	}
}
