#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubStockDayLine.h"

IMPLEMENT_DYNCREATE(CProductFinnhubStockDayLine, CProductFinnhub)

bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);

CProductFinnhubStockDayLine::CProductFinnhubStockDayLine() {
	m_strClassName = _T("Finnhub stock dayline");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubStockDayLine::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	const auto strMiddle = pStock->GetFinnhubDayLineInquiryString(CVirtualMarket::GetUTCTime());

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	const auto pvDayLine = ParseFinnhubStockCandle2(pWebData);
	pStock->SetDayLineNeedUpdate(false);
	for (const auto& pDayLine : *pvDayLine) {
		pDayLine->SetExchange(pStock->GetExchangeCode());
		pDayLine->SetStockSymbol(pStock->GetSymbol());
		pDayLine->SetDisplaySymbol(pStock->GetTicker());
		const auto lTemp = TransferToDate(pDayLine->m_time, m_pMarket->GetMarketTimeZone());
		pDayLine->SetDate(lTemp);
	}
	if (!pvDayLine->empty()) {
		pStock->UpdateDayLine(*pvDayLine);
		if (pStock->GetDayLineSize() > 0) {
			// 添加了新数据
			pStock->SetDayLineNeedSaving(true);
			pStock->SetUpdateProfileDB(true);
			const long lSize = pStock->GetDayLineSize() - 1;
			const auto pDayLine = pStock->GetDayLine(lSize);
			if (!IsEarlyThen(pDayLine->GetMarketDate(), m_pMarket->GetMarketDate(), 100)) {
				pStock->SetIPOStatus(_STOCK_IPOED_);
			}
			return true;
		}
	}
	//TRACE("处理%s日线数据\n", pStock->GetSymbol().GetBuffer());

	return false;
}

CDayLineVectorPtr CProductFinnhubStockDayLine::ParseFinnhubStockCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	ptree pt2, pt3;
	CDayLinePtr pDayLine = nullptr;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvDayLine;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvDayLine;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvDayLine;
	}

	const auto ppt = pWebData->GetPTree();
	try {
		auto s = ppt->get<string>(_T("s"));
		if (s == _T("no_data")) {
			// 没有日线数据，无需检查此股票的日线和实时数据
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("日线返回值不为ok"));
			return pvDayLine;
		}
	}
	catch (ptree_error& e) {
		// 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle missing 's' ") + GetInquiry(), e);
		return pvDayLine;
	}

	try {
		time_t tTemp;
		pt2 = ppt->get_child(_T("t"));
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			tTemp = pt3.get_value<time_t>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle missing 't' ") + GetInquiry(), e);
		return pvDayLine;
	}
	try {
		int i;
		INT64 llTemp;
		double dTemp;
		pt2 = ppt->get_child(_T("c"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetClose(dTemp * 1000);
		}
		pt2 = ppt->get_child(_T("o"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(dTemp * 1000);
		}
		pt2 = ppt->get_child(_T("h"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetHigh(dTemp * 1000);
		}
		pt2 = ppt->get_child(_T("l"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetLow(dTemp * 1000);
		}
		pt2 = ppt->get_child(_T("v"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			llTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle Error#3 ") + GetInquiry(), e);
	}
	ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。

	return pvDayLine;
}

CDayLineVectorPtr CProductFinnhubStockDayLine::ParseFinnhubStockCandle2(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	json pt2;
	CDayLinePtr pDayLine = nullptr;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvDayLine;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvDayLine;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvDayLine;
	}

	const auto pjs = pWebData->GetJSon();
	try {
		auto s = jsonGetString(pjs, _T("s"));
		if (s == _T("no_data")) {
			// 没有日线数据，无需检查此股票的日线和实时数据
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("日线返回值不为ok"));
			return pvDayLine;
		}
	}
	catch (json::exception& e) {
		// 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle missing 's' ") + GetInquiry(), e.what());
		return pvDayLine;
	}

	try {
		time_t tTemp;
		if (!jsonGetChild(pjs, _T("t"), &pt2)) return pvDayLine;
		for (auto it = pt2.begin(); it != pt2.end(); ++it) {
			tTemp = it->get<time_t>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle missing 't' ") + GetInquiry(), e.what());
		return pvDayLine;
	}
	try {
		int i;
		INT64 llTemp;
		double dTemp;
		if (jsonGetChild(pjs, _T("c"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				dTemp = it->get<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetClose(dTemp * 1000);
			}
		}
		if (jsonGetChild(pjs, _T("o"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				dTemp = it->get<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetOpen(dTemp * 1000);
			}
		}
		if (jsonGetChild(pjs, _T("h"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				dTemp = it->get<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetHigh(dTemp * 1000);
			}
		}
		if (jsonGetChild(pjs, _T("l"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				dTemp = it->get<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetLow(dTemp * 1000);
			}
		}
		if (jsonGetChild(pjs, _T("v"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				llTemp = it->get<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetVolume(llTemp);
			}
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle Error#3 ") + GetInquiry(), e.what());
	}
	ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。

	return pvDayLine;
}
