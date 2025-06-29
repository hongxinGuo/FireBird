#include"pch.h"

#include"GeneralCheck.h"
#include"ChinaMarket.h"

#include"ContainerVirtualStock.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerVirtualStockTest : public Test {
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
			CChinaStockPtr pStock = nullptr;

			for (int i = 0; i < 100; i++) {
				pStock = gl_dataContainerChinaStock.GetStock(i);
				m_containerVirtualStock.Add(pStock);
			}
		}

		void TearDown() override {
			EXPECT_EQ(m_containerVirtualStock.Size(), 100);
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerVirtualStock m_containerVirtualStock;
	};

	TEST_F(CContainerVirtualStockTest, TestIsInSymbolMap1) {
		string strSymbol = gl_dataContainerChinaStock.GetStock(1)->GetSymbol();
		EXPECT_TRUE(m_containerVirtualStock.IsSymbol(strSymbol)) << "位于1-100之间";

		strSymbol = gl_dataContainerChinaStock.GetStock(101)->GetSymbol();
		EXPECT_FALSE(m_containerVirtualStock.IsSymbol(strSymbol)) << "不位于1-100之间";
	}

	TEST_F(CContainerVirtualStockTest, TestIsUpdateProfileDB1) {
		EXPECT_FALSE(m_containerVirtualStock.IsUpdateProfileDB());
		m_containerVirtualStock.Get(0)->SetUpdateProfileDB(true);

		EXPECT_TRUE(m_containerVirtualStock.IsUpdateProfileDB());

		m_containerVirtualStock.Get(0)->SetUpdateProfileDB(false);
	}

	TEST_F(CContainerVirtualStockTest, TestGetSymbols) {
		const vectorString vString = m_containerVirtualStock.GetSymbols();

		EXPECT_STREQ(vString.at(0).c_str(), _T("000001.SS"));
		EXPECT_STREQ(vString.at(1).c_str(), _T("000001.SZ"));
		EXPECT_STREQ(vString.at(2).c_str(), _T("000002.SS"));
	}

	TEST_F(CContainerVirtualStockTest, TestGet1) {
		EXPECT_STREQ(gl_dataContainerChinaStock.GetStock(50)->GetSymbol().c_str(), m_containerVirtualStock.Get(50)->GetSymbol().c_str());
	}

	TEST_F(CContainerVirtualStockTest, TestGet2) {
		const string strSymbol = gl_dataContainerChinaStock.GetStock(1)->GetSymbol();

		EXPECT_STREQ(gl_dataContainerChinaStock.GetStock(strSymbol)->GetSymbol().c_str(), m_containerVirtualStock.Get(strSymbol)->GetSymbol().c_str());
	}

	TEST_F(CContainerVirtualStockTest, TestGetOffset) {
		EXPECT_EQ(m_containerVirtualStock.GetOffset(gl_dataContainerChinaStock.GetStock(0)->GetSymbol()), 0);
		EXPECT_EQ(m_containerVirtualStock.GetOffset(gl_dataContainerChinaStock.GetStock(50)->GetSymbol()), 50);
		EXPECT_EQ(m_containerVirtualStock.GetOffset(gl_dataContainerChinaStock.GetStock(99)->GetSymbol()), 99);
	}

	TEST_F(CContainerVirtualStockTest, TestAdd1) {
		const CVirtualStockPtr pStock = nullptr;

		m_containerVirtualStock.Add(pStock);
		EXPECT_EQ(m_containerVirtualStock.Size(), 100) << "不添加nullptr";
	}

	TEST_F(CContainerVirtualStockTest, TestAdd2) {
		const CVirtualStockPtr pStock = m_containerVirtualStock.Get(1);

		m_containerVirtualStock.Add(pStock);
		EXPECT_EQ(m_containerVirtualStock.Size(), 100) << "不添加已存在的数据";
	}

	TEST_F(CContainerVirtualStockTest, TestAdd3) {
		const CVirtualStockPtr pStock = gl_dataContainerChinaStock.GetStock(101);

		m_containerVirtualStock.Add(pStock);
		EXPECT_EQ(m_containerVirtualStock.Size(), 101) << "添加不存在的数据";

		m_containerVirtualStock.Delete(pStock);
	}

	TEST_F(CContainerVirtualStockTest, TestDelete1) {
		const CVirtualStockPtr pStock = nullptr;

		m_containerVirtualStock.Delete(pStock);
		EXPECT_EQ(m_containerVirtualStock.Size(), 100) << "不删除nullptr";
	}

	TEST_F(CContainerVirtualStockTest, TestDelete2) {
		const CVirtualStockPtr pStock = gl_dataContainerChinaStock.GetStock(101);

		m_containerVirtualStock.Delete(pStock);
		EXPECT_EQ(m_containerVirtualStock.Size(), 100) << "不删除不存在的数据";
	}

	TEST_F(CContainerVirtualStockTest, TestDelete3) {
		const CVirtualStockPtr pStock = gl_dataContainerChinaStock.GetStock(1);

		m_containerVirtualStock.Delete(pStock);
		EXPECT_EQ(m_containerVirtualStock.Size(), 99) << "删除已存在的数据";

		m_containerVirtualStock.Add(pStock);
	}

	TEST_F(CContainerVirtualStockTest, TestUpdateSymbolMap) {
		const CVirtualStockPtr pStock = m_containerVirtualStock.Get(0);
		const CVirtualStockPtr pStock2 = m_containerVirtualStock.Get(1);
		EXPECT_EQ(m_containerVirtualStock.GetOffset(pStock->GetSymbol()), 0);
		m_containerVirtualStock.Delete(pStock);

		EXPECT_EQ(m_containerVirtualStock.Size(), 99) << "删除已存在的数据";
		EXPECT_EQ(m_containerVirtualStock.GetOffset(pStock2->GetSymbol()), 0) << "Delete调用UpdateSymbolMap";

		m_containerVirtualStock.Add(pStock);
		EXPECT_EQ(m_containerVirtualStock.Size(), 100);
		EXPECT_EQ(m_containerVirtualStock.GetOffset(pStock->GetSymbol()), 99) << "Add不调用UpdateSymbolMap";
	}

	TEST_F(CContainerVirtualStockTest, TestSort) {
		const auto pStock = make_shared<CChinaStock>();
		pStock->SetSymbol(_T("0.A")); // 
		m_containerVirtualStock.Add(pStock);
		EXPECT_STREQ(m_containerVirtualStock.Get(m_containerVirtualStock.Size() - 1)->GetSymbol().c_str(), _T("0.A"));
		EXPECT_EQ(m_containerVirtualStock.Size(), 101);

		m_containerVirtualStock.Sort();
		EXPECT_STREQ(m_containerVirtualStock.Get(0)->GetSymbol().c_str(), _T("0.A")) << "0.A位于第一位";

		m_containerVirtualStock.Delete(pStock);
		EXPECT_EQ(m_containerVirtualStock.Size(), 100);
	}
}
