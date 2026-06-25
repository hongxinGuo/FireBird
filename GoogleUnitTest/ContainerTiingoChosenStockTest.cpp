#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerTiingoChosenStock.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerTiingoChosenStockTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerTiingoChosenStock m_dataTiingoChosenStock;
	};

	TEST_F(CContainerTiingoChosenStockTest, TestInitialize) {
		EXPECT_EQ(m_dataTiingoChosenStock.Size(), 0);
	}

	TEST_F(CContainerTiingoChosenStockTest, TestLoad) {
		EXPECT_EQ(m_dataTiingoChosenStock.Size(), 0) << "初始未装载IEX代码";

		m_dataTiingoChosenStock.LoadDB();
		EXPECT_EQ(m_dataTiingoChosenStock.Size(), 4) << "默认状态下装载4个代码";

		const CTiingoStockPtr pIEX = dynamic_pointer_cast<CTiingoStock>(m_dataTiingoChosenStock.Get(2));
		EXPECT_EQ(pIEX->GetSymbol(), "AAL") << "装载时没有排序，使用的是原始位置";
	}
}
