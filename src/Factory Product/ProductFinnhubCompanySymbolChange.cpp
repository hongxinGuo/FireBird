//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Finnhub经济日历。
///	此信息为premium，使用此信息来判断finnhub账户类型（免费还是付费）
///
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubCompanySymbolChange.h"

#include "FinnhubDataSource.h"
#include "WebData.h"

CProductFinnhubCompanySymbolChange::CProductFinnhubCompanySymbolChange() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/ca/symbol-change?from=2020-01-01&to=2026-12-31&";
}

string CProductFinnhubCompanySymbolChange::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	ASSERT(m_strInquiringExchange == "ALL");
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	return m_strInquiry;
}

void CProductFinnhubCompanySymbolChange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvCompanySymbolChange = ParseFinnhubCompanySymbolChange(pWebData);
	if (pvCompanySymbolChange->empty()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
	}
	else {
		//gl_dataContainerFinnhubCompanySymbolChange.Update(*pvCompanySymbolChange);
	}
}

CCompanySymbolChangesPtr CProductFinnhubCompanySymbolChange::ParseFinnhubCompanySymbolChange(const CWebDataPtr& pWebData) {
	auto pvCompanySymbolChange = make_shared<vector<CCompanySymbolChange>>();
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvCompanySymbolChange;
	if (!IsValidData(pWebData)) return pvCompanySymbolChange;

	try {
		nlohmannJson js2 = jsonGetChild(js, "data");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			CCompanySymbolChange companySymbolChange;
			string s = jsonGetString(it, "atDate");
			companySymbolChange.m_sTime = s;
			s = jsonGetString(it, "newSymbol");
			companySymbolChange.m_sNewSymbol = s;
			s = jsonGetString(it, "oldSymbol");
			companySymbolChange.m_sOldSymbol = s;
			pvCompanySymbolChange->push_back(companySymbolChange);
			companySymbolChange.Reset();
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub company symbol change ", e.what());
	}
	return pvCompanySymbolChange;
}
void CProductFinnhubCompanySymbolChange::UpdateSystemStatus() {
	gl_pFinnhubDataSource->SetUpdateCompanySymbolChange(false);
	gl_systemMessage.PushInformationMessage("Finnhub company symbol change updated");
	if (IsNoRightToAccess()) {// Note 在此确定Finnhub账户类型
		gl_systemConfiguration.ChangeFinnhubAccountTypeToFree();
		gl_systemMessage.PushStockMarketInformationMessage("free finnhub account");
	}
	else {
		gl_systemConfiguration.ChangeFinnhubAccountTypeToPaid();
		gl_systemMessage.PushStockMarketInformationMessage("paid finnhub account");
	}
}
