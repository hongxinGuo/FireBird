#include "pch.h"

#include"jsonParse.h"

#include"TiingoFundamentalDefinition.h"
#include "ProductTiingoFundamentalDefinition.h"

#include "ContainerTiingoFundamentalDefinition.h"
#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"
#include "SystemMessage.h"

#include"cpr/cpr.h"

using std::make_shared;

CProductTiingoFundamentalDefinition::CProductTiingoFundamentalDefinition() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/fundamentals/definitions?";
}

void CProductTiingoFundamentalDefinition::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string s = inquiry + "&token=" + gl_pTiingoDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ s });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
		}
		const auto pvFundamentalDefinition = Parse(m_r.text);
		if (!pvFundamentalDefinition->empty()) {
			for (auto& definition : *pvFundamentalDefinition) {
				if (!gl_dataContainerTiingoFundamentalDefinition.HaveDefinition(definition.m_strDataCode)) {
					gl_dataContainerTiingoFundamentalDefinition.Add(definition);
					gl_dataContainerTiingoFundamentalDefinition.SetUpdateDB(true);
				}
			}
		}
	}
}

void CProductTiingoFundamentalDefinition::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		break;
	default:
		break;
	}
}

shared_ptr<vector<string>> CProductTiingoFundamentalDefinition::CreateMessage() {
	m_strInquiringSymbol = "All";
	m_inquiryString = m_strInquiryFunction;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
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
CTiingoFundamentalDefinitionsPtr CProductTiingoFundamentalDefinition::Parse(const string& text) {
	auto pvFundamentalDefinition = make_shared<vector<CTiingoFundamentalDefinition>>();
	pvFundamentalDefinition->reserve(100); // 预先分配空间，减少内存重新分配的次数。
	CTiingoFundamentalDefinitionPtr pFundamentalDefinition = nullptr;

	if (::IsVoidJson(text)) return pvFundamentalDefinition;
	if (IsNoRightToAccess()) return pvFundamentalDefinition;

	try {
		string s1;
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(text);
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
