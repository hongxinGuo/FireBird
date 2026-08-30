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
#include"SystemMessage.h"

#include "ProductFinnhubEconomicCalendar.h"

#include "ContainerFinnhubEconomicCalendar.h"
#include "FinnhubDataSource.h"
#include "SystemConfiguration.h"
#include"EconomicCalendar.h"
#include"cpr/cpr.h"

using std::make_shared;

CProductFinnhubEconomicCalendar::CProductFinnhubEconomicCalendar() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/calendar/economic?";
}

void CProductFinnhubEconomicCalendar::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		const auto pvEconomicCalendar = Parse(m_r.text);
		if (pvEconomicCalendar->empty()) {
			m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		}
		else {
			gl_dataContainerFinnhubEconomicCalendar.Update(*pvEconomicCalendar);
		}
	}
}

void CProductFinnhubEconomicCalendar::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0: //
		// do nothing
		break;
	case 401: // no right to access
	case 403:
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		string sType = typeid(this).name();
		string s = std::format("{} error. http code: {}, error code:{}, message:{}", sType, r.status_code, static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushErrorMessage(s);
		break;
	}
}

shared_ptr<vector<string>> CProductFinnhubEconomicCalendar::CreateMessage() {
	m_inquiryString = m_strInquiryFunction;
	ABSL_DCHECK(m_strInquiringExchange == "ALL");
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

CEconomicCalendarsPtr CProductFinnhubEconomicCalendar::Parse(const string& text) {
	auto pvEconomicCalendar = make_shared<vector<CEconomicCalendar>>();
	pvEconomicCalendar->reserve(1000);

	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	nlohmannJson js;

	if (text.empty()) return pvEconomicCalendar;
	if (!::CreateJsonWithNlohmann(js, text)) return pvEconomicCalendar;
	if (::IsVoidJson(text)) return pvEconomicCalendar; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return pvEconomicCalendar;

	try {
		nlohmannJson js2 = jsonGetChild(js, "economicCalendar");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			CEconomicCalendar economicCalendar;
			string s = jsonGetString(it, "country");
			if (!s.empty()) economicCalendar.m_strCountry = s;
			s = jsonGetString(it, "event");
			economicCalendar.m_strEvent = s;
			s = jsonGetString(it, "impact");
			economicCalendar.m_strImpact = s;
			economicCalendar.m_dEstimate = jsonGetDouble(it, "estimate");
			economicCalendar.m_dActual = jsonGetDouble(it, "actual");
			economicCalendar.m_dPrev = jsonGetDouble(it, "prev");
			s = jsonGetString(it, "time");
			economicCalendar.m_strTime = s;
			s = jsonGetString(it, "unit");
			economicCalendar.m_strUnit = s;
			pvEconomicCalendar->push_back(economicCalendar);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Economic Calendar ", e.what());
	}
	return pvEconomicCalendar;
}
void CProductFinnhubEconomicCalendar::UpdateSystemStatus() {
	gl_pFinnhubDataSource->SetUpdateEconomicCalendar(false);
	gl_systemMessage.PushInformationMessage("Finnhub economic calendar updated");
	if (IsNoRightToAccess()) {// Note 在此确定Finnhub账户类型
		gl_systemConfiguration.ChangeFinnhubAccountTypeToFree();
		gl_systemMessage.PushStockMarketInformationMessage("free finnhub account");
	}
	else {
		gl_systemConfiguration.ChangeFinnhubAccountTypeToPaid();
		gl_systemMessage.PushStockMarketInformationMessage("paid finnhub account");
	}
}
