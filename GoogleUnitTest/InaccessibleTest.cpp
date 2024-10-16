#include"pch.h"

#include"GeneralCheck.h"
#include"Inaccessible.h"

namespace FireBirdTest {
	class CInaccessibleTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			vSymbols.push_back(_T("US"));
			vSymbols.push_back(_T("SS"));
			vSymbols.push_back(_T("SZ"));
			inaccessible.Assign(_T("COMPANY_NEWS"), 1, vSymbols);
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		vector<CString> vSymbols;
		CInaccessible inaccessible;
	};

	TEST_F(CInaccessibleTest, TestAssign) {
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("US"))) << "默认包括US";
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("SZ"))) << "默认包括US";
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("SS"))) << "默认包括US";
		EXPECT_FALSE(inaccessible.HaveSymbol(_T("FA"))) << "默认不包括FA";
		const vector<CString> v{ _T("US2"), _T("SZ2"), _T("SS2") };
		inaccessible.Assign(_T("Another"), 2, v);

		EXPECT_TRUE(inaccessible.HaveSymbol(_T("US2")));
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("SZ2")));
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("SS2")));
		EXPECT_FALSE(inaccessible.HaveSymbol(_T("US")));
		EXPECT_FALSE(inaccessible.HaveSymbol(_T("SZ")));
		EXPECT_FALSE(inaccessible.HaveSymbol(_T("SS")));
	}

	TEST_F(CInaccessibleTest, TestAddSymbol) {
		EXPECT_FALSE(inaccessible.HaveSymbol(_T("FA"))) << "默认不包括FA";
		inaccessible.AddSymbol(_T("FA"));
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("FA"))) << "添加后就包括FA了";
	}

	TEST_F(CInaccessibleTest, TestDeleteSymbol) {
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("US"))) << "默认包括US";
		inaccessible.DeleteSymbol(_T("US"));
		EXPECT_FALSE(inaccessible.HaveSymbol(_T("US"))) << "删除后就不包括US了";
	}

	TEST_F(CInaccessibleTest, TestHaveSymbol) {
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("US"))) << "默认包括US";
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("SZ"))) << "默认包括US";
		EXPECT_TRUE(inaccessible.HaveSymbol(_T("SS"))) << "默认包括US";
		EXPECT_FALSE(inaccessible.HaveSymbol(_T("FA"))) << "默认不包括FA";
	}
}
