#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ContainerTiingoStock.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerTiingoStockTest : public ::testing::Test {
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
		CContainerTiingoStock m_dataTiingoStock;
	};

	TEST_F(CContainerTiingoStockTest, TestUpdateTiingoStockDB) {
		CSetTiingoStock setTiingoStock;
		EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());
		EXPECT_TRUE(gl_systemConfiguration.IsPaidTypeTiingoAccount()) << "函数UpdateProfile只运行在付费账户状态下";

		auto pTiingoStock = make_shared<CTiingoStock>(); // 这个是数据库中已存在的证券
		pTiingoStock->SetActive(true);
		pTiingoStock->SetIsADR(false);
		pTiingoStock->SetSicCode(1002);
		pTiingoStock->SetCompanyFinancialStatementUpdateDate(20210101);
		pTiingoStock->SetCompanyWebSite("");
		pTiingoStock->SetLocation("");
		pTiingoStock->SetName(""); //
		pTiingoStock->SetReportingCurrency("");
		pTiingoStock->SetSECFilingWebSite("");
		pTiingoStock->SetSicIndustry("");
		pTiingoStock->SetSicSector("Test"); // 用于删除
		pTiingoStock->SetSymbol("A"); // 已存在代码
		pTiingoStock->SetTiingoIndustry("");
		pTiingoStock->SetTiingoPermaTicker("");
		pTiingoStock->SetTiingoSector("");
		pTiingoStock->SetUpdateProfileDB(true);
		gl_dataContainerTiingoStock.UpdateProfile(pTiingoStock);

		pTiingoStock = make_shared<CTiingoStock>();
		pTiingoStock->SetActive(true);
		pTiingoStock->SetIsADR(false);
		pTiingoStock->SetSicCode(1002);
		pTiingoStock->SetCompanyFinancialStatementUpdateDate(20210101);
		pTiingoStock->SetCompanyWebSite("www.abc.com");
		pTiingoStock->SetLocation("Irvine CA USA");
		pTiingoStock->SetName("ABCDE"); // 新代码
		pTiingoStock->SetReportingCurrency("US Dollar");
		pTiingoStock->SetSECFilingWebSite("abc");
		pTiingoStock->SetSicIndustry("Computer Science");
		pTiingoStock->SetSicSector("Test");
		pTiingoStock->SetSymbol("ABCDEF"); // 新代码
		pTiingoStock->SetTiingoIndustry("Computer");
		pTiingoStock->SetTiingoPermaTicker("abcdefg");
		pTiingoStock->SetTiingoSector("gfedcba");
		pTiingoStock->SetUpdateProfileDB(true);
		gl_dataContainerTiingoStock.Add(pTiingoStock);

		EXPECT_TRUE(gl_dataContainerTiingoStock.IsUpdateProfileDB()) << "添加了两个股票";

		gl_dataContainerTiingoStock.UpdateDB(); // 更新代码集

		// 恢复原状
		setTiingoStock.m_strFilter = "[SICSector] = 'Test'";
		setTiingoStock.m_strSort = "[Ticker]";
		setTiingoStock.Open();
		EXPECT_FALSE(setTiingoStock.IsEOF()) << "存入了两股票代码";
		setTiingoStock.m_pDatabase->BeginTrans();
		EXPECT_STREQ(setTiingoStock.m_Ticker, _T("A")) << "已存在代码";
		EXPECT_EQ(setTiingoStock.m_SicCode, 1002);
		setTiingoStock.Edit();
		setTiingoStock.m_SicSector = "";
		setTiingoStock.m_SicCode = 0;
		setTiingoStock.Update();
		setTiingoStock.MoveNext();
		EXPECT_STREQ(setTiingoStock.m_Ticker, _T("ABCDEF")) << "新代码";
		setTiingoStock.Delete();
		setTiingoStock.MoveNext();
		EXPECT_TRUE(setTiingoStock.IsEOF());
		setTiingoStock.m_pDatabase->CommitTrans();
		setTiingoStock.Close();

		gl_dataContainerTiingoStock.Delete(pTiingoStock);
	}

	TEST_F(CContainerTiingoStockTest, TestSetAllFinancialStateStatus) {
		for (size_t index = 0; index < m_dataTiingoStock.Size(); index++) {
			auto pStock = m_dataTiingoStock.GetStock(index);
			EXPECT_TRUE(pStock->IsUpdateFinancialState());
		}

		m_dataTiingoStock.SetUpdateFinancialState(false);

		for (size_t index = 0; index < m_dataTiingoStock.Size(); index++) {
			auto pStock = m_dataTiingoStock.GetStock(index);
			EXPECT_FALSE(pStock->IsUpdateFinancialState());
		}

		// 恢复原状
		m_dataTiingoStock.SetUpdateFinancialState(true);
	}
}
