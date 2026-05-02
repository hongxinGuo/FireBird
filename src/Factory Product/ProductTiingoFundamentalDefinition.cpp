#include "pch.h"

#include"jsonParse.h"

#include"TiingoFundamentalDefinition.h"
#include "ProductTiingoFundamentalDefinition.h"

#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"
#include "WebData.h"

CProductTiingoFundamentalDefinition::CProductTiingoFundamentalDefinition() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/fundamentals/definitions?";
}

string CProductTiingoFundamentalDefinition::CreateMessage() {
	m_strInquiringSymbol = "All";
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductTiingoFundamentalDefinition::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvFundamentalDefinition = ParseTiingoFundamentalDefinition(pWebData);
	if (!pvFundamentalDefinition->empty()) {
		for (auto& definition : *pvFundamentalDefinition) {
			if (!gl_dataContainerTiingoFundamentalDefinition.HaveDefinition(definition.m_strDataCode)) {
				gl_dataContainerTiingoFundamentalDefinition.Add(definition);
				gl_dataContainerTiingoFundamentalDefinition.SetUpdateDB(true);
			}
		}
	}
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
	auto pvFundamentalDefinition = make_shared<vector<CTiingoFundamentalDefinition>>();
	pvFundamentalDefinition->reserve(100); // 预先分配空间，减少内存重新分配的次数。
	CTiingoFundamentalDefinitionPtr pFundamentalDefinition = nullptr;

	if (!IsValidData(pWebData)) return pvFundamentalDefinition;

	try {
		string s1;
		string_view svJson = pWebData->GetStringView();
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		int iCount = 0;
		for (auto item : doc) {
			auto itemValue = item.value();
			CTiingoFundamentalDefinition fundamentalDefinition;
			s1 = simdjsonGetStringView(itemValue, "dataCode");
			fundamentalDefinition.m_strDataCode = s1;
			s1 = simdjsonGetStringView(itemValue, "name");
			fundamentalDefinition.m_strName = s1;
			s1 = simdjsonGetStringView(itemValue, "description");
			fundamentalDefinition.m_strDescription = s1;
			s1 = simdjsonGetStringView(itemValue, "statementType");
			fundamentalDefinition.m_strStatementType = s1;
			s1 = simdjsonGetStringView(itemValue, "units");
			fundamentalDefinition.m_strUnits = s1;

			pvFundamentalDefinition->push_back(fundamentalDefinition);
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage("Tiingo fundamental definitions ", error.what());
	}

	return pvFundamentalDefinition;
}

void CProductTiingoFundamentalDefinition::UpdateSystemStatus() {
	gl_pTiingoDataSource->SetUpdateFundamentalDefinition(false);
	gl_systemMessage.PushInformationMessage("Fundamental definition updated");
}
