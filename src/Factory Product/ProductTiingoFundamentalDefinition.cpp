#include "pch.h"

#include"jsonParse.h"

#include"TiingoFundamentalDefinition.h"
#include "ProductTiingoFundamentalDefinition.h"

#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"

CProductTiingoFundamentalDefinition::CProductTiingoFundamentalDefinition() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/fundamentals/definitions?");
}

CString CProductTiingoFundamentalDefinition::CreateMessage() {
	m_strInquiringSymbol = _T("All");
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductTiingoFundamentalDefinition::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvFundamentalDefinition = ParseTiingoFundamentalDefinition(pWebData);
	if (!pvFundamentalDefinition->empty()) {
		for ([[maybe_unused]] const auto& pDefinition : *pvFundamentalDefinition) {
			if (!gl_dataContainerTiingoFundamentalDefinition.HaveDefinition(pDefinition->m_strDataCode)) {
				gl_dataContainerTiingoFundamentalDefinition.Add(pDefinition);
				gl_dataContainerTiingoFundamentalDefinition.SetUpdateDB(true);
			}
		}
	}
	gl_systemConfiguration.DecreaseTiingoBandWidth(pWebData->GetBufferLength());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//[
//	{
//		"dataCode":"liabilitiesCurrent",
//		"name" : "Current Liabilities",
//		"description" : "Debt or liabilities that are due within a year",
//		"statementType" : "balanceSheet",
//		"units" : "$"
//	},
//	{
//		"dataCode":"rps",
//		"name" : "Revenue Per Share",
//		"description" : "Revenue per share",
//		"statementType" : "overview",
//		"units" : "$"
//	}
//]
//
// 使用simdjson解析，速度为Nlohmann-json的三倍。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoFundamentalDefinitionsPtr CProductTiingoFundamentalDefinition::ParseTiingoFundamentalDefinition(const CWebDataPtr& pWebData) {
	auto pvFundamentalDefinition = make_shared<vector<CTiingoFundamentalDefinitionPtr>>();
	CTiingoFundamentalDefinitionPtr pFundamentalDefinition = nullptr;
	string s1;

	if (!IsValidData(pWebData)) return pvFundamentalDefinition;

	try {
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		CString str;
		int iCount = 0;
		for (auto item : doc) {
			auto itemValue = item.value();
			pFundamentalDefinition = make_shared<CTiingoFundamentalDefinition>();
			s1 = jsonGetStringView(itemValue, _T("dataCode"));
			pFundamentalDefinition->m_strDataCode = s1.c_str();;
			s1 = jsonGetStringView(itemValue, _T("name"));
			pFundamentalDefinition->m_strName = s1.c_str();
			s1 = jsonGetStringView(itemValue, _T("description"));
			pFundamentalDefinition->m_strDescription = s1.c_str();;
			s1 = jsonGetStringView(itemValue, _T("statementType"));
			pFundamentalDefinition->m_strStatementType = s1.c_str();
			s1 = jsonGetStringView(itemValue, _T("units"));
			pFundamentalDefinition->m_strUnits = s1.c_str();;

			pvFundamentalDefinition->push_back(pFundamentalDefinition);
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Tiingo fundamental definitions "), error.what());
	}

	return pvFundamentalDefinition;
}

void CProductTiingoFundamentalDefinition::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetUpdateFundamentalDefinition(false);
}
