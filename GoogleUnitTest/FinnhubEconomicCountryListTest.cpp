#include"pch.h"

#include"GeneralCheck.h"
#include"Country.h"
#include "FinnhubDataSource.h"
#include"WorldMarket.h"
#include"ProductFinnhubEconomicCountryList.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubEconomicCountryListTest : public Test {
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
		CProductFinnhubEconomicCountryList countryList;
	};

	TEST_F(CFinnhubEconomicCountryListTest, TestInitialize) {
		EXPECT_EQ(countryList.GetIndex(), 0);
		EXPECT_EQ(countryList.GetInquiryFunction(), "https://finnhub.io/api/v1/country?");
	}

	TEST_F(CFinnhubEconomicCountryListTest, TestCreatMessage) {
		EXPECT_EQ(countryList.CreateMessage(), "https://finnhub.io/api/v1/country?");
	}

	TEST_F(CFinnhubEconomicCountryListTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCountryList());

		countryList.UpdateSystemStatus(gl_pFinnhubDataSource);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateCountryList());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopInformationMessage(), "Finnhub economic country List updated");

		gl_pFinnhubDataSource->SetUpdateCountryList(true);
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	Test_FinnhubWebData finnhubWebData92(2, "", "{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"New Country1\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]");
	// 第一个数据缺乏CodeNo
	Test_FinnhubWebData finnhubWebData93(3, "","[{\"code2\":\"NR\",\"code3\":\"NRU\",\"Missing\":\"520\",\"country\":\"New Country\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]");
	// 第二个数据缺乏Code2
	Test_FinnhubWebData finnhubWebData94(4, "","[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Zero\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"Missing\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]");
	// 数据缺乏symbol
	Test_FinnhubWebData finnhubWebData95(5, "","[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Zero\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"Missing\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]");
	// 空数据
	Test_FinnhubWebData finnhubWebData96(6, "", "{}");
	// 无权访问数据
	Test_FinnhubWebData finnhubWebData97(7, "", "{\"error\":\"You don't have access to this resource.\"}");
	// 正确的数据
	Test_FinnhubWebData finnhubWebData100(10, "", "[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Zero\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]");

	class ParseFinnhubCountryListTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubEconomicCountryList.__Test_checkAccessRight(m_pWebData);

			m_pvCountry = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CCountriesPtr m_pvCountry;
		CProductFinnhubEconomicCountryList m_finnhubEconomicCountryList;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCountryList1, ParseFinnhubCountryListTest,
	                         testing::Values(&finnhubWebData92, &finnhubWebData93, &finnhubWebData94,
		                         &finnhubWebData95, &finnhubWebData96, &finnhubWebData97, &finnhubWebData100));

	TEST_P(ParseFinnhubCountryListTest, TestParseFinnhubCountryList0) {
		m_pvCountry = m_finnhubEconomicCountryList.ParseFinnhubCountryList(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_EQ(m_pvCountry->size(), 0);
			break;
		case 3: // 缺乏CodeNo
			EXPECT_EQ(m_pvCountry->size(), 0);
			break;
		case 4: // 第二个数据缺Code2
			EXPECT_EQ(m_pvCountry->size(), 1);
			EXPECT_EQ(m_pvCountry->at(0)->m_strCode2, "NR");
			EXPECT_EQ(m_pvCountry->at(0)->m_strCurrencyCode, "AUD");
			break;
		case 5: // 第二个数据缺CodeNo
			EXPECT_EQ(m_pvCountry->size(), 1);
			EXPECT_EQ(m_pvCountry->at(0)->m_strCode2, "NR");
			EXPECT_EQ(m_pvCountry->at(0)->m_strCurrencyCode, "AUD");
			break;
		case 6: // 空数据
			EXPECT_TRUE(m_pvCountry->empty());
			EXPECT_TRUE(m_finnhubEconomicCountryList.IsVoidData());
			break;
		case 7: // 无权访问数据
			EXPECT_TRUE(m_pvCountry->empty());
			EXPECT_TRUE(m_finnhubEconomicCountryList.IsNoRightToAccess());
			break;
		case 10:
			EXPECT_EQ(m_pvCountry->size(), 2);
			EXPECT_EQ(m_pvCountry->at(1)->m_strCountry, "Zero") << "以国家名称排序，位于第二个位置";
			EXPECT_EQ(m_pvCountry->at(1)->m_strCode2, "NR");
			EXPECT_EQ(m_pvCountry->at(1)->m_strCurrencyCode, "AUD");
			EXPECT_EQ(m_pvCountry->at(0)->m_strCountry, "Saint Martin (French part)") << "以国家名称排序，位于第一个位置";
			EXPECT_EQ(m_pvCountry->at(0)->m_strCode2, "MF");
			EXPECT_EQ(m_pvCountry->at(0)->m_strCurrencyCode, "ANG");
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubCountryListTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubEconomicCountryList.__Test_checkAccessRight(m_pWebData);

			m_finnhubEconomicCountryList.SetMarket(gl_pWorldMarket);
		}

		void TearDown() override {
			// clearUp
			if (gl_dataContainerFinnhubCountry.IsCountry("Zero")) {
				const auto pCountry = gl_dataContainerFinnhubCountry.GetCountry("Zero");
				gl_dataContainerFinnhubCountry.Delete(pCountry);
			}
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubEconomicCountryList m_finnhubEconomicCountryList;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCountryList1, ProcessFinnhubCountryListTest,
	                         testing::Values(&finnhubWebData92, &finnhubWebData93, &finnhubWebData94,
		                         &finnhubWebData95, &finnhubWebData96, &finnhubWebData97, &finnhubWebData100));

	TEST_P(ProcessFinnhubCountryListTest, TestProcessFinnhubCountryList0) {
		const auto l = gl_dataContainerFinnhubCountry.GetTotalCountry();
		CCountryPtr pCountry = nullptr;
		m_finnhubEconomicCountryList.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			break;
		case 3: // 缺乏CodeNo
			break;
		case 4: // 第二个数据缺Code2
			EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), l + 1);
			break;
		case 5: // 第二个数据缺CodeNo
			EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), l + 1);
			break;
		case 6: // 空数据
			EXPECT_TRUE(m_finnhubEconomicCountryList.IsVoidData());
			break;
		case 7: // 无权访问数据
			EXPECT_TRUE(m_finnhubEconomicCountryList.IsNoRightToAccess());
			break;
		case 10:
			EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), l + 1);
			break;
		default:
			break;
		}
	}
}
