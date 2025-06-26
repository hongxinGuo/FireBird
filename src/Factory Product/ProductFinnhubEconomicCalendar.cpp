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
#include"JsonGetValue.h"

#include "ProductFinnhubEconomicCalendar.h"

#include "FinnhubDataSource.h"
#include "WebData.h"

CProductFinnhubEconomicCalendar::CProductFinnhubEconomicCalendar() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/calendar/economic?");
}

CString CProductFinnhubEconomicCalendar::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	ASSERT(m_strInquiringExchange.Compare(_T("ALL")) == 0);
	m_strInquiringExchange = _T("ALL"); // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	return m_strInquiry;
}

void CProductFinnhubEconomicCalendar::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvEconomicCalendar = ParseFinnhubEconomicCalendar(pWebData);
	if (pvEconomicCalendar->size() == 0) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
	}
	else {
		gl_dataContainerFinnhubEconomicCalendar.Update(*pvEconomicCalendar);
	}
}

CEconomicCalendarsPtr CProductFinnhubEconomicCalendar::ParseFinnhubEconomicCalendar(const CWebDataPtr& pWebData) {
	auto pvEconomicCalendar = make_shared<vector<CEconomicCalendarPtr>>();
	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	string s;
	json js;

	if (!pWebData->CreateJson(js)) return pvEconomicCalendar;
	if (!IsValidData(pWebData)) return pvEconomicCalendar;

	try {
		json js2 = jsonGetChild(js, _T("economicCalendar"));
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			pEconomicCalendar = make_shared<CEconomicCalendar>();
			s = jsonGetString(it, _T("country"));
			if (!s.empty()) pEconomicCalendar->m_strCountry = s.c_str();
			s = jsonGetString(it,_T("event"));
			pEconomicCalendar->m_strEvent = s;
			s = jsonGetString(it,_T("impact"));
			pEconomicCalendar->m_strImpact = s;
			pEconomicCalendar->m_dEstimate = jsonGetDouble(it, _T("estimate"));
			pEconomicCalendar->m_dActual = jsonGetDouble(it, _T("actual"));
			pEconomicCalendar->m_dPrev = jsonGetDouble(it, _T("prev"));
			s = jsonGetString(it,_T("time"));
			pEconomicCalendar->m_strTime = s;
			s = jsonGetString(it,_T("unit"));
			pEconomicCalendar->m_strUnit = s;
			pvEconomicCalendar->push_back(pEconomicCalendar);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Economic Calendar "), e.what());
	}
	return pvEconomicCalendar;
}
void CProductFinnhubEconomicCalendar::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CFinnhubDataSource")) == 0);

	dynamic_pointer_cast<CFinnhubDataSource>(pDataSource)->SetUpdateEconomicCalendar(false);
	gl_systemMessage.PushInformationMessage(_T("Finnhub economic calendar updated"));
	if (IsNoRightToAccess()) {// Note 在此确定Finnhub账户类型
		gl_systemConfiguration.ChangeFinnhubAccountTypeToFree();
		gl_systemMessage.PushInnerSystemInformationMessage("free finnhub account");
	}
	else {
		gl_systemConfiguration.ChangeFinnhubAccountTypeToPaid();
		gl_systemMessage.PushInnerSystemInformationMessage("paid finnhub account");
	}
}
