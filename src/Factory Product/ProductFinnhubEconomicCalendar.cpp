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

#include "ProductFinnhubEconomicCalendar.h"

#include "FinnhubDataSource.h"
#include "WebData.h"

CProductFinnhubEconomicCalendar::CProductFinnhubEconomicCalendar() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/calendar/economic?";
}

string CProductFinnhubEconomicCalendar::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	ASSERT(m_strInquiringExchange == "ALL");
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	return m_strInquiry;
}

void CProductFinnhubEconomicCalendar::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvEconomicCalendar = ParseFinnhubEconomicCalendar(pWebData);
	if (pvEconomicCalendar->empty()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
	}
	else {
		gl_dataContainerFinnhubEconomicCalendar.Update(*pvEconomicCalendar);
	}
}

CEconomicCalendarsPtr CProductFinnhubEconomicCalendar::ParseFinnhubEconomicCalendar(const CWebDataPtr& pWebData) {
	auto pvEconomicCalendar = make_shared<vector<CEconomicCalendar>>();
	pvEconomicCalendar->reserve(1000);

	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvEconomicCalendar;
	if (!IsValidData(pWebData)) return pvEconomicCalendar;

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
