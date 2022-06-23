#include"pch.h"

#include"GeneralCheck.h"

#include"Country.h"

#include"WorldMarket.h"

#include"ProductFinnhubEconomicCountryList.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubEconomicCountryListTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CProductFinnhubEconomicCountryList countryList;
	};

	TEST_F(CFinnhubEconomicCountryListTest, TestInitialize) {
		EXPECT_EQ(countryList.GetIndex(), -1);
		EXPECT_STREQ(countryList.GetInquiringStr(), _T("https://finnhub.io/api/v1/country?"));
	}

	TEST_F(CFinnhubEconomicCountryListTest, TestCreatMessage) {
		EXPECT_STREQ(countryList.CreatMessage(), _T("https://finnhub.io/api/v1/country?"));
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	FinnhubWebData finnhubWebData92(2, _T(""), _T("{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"New Country1\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// 第一个数据缺乏CodeNo
	FinnhubWebData finnhubWebData93(3, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"Missing\":\"520\",\"country\":\"New Country\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// 第二个数据缺乏Code2
	FinnhubWebData finnhubWebData94(4, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"New Country\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"Missing\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// 数据缺乏symbol
	FinnhubWebData finnhubWebData95(5, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"New Country\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"Missing\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// 正确的数据
	FinnhubWebData finnhubWebData100(10, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"New Country\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));

	class ParseFinnhubCountryListTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_pvCountry = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CCountryVectorPtr m_pvCountry;
		CProductFinnhubEconomicCountryList m_finnhubEconomicCountryList;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCountryList1, ParseFinnhubCountryListTest,
		testing::Values(&finnhubWebData92, &finnhubWebData93, &finnhubWebData94,
			&finnhubWebData95, &finnhubWebData100));

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
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2, _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode, _T("AUD"));
			break;
		case 5: // 第二个数据缺CodeNo
			EXPECT_EQ(m_pvCountry->size(), 1);
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2, _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode, _T("AUD"));
			break;
		case 10:
			EXPECT_EQ(m_pvCountry->size(), 2);
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2, _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode, _T("AUD"));
			EXPECT_STREQ(m_pvCountry->at(1)->m_strCode2, _T("MF"));
			EXPECT_STREQ(m_pvCountry->at(1)->m_strCurrencyCode, _T("ANG"));
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubCountryListTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubEconomicCountryList.SetMarket(gl_pWorldMarket.get());
		}
		virtual void TearDown(void) override {
			// clearup
			if (gl_pWorldMarket->IsCountry(_T("New Country"))) {
				auto pCountry = gl_pWorldMarket->GetCountry(_T("New Country"));
				gl_pWorldMarket->DeleteCountry(pCountry);
			}
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubEconomicCountryList m_finnhubEconomicCountryList;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCountryList1, ProcessFinnhubCountryListTest,
		testing::Values(&finnhubWebData92, &finnhubWebData93, &finnhubWebData94,
			&finnhubWebData95, &finnhubWebData100));

	TEST_P(ProcessFinnhubCountryListTest, TestProcessFinnhubCountryList0) {
		long l = gl_pWorldMarket->GetTotalCountry();
		CCountryPtr pCountry = nullptr;
		bool fSucceed = m_finnhubEconomicCountryList.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_TRUE(fSucceed);
			break;
		case 3: // 缺乏CodeNo
			EXPECT_TRUE(fSucceed);
			break;
		case 4: // 第二个数据缺Code2
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), l + 1);
			break;
		case 5: // 第二个数据缺CodeNo
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), l + 1);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(gl_pWorldMarket->GetTotalCountry(), l + 1);
			break;
		default:
			break;
		}
	}
}