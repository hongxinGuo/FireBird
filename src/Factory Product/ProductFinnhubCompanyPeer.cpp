#include "pch.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"
#include "ContainerFinnhubStock.h"

#include "ProductFinnhubCompanyPeer.h"

#include "FinnhubDataSource.h"
#include "jsonParse.h"
#include"cpr/cpr.h"

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/peers?symbol=";
}

void CProductFinnhubCompanyPeer::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}
		const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
		const nlohmannJson jsonPeer = Parse(m_r.text);
		pStock->SetPeer(jsonPeer);
		pStock->SetPeerUpdateDate(gl_pWorldMarket->GetMarketDate());
		pStock->SetUpdatePeer(false);
		pStock->SetUpdateProfileDB(true);
	}
}

void CProductFinnhubCompanyPeer::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
		break;
	case 401:
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		WebErrorReport();
		break;
	}
}

void CProductFinnhubCompanyPeer::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubCompanyPeer::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);

	m_strInquiringExchange = pStock->GetExchange();
	m_inquiryString = m_strInquiryFunction + pStock->GetSymbol();
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

nlohmannJson CProductFinnhubCompanyPeer::Parse(const string& text) {
	nlohmannJson jsonPeer; // 默认的空状态（没有竞争对手)
	nlohmannJson js;

	if (text.empty()) return jsonPeer;
	if (!::CreateJsonWithNlohmann(js, text)) return jsonPeer;
	if (::IsVoidJson(text)) return jsonPeer; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return jsonPeer;

	return js;
}
