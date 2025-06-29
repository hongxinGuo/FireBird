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
		EXPECT_STREQ(countryList.GetInquiryFunction().c_str(), _T("https://finnhub.io/api/v1/country?"));
	}

	TEST_F(CFinnhubEconomicCountryListTest, TestCreatMessage) {
		EXPECT_STREQ(countryList.CreateMessage().c_str(), _T("https://finnhub.io/api/v1/country?"));
	}

	TEST_F(CFinnhubEconomicCountryListTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCountryList());

		countryList.UpdateDataSourceStatus(gl_pFinnhubDataSource);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateCountryList());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage().c_str(), _T("Finnhub economic country List updated"));

		gl_pFinnhubDataSource->SetUpdateCountryList(true);
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	Test_FinnhubWebData finnhubWebData92(2, _T(""), _T("{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"New Country1\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// ��һ������ȱ��CodeNo
	Test_FinnhubWebData finnhubWebData93(3, _T(""),_T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"Missing\":\"520\",\"country\":\"New Country\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// �ڶ�������ȱ��Code2
	Test_FinnhubWebData finnhubWebData94(4, _T(""),_T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Zero\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"Missing\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// ����ȱ��symbol
	Test_FinnhubWebData finnhubWebData95(5, _T(""),_T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Zero\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"Missing\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));
	// ������
	Test_FinnhubWebData finnhubWebData96(6, _T(""), _T("{}"));
	// ��Ȩ��������
	Test_FinnhubWebData finnhubWebData97(7, _T(""), _T("{\"error\":\"You don't have access to this resource.\"}"));
	// ��ȷ������
	Test_FinnhubWebData finnhubWebData100(10, _T(""), _T("[{\"code2\":\"NR\",\"code3\":\"NRU\",\"codeNo\":\"520\",\"country\":\"Zero\",\"currency\":\"Australian Dollars\",\"currencyCode\":\"AUD\"}, {\"code2\":\"MF\",\"code3\":\"MAF\",\"codeNo\":\"663\",\"country\":\"Saint Martin (French part)\",\"currency\":\"Netherlands Antillean guilder\",\"currencyCode\":\"ANG\"}]"));

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
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvCountry->size(), 0);
			break;
		case 3: // ȱ��CodeNo
			EXPECT_EQ(m_pvCountry->size(), 0);
			break;
		case 4: // �ڶ�������ȱCode2
			EXPECT_EQ(m_pvCountry->size(), 1);
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2.c_str(), _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode.c_str(), _T("AUD"));
			break;
		case 5: // �ڶ�������ȱCodeNo
			EXPECT_EQ(m_pvCountry->size(), 1);
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2.c_str(), _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode.c_str(), _T("AUD"));
			break;
		case 6: // ������
			EXPECT_TRUE(m_pvCountry->empty());
			EXPECT_TRUE(m_finnhubEconomicCountryList.IsVoidData());
			break;
		case 7: // ��Ȩ��������
			EXPECT_TRUE(m_pvCountry->empty());
			EXPECT_TRUE(m_finnhubEconomicCountryList.IsNoRightToAccess());
			break;
		case 10:
			EXPECT_EQ(m_pvCountry->size(), 2);
			EXPECT_STREQ(m_pvCountry->at(1)->m_strCountry.c_str(), _T("Zero")) << "�Թ�����������λ�ڵڶ���λ��";
			EXPECT_STREQ(m_pvCountry->at(1)->m_strCode2.c_str(), _T("NR"));
			EXPECT_STREQ(m_pvCountry->at(1)->m_strCurrencyCode.c_str(), _T("AUD"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCountry.c_str(), _T("Saint Martin (French part)")) << "�Թ�����������λ�ڵ�һ��λ��";
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCode2.c_str(), _T("MF"));
			EXPECT_STREQ(m_pvCountry->at(0)->m_strCurrencyCode.c_str(), _T("ANG"));
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
			if (gl_dataContainerFinnhubCountry.IsCountry(_T("Zero"))) {
				const auto pCountry = gl_dataContainerFinnhubCountry.GetCountry(_T("Zero"));
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
		case 2: // ��ʽ����
			break;
		case 3: // ȱ��CodeNo
			break;
		case 4: // �ڶ�������ȱCode2
			EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), l + 1);
			break;
		case 5: // �ڶ�������ȱCodeNo
			EXPECT_EQ(gl_dataContainerFinnhubCountry.GetTotalCountry(), l + 1);
			break;
		case 6: // ������
			EXPECT_TRUE(m_finnhubEconomicCountryList.IsVoidData());
			break;
		case 7: // ��Ȩ��������
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
