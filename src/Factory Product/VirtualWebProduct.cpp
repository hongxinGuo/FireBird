#include"pch.h"

#include"VirtualWebProduct.h"
#include"DayLine.h"
#include "SystemMessage.h"
#include"log.h"

[[nodiscard]] bool CompareDayLineDate(const CDayLine& p1, const CDayLine& p2) {
	return p1.GetDate() < p2.GetDate();
}

bool CVirtualWebProduct::IsVoidJson(const string& text) {
	if (text == "{}" || text == "[]") {
		m_iReceivedDataStatus = VOID_DATA_;
		return true;
	}
	return false;
}

void CVirtualWebProduct::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0: //
		// do nothing
		break;
	case 200: // OK
		break;
	default:
		ReportWebError();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 检查网站是否允许申请该类数据，不允许的话将交易所加入禁入名单。
// 如果是美国交易所不允许的话，需要连续10次不允许后才将美国交易所添加进禁入名单。
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualWebProduct::CheckInaccessible() {
	static int s_iCounter = 0; // 当交易所为美国时，使用此计数器来判断是否将美国交易所添加进限制名单

	if (!IsNoRightToAccess()) {
		s_iCounter = 0;
		return false;
	}
	if (IsUSMarket()) {
		if (s_iCounter++ < 100) { // 当美国市场连续出现100次无权访问数据时，则不再查询
			return false;
		}
	}
	AddInaccessibleSymbol();
	s_iCounter = 0;

	return true;
}

bool CVirtualWebProduct::IsUSMarket() const {
	if (m_strInquiringExchange == "US") return true;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 需要使用typeid(*this).name()来获取实际调用的派生类名，因为此函数是虚函数，可能会被派生类覆盖。
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualWebProduct::ReportWebError() {
	string s = std::format("{} http code:{}, CPR error code:{} CPR message: {}",
	                       typeid(*this).name(), m_r.status_code, static_cast<int>(m_r.error.code), m_r.error.message);
	gl_dailyLogger->info("{}", s);
	gl_systemMessage.PushWebInformationMessage(s);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 需要使用typeid(*this).name()来获取实际调用的派生类名，因为此函数是虚函数，可能会被派生类覆盖。
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualWebProduct::ReportWebError(const string& symbol) {
	string httpCode;
	if (gl_mapHTTPStatusCode.contains(m_r.status_code)) {
		httpCode = gl_mapHTTPStatusCode.at(m_r.status_code);
	}
	string s = std::format("{} stock:{} http code:{}({}), CPR error code:{} CPR message: {}",
	                       typeid(*this).name(), symbol, m_r.status_code, httpCode, static_cast<int>(m_r.error.code), m_r.error.message);
	gl_dailyLogger->info("{}", s);
	gl_systemMessage.PushWebInformationMessage(s);
}
