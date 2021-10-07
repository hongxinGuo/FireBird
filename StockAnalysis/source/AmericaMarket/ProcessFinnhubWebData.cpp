#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include"WebData.h"
#include"EconomicCalendar.h"

#include"WorldMarket.h"
#include"CallableFunction.h"

#include"TiingoWebSocketData.h"

using namespace std;
#include<algorithm>
#include<memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//using namespace boost::property_tree;

/// <summary>
/// 高级版的公司简介，需要申请付费账号
/// </summary>
/// <param name="pWebData"></param>
/// <param name="pStock"></param>
/// <returns></returns>
bool CWorldMarket::ProcessFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr& pStock) {
	ptree pt;
	string s;

	if (pWebData->GetBufferLength() < 20) {
		pStock->SetProfileUpdateDate(gl_pWorldMarket->GetFormatedMarketDate());
		pStock->SetUpdateProfileDB(true);
		return false; // 没有公司简介
	}
	if (!ConvertToJSON(pt, pWebData)) return false;
	try {
		s = pt.get<string>(_T("address"));
		pStock->SetAddress(s.c_str());
	}
	catch (ptree_error&) {
		pStock->SetProfileUpdateDate(gl_pWorldMarket->GetFormatedMarketDate());
		pStock->SetUpdateProfileDB(true);
		return false; // 没有公司简介
	}
	s = pt.get<string>(_T("city"));
	pStock->SetCity(s.c_str());
	s = pt.get<string>(_T("country"));
	if (s.size() > 0) pStock->SetCountry(s.c_str());
	s = pt.get<string>(_T("currency"));
	if (s.size() > 0) pStock->SetCurrency(s.c_str());
	s = pt.get<string>(_T("cusip"));
	if (s.size() > 0) pStock->SetCusip(s.c_str());
	s = pt.get<string>(_T("sedol"));
	if (s.size() > 0) pStock->SetSedol(s.c_str());
	s = pt.get<string>(_T("description"));
	if (s.size() > 0) pStock->SetDescription(s.c_str());
	s = pt.get<string>(_T("exchange"));
	if (s.size() > 0) pStock->SetListedExchange(s.c_str());
	s = pt.get<string>(_T("ggroup"));
	if (s.size() > 0) pStock->SetGgroup(s.c_str());
	s = pt.get<string>(_T("gind"));
	if (s.size() > 0) pStock->SetGind(s.c_str());
	s = pt.get<string>(_T("gsector"));
	if (s.size() > 0) pStock->SetGsector(s.c_str());
	s = pt.get<string>(_T("gsubind"));
	if (s.size() > 0) pStock->SetGsubind(s.c_str());
	s = pt.get<string>(_T("ipo"));
	if (s.size() > 0) pStock->SetIPODate(s.c_str());
	s = pt.get<string>(_T("isin"));
	if (s.size() > 0) pStock->SetIsin(s.c_str());
	s = pt.get<string>(_T("marketCapitalization"));
	if (s.size() > 0) pStock->SetMarketCapitalization(atof(s.c_str()));

	s = pt.get<string>(_T("naics"));
	if (s.size() > 0) pStock->SetNaics(s.c_str());
	s = pt.get<string>(_T("naicsNationalIndustry"));
	if (s.size() > 0) pStock->SetNaicsNationalIndustry(s.c_str());
	s = pt.get<string>(_T("naicsSector"));
	if (s.size() > 0) pStock->SetNaicsSector(s.c_str());
	s = pt.get<string>(_T("naicsSubsector"));
	if (s.size() > 0) pStock->SetNaicsSubsector(s.c_str());
	s = pt.get<string>(_T("name"));
	if (s.size() > 0) pStock->SetName(s.c_str());
	s = pt.get<string>(_T("phone"));
	if (s.size() > 0) pStock->SetPhone(s.c_str());

	s = pt.get<string>(_T("shareOutstanding"));
	if (s.size() > 0) pStock->SetShareOutstanding(atof(s.c_str()));
	s = pt.get<string>(_T("state"));
	if (s.size() > 0) pStock->SetState(s.c_str());
	s = pt.get<string>(_T("ticker"));
	if (s.size() > 0) pStock->SetTicker(s.c_str());
	s = pt.get<string>(_T("weburl"));
	if (s.size() > 0) pStock->SetWebURL(s.c_str());

	s = pt.get<string>(_T("logo"));
	if (s.size() > 0) pStock->SetLogo(s.c_str());
	s = pt.get<string>(_T("finnhubIndustry"));
	if (s.size() > 0) pStock->SetFinnhubIndustry(s.c_str());
	pStock->SetProfileUpdateDate(gl_pWorldMarket->GetFormatedMarketDate());
	pStock->SetUpdateProfileDB(true);
	return true;
}

/// <summary>
/// 简版的公司简介，格式如下：
/// "country": "US",
/// "currency": "USD",
/// "exchange" : "NASDAQ/NMS (GLOBAL MARKET)",
/// "ipo" : "1980-12-12",
/// "marketCapitalization" : 1415993,
/// "name" : "Apple Inc",
/// "phone" : "14089961010",
/// "shareOutstanding" : 4375.47998046875,
/// "ticker" : "AAPL",
/// "weburl" : "https://www.apple.com/",
/// "logo" : "https://static.finnhub.io/logo/87cb30d8-80df-11ea-8951-00000000092a.png",
/// "finnhubIndustry" : "Technology"
///
/// </summary>
/// <param name="pWebData"></param>
/// <param name="pStock"></param>
/// <returns></returns>
bool CWorldMarket::ProcessFinnhubStockProfileConcise(CWebDataPtr pWebData, CWorldStockPtr& pStock) {
	ptree pt;
	string s;
	string sError;

	if (pWebData->GetBufferLength() < 30) {
		TRACE("%s无公司简介, 字符串长度为：%d\n", pStock->GetSymbol().GetBuffer(), pWebData->GetBufferLength());
		//正常的长度大致为300左右, 小于100则为无效信息。
		return true; // 没有公司简介也返回任务完成成功。此时应该返回两个字符：{}
	}
	if (!ConvertToJSON(pt, pWebData)) return false;
	try {
		s = pt.get<string>(_T("ticker"));
		if (s.size() > 0) pStock->SetTicker(s.c_str());
		s = pt.get<string>(_T("country"));
		if (s.size() > 0) pStock->SetCountry(s.c_str());
		s = pt.get<string>(_T("currency"));
		if (s.size() > 0) pStock->SetCurrency(s.c_str());
		s = pt.get<string>(_T("exchange"));
		if (s.size() > 0) pStock->SetListedExchange(s.c_str());
		s = pt.get<string>(_T("name"));
		if (s.size() > 0) pStock->SetName(s.c_str());
		s = pt.get<string>(_T("finnhubIndustry"));
		if (s.size() > 0) pStock->SetFinnhubIndustry(s.c_str());
		s = pt.get<string>(_T("logo"));
		if (s.size() > 0) pStock->SetLogo(s.c_str());
		s = pt.get<string>(_T("marketCapitalization"));
		if (s.size() > 0) pStock->SetMarketCapitalization(atof(s.c_str()));
		s = pt.get<string>(_T("phone"));
		if (s.size() > 0) pStock->SetPhone(s.c_str());
		if (s.size() > 0) pStock->SetShareOutstanding(pt.get<double>(_T("shareOutstanding")));
		s = pt.get<string>(_T("weburl"));
		if (s.size() > 0) pStock->SetWebURL(s.c_str());
		s = pt.get<string>(_T("ipo"));
		if (s.size() > 0) pStock->SetIPODate(s.c_str());
	}
	catch (ptree_error&) {
		return false; // 出现错误则返回任务失败
	}
	return true;
}

bool CWorldMarket::ProcessFinnhubStockSymbol(CWebDataPtr pWebData, vector<CWorldStockPtr>& vStock) {
	CWorldStockPtr pStock = make_shared<CWorldStock>();
	ptree pt, pt2;
	string s, sError;

	if (!ConvertToJSON(pt, pWebData)) return false;

	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		pStock = make_shared<CWorldStock>();
		try {
			pt2 = it->second;
			s = pt2.get<string>(_T("currency"));
			if (s.size() > 0) pStock->SetCurrency(s.c_str());
			s = pt2.get<string>(_T("description"));
			if (s.size() > 0) pStock->SetDescription(s.c_str());
			s = pt2.get<string>(_T("displaySymbol"));
			pStock->SetDisplaySymbol(s.c_str());
			s = pt2.get<string>(_T("figi"));
			if (s.size() > 0) pStock->SetFigi(s.c_str());
			s = pt2.get<string>(_T("mic"));
			if (s.size() > 0) pStock->SetMic(s.c_str());
			s = pt2.get<string>(_T("symbol"));
			pStock->SetSymbol(s.c_str());
			s = pt2.get<string>(_T("type"));
			if (s.size() > 0) pStock->SetType(s.c_str());
			vStock.push_back(pStock);
		}
		catch (ptree_error&) {
			TRACE("下载Finnhub Symbol有误\n");
			return false;
		}
	}
	return true;
}

bool CWorldMarket::ProcessFinnhubStockCandle(CWebDataPtr pWebData, CWorldStockPtr& pStock) {
	vector<CDayLinePtr> vDayLine;
	ptree pt, pt2, pt3;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	INT64 llTemp = 0;
	time_t tTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	int i = 0;
	CString str;
	string sError;

	if (!ConvertToJSON(pt, pWebData)) { // 工作线程故障
		str = _T("下载");
		str += pStock->GetSymbol();
		str += _T("日线故障\n");
		TRACE("%s", str.GetBuffer());
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		return false;
	}

	try {
		s = pt.get<string>(_T("s"));
		if (s.compare(_T("no_data")) == 0) { // 没有日线数据，无需检查此股票的日线和实时数据
			pStock->SetUpdateProfileDB(true);
			return true;
		}
		if (s.compare(_T("ok")) != 0) {
			str = _T("下载");
			str += pStock->GetSymbol();
			str += _T("日线返回值不为ok\n");
			TRACE("%s", str.GetBuffer());
			gl_systemMessage.PushInformationMessage(str);
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			return false;
		}
	}
	catch (ptree_error&) { // 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		pStock->SetUpdateProfileDB(true);
		return false;
	}
	try {
		pt2 = pt.get_child(_T("t"));
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			tTemp = pt3.get_value<time_t>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetExchange(pStock->GetExchangeCode());
			pDayLine->SetStockSymbol(pStock->GetSymbol());
			pDayLine->SetDisplaySymbol(pStock->GetTicker());
			pDayLine->SetTime(tTemp);
			lTemp = FormatToDate(tTemp);
			pDayLine->SetDate(lTemp);
			vDayLine.push_back(pDayLine);
		}
	}
	catch (ptree_error&) {
		return false;
	}
	try {
		pt2 = pt.get_child(_T("c"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetClose(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("o"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetOpen(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("h"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetHigh(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("l"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetLow(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("v"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			llTemp = pt3.get_value<INT64>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (ptree_error&) {
	}
	sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate); // 以日期早晚顺序排列。
	pStock->UpdateDayLine(vDayLine);
	pStock->SetDayLineNeedUpdate(false);
	pStock->SetDayLineNeedSaving(true);
	pStock->SetUpdateProfileDB(true);
	return true;
}

bool CWorldMarket::ProcessFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr& pStock) {
	string s;
	ptree pt;
	double dTemp = 0;
	time_t tt = 0;

	if (!ConvertToJSON(pt, pWebData)) {
		return false;
	}
	try {
		dTemp = pt.get<double>(_T("c"));
		pStock->SetNew(dTemp * 1000);
		dTemp = pt.get<double>(_T("h"));
		pStock->SetHigh(dTemp * 1000);
		dTemp = pt.get<double>(_T("l"));
		pStock->SetLow(dTemp * 1000);
		dTemp = pt.get<double>(_T("o"));
		pStock->SetOpen(dTemp * 1000);
		dTemp = pt.get<double>(_T("pc"));
		pStock->SetLastClose(dTemp * 1000);
		tt = pt.get<time_t>(_T("t"));
		pStock->SetTransactionTime(tt);
	}
	catch (ptree_error&) { // 数据格式不对，跳过。
		return false;
	}

	return true;
}

bool CWorldMarket::ProcessFinnhubForexExchange(CWebDataPtr pWebData, vector<CString>& vExchange) {
	ptree pt, pt2;
	string s;
	CString str = _T("");
	string sError;

	if (!ConvertToJSON(pt, pWebData)) return false;

	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		pt2 = it->second;
		s = pt2.get_value<string>();
		str = s.c_str();
		vExchange.push_back(str);
	}

	return true;
}

bool CWorldMarket::ProcessFinnhubForexSymbol(CWebDataPtr pWebData, vector<CForexSymbolPtr>& vForexSymbol) {
	CForexSymbolPtr pSymbol = make_shared<CFinnhubForexSymbol>();
	ptree pt, pt2;
	string s;
	string sError;

	if (!ConvertToJSON(pt, pWebData)) return false;

	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		pSymbol = make_shared<CFinnhubForexSymbol>();
		try {
			pt2 = it->second;
			s = pt2.get<string>(_T("description"));
			if (s.size() > 0) pSymbol->SetDescription(s.c_str());
			s = pt2.get<string>(_T("displaySymbol"));
			pSymbol->SetDisplaySymbol(s.c_str());
			s = pt2.get<string>(_T("symbol"));
			pSymbol->SetSymbol(s.c_str());
			vForexSymbol.push_back(pSymbol);
		}
		catch (ptree_error&) {
			TRACE("Finnhub Forex Symbol Error\n");
			return false;
		}
	}

	return true;
}

bool CWorldMarket::ProcessFinnhubForexCandle(CWebDataPtr pWebData, CForexSymbolPtr& pForexSymbol) {
	vector<CDayLinePtr> vDayLine;
	ptree pt, pt2, pt3;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	INT64 llTemp = 0;
	time_t tTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	int i = 0;
	CString str;
	string sError;

	if (!ConvertToJSON(pt, pWebData)) { // 工作线程故障
		str = _T("下载");
		str += pForexSymbol->GetSymbol();
		str += _T("日线故障\n");
		TRACE("%s", str.GetBuffer());
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		return false;
	}

	try {
		s = pt.get<string>(_T("s"));
		if (s.compare(_T("no_data")) == 0) { // 没有日线数据，无需检查此股票的日线和实时数据
			pForexSymbol->SetIPOStatus(__STOCK_NULL__);
			pForexSymbol->SetUpdateProfileDB(true);
			return true;
		}
		if (s.compare(_T("ok")) != 0) {
			str = _T("下载");
			str += pForexSymbol->GetSymbol();
			str += _T("日线返回值不为ok\n");
			TRACE("%s", str.GetBuffer());
			gl_systemMessage.PushInformationMessage(str);
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			return false;
		}
	}
	catch (ptree_error&) { // 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		pForexSymbol->SetIPOStatus(__STOCK_NULL__);
		pForexSymbol->SetUpdateProfileDB(true);
		return true;
	}
	try {
		pt2 = pt.get_child(_T("t"));
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			tTemp = pt3.get_value<time_t>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetExchange(pForexSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pForexSymbol->GetSymbol());
			pDayLine->SetTime(tTemp);
			lTemp = FormatToDate(tTemp);
			pDayLine->SetDate(lTemp);
			vDayLine.push_back(pDayLine);
		}
	}
	catch (ptree_error&) {
		return false;
	}
	try {
		pt2 = pt.get_child(_T("c"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetClose(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("h"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetHigh(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("l"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetLow(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("o"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetOpen(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("v"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			llTemp = pt3.get_value<INT64>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (ptree_error&) {
		// 有些外汇交易不提供成交量，忽略就可以了
	}
	pForexSymbol->SetIPOStatus(__STOCK_IPOED__);
	sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate);
	pForexSymbol->UpdateDayLine(vDayLine);
	pForexSymbol->SetDayLineNeedUpdate(false);
	pForexSymbol->SetDayLineNeedSaving(true);
	pForexSymbol->SetUpdateProfileDB(true);
	return true;
}

bool CWorldMarket::ProcessFinnhubCryptoExchange(CWebDataPtr pWebData, vector<CString>& vExchange) {
	ptree pt, pt2;
	string s;
	CString str = _T("");
	string sError;

	if (!ConvertToJSON(pt, pWebData)) return false;

	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		pt2 = it->second;
		s = pt2.get_value<string>();
		str = s.c_str();
		vExchange.push_back(str);
	}

	return true;
}

bool CWorldMarket::ProcessFinnhubCryptoSymbol(CWebDataPtr pWebData, vector<CCryptoSymbolPtr>& vCryptoSymbol) {
	CCryptoSymbolPtr pSymbol = make_shared<CFinnhubCryptoSymbol>();
	ptree pt, pt2;
	string s;
	string sError;

	if (!ConvertToJSON(pt, pWebData)) return false;

	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		pSymbol = make_shared<CFinnhubCryptoSymbol>();
		try {
			pt2 = it->second;
			s = pt2.get<string>(_T("description"));
			if (s.size() > 0) pSymbol->SetDescription(s.c_str());
			s = pt2.get<string>(_T("displaySymbol"));
			pSymbol->SetDisplaySymbol(s.c_str());
			s = pt2.get<string>(_T("symbol"));
			pSymbol->SetSymbol(s.c_str());
			vCryptoSymbol.push_back(pSymbol);
		}
		catch (ptree_error&) {
			TRACE("Finnhub Crypto Symbol Error\n");
			return false;
		}
	}

	return true;
}

bool CWorldMarket::ProcessFinnhubCryptoCandle(CWebDataPtr pWebData, CCryptoSymbolPtr& pCryptoSymbol) {
	vector<CDayLinePtr> vDayLine;
	ptree pt, pt2, pt3;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	INT64 llTemp = 0;
	time_t tTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	int i = 0;
	CString str;
	string sError;

	if (!ConvertToJSON(pt, pWebData)) { // 工作线程故障
		str = _T("下载");
		str += pCryptoSymbol->GetSymbol();
		str += _T("日线故障\n");
		TRACE("%s", str.GetBuffer());
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		return false;
	}

	try {
		s = pt.get<string>(_T("s"));
		if (s.compare(_T("no_data")) == 0) { // 没有日线数据，无需检查此股票的日线和实时数据
			pCryptoSymbol->SetIPOStatus(__STOCK_NULL__);
			pCryptoSymbol->SetUpdateProfileDB(true);
			return true;
		}
		if (s.compare(_T("ok")) != 0) {
			str = _T("下载");
			str += pCryptoSymbol->GetSymbol();
			str += _T("日线返回值不为ok\n");
			TRACE("%s", str.GetBuffer());
			gl_systemMessage.PushInformationMessage(str);
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			return false;
		}
	}
	catch (ptree_error&) { // 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		pCryptoSymbol->SetIPOStatus(__STOCK_NULL__);
		pCryptoSymbol->SetUpdateProfileDB(true);
		return true;
	}
	try {
		pt2 = pt.get_child(_T("t"));
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			tTemp = pt3.get_value<time_t>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetExchange(pCryptoSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pCryptoSymbol->GetSymbol());
			pDayLine->SetTime(tTemp);
			lTemp = FormatToDate(tTemp);
			pDayLine->SetDate(lTemp);
			vDayLine.push_back(pDayLine);
		}
	}
	catch (ptree_error&) {
		return false;
	}
	try {
		pt2 = pt.get_child(_T("c"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetClose(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("h"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetHigh(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("l"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetLow(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("o"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetOpen(dTemp * 1000);
		}
	}
	catch (ptree_error&) {
	}
	try {
		pt2 = pt.get_child(_T("v"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			llTemp = pt3.get_value<INT64>();
			pDayLine = vDayLine.at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (ptree_error&) {
		// 有些外汇交易不提供成交量，忽略就可以了
	}
	pCryptoSymbol->SetIPOStatus(__STOCK_IPOED__);
	sort(vDayLine.begin(), vDayLine.end(), CompareDayLineDate);
	pCryptoSymbol->UpdateDayLine(vDayLine);
	pCryptoSymbol->SetDayLineNeedUpdate(false);
	pCryptoSymbol->SetDayLineNeedSaving(true);
	pCryptoSymbol->SetUpdateProfileDB(true);
	return true;
}

bool CWorldMarket::ProcessFinnhubCountryList(CWebDataPtr pWebData, vector<CCountryPtr>& vCountry) {
	CCountryPtr pCountry = nullptr;
	ptree pt, pt2;
	string s;

	if (!ConvertToJSON(pt, pWebData)) return false;

	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		pCountry = make_shared<CCountry>();
		pt2 = it->second;
		try {
			s = pt2.get<string>(_T("code2"));
			if (s.size() > 0) pCountry->m_strCode2 = s.c_str();
			s = pt2.get<string>(_T("code3"));
			pCountry->m_strCode3 = s.c_str();
			s = pt2.get<string>(_T("codeNo"));
			pCountry->m_strCodeNo = s.c_str();
			s = pt2.get<string>(_T("country"));
			pCountry->m_strCountry = s.c_str();
			s = pt2.get<string>(_T("currency"));
			pCountry->m_strCurrency = s.c_str();
			s = pt2.get<string>(_T("currencyCode"));
			pCountry->m_strCurrencyCode = s.c_str();
		}
		catch (ptree_error&) {
			return false;
		}
		vCountry.push_back(pCountry);
	}
	sort(vCountry.begin(), vCountry.end(), CompareCountryList);
	return true;
}

bool CWorldMarket::ProcessFinnhubStockPeer(CWebDataPtr pWebData, CWorldStockPtr& pStock) {
	char buffer[1000]{};
	int i = 0;
	ptree pt;
	string sError;

	if (pWebData->GetBufferLength() <= 3) {
		pStock->SetPeer(_T(" ")); // 清空
		return true; // 没有有效的同业竞争对手
	}
	if (!ConvertToJSON(pt, pWebData)) return false;

	ASSERT(pWebData->GetBufferLength() < 1000);
	for (i = 0; i < pWebData->GetBufferLength(); i++) {
		buffer[i] = pWebData->GetData(i);
	}
	if (i > 200) {
		buffer[200] = 0x000;
	}
	else {
		buffer[pWebData->GetBufferLength()] = 0x000;
	}
	pStock->SetPeer(buffer);

	return true;
}

bool CWorldMarket::ProcessFinnhubStockInsiderTransaction(CWebDataPtr pWebData, vector<CInsiderTransactionPtr>& vInsiderTransaction) {
	char buffer[1000]{};
	ptree pt, pt1, pt2;
	string sError;
	string s;
	string stockSymbol;
	long year, month, day;
	CInsiderTransactionPtr pInsiderTransaction = nullptr;

	if (!ConvertToJSON(pt, pWebData)) return false;

	try {
		pt1 = pt.get_child(_T("data"));
		stockSymbol = pt.get<string>(_T("symbol"));
	}
	catch (ptree_error&) {
		return false;
	}
	for (ptree::iterator it = pt1.begin(); it != pt1.end(); ++it) {
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->m_strSymbol = stockSymbol.c_str();
		pt2 = it->second;
		try {
			s = pt2.get<string>(_T("name"));
			if (s.size() > 0) pInsiderTransaction->m_strPersonName = s.c_str();
			pInsiderTransaction->m_lShare = pt2.get<long>(_T("share"));
			pInsiderTransaction->m_lChange = pt2.get<long>(_T("change"));
			s = pt2.get<string>(_T("filingDate"));
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lFilingDate = year * 10000 + month * 100 + day;
			s = pt2.get<string>(_T("transactionDate"));
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lTransactionDate = year * 10000 + month * 100 + day;
			s = pt2.get<string>(_T("transactionCode"));
			pInsiderTransaction->m_strTransactionCode = s.c_str();
			pInsiderTransaction->m_dTransactionPrice = pt2.get<double>(_T("transactionPrice"));
		}
		catch (ptree_error&) {
			return false;
		}
		vInsiderTransaction.push_back(pInsiderTransaction);
	}
	sort(vInsiderTransaction.begin(), vInsiderTransaction.end(), CompareInsiderTransaction);

	return true;
}

bool CWorldMarket::ProcessFinnhubEconomicCalendar(CWebDataPtr pWebData, vector<CEconomicCalendarPtr>& vEconomicCalendar) {
	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	ptree pt, pt1, pt2;
	string s;

	if (!ConvertToJSON(pt, pWebData)) return false;

	try {
		pt1 = pt.get_child(_T("economicCalendar"));
	}
	catch (ptree_error&) {
		return false;
	}
	for (ptree::iterator it = pt1.begin(); it != pt1.end(); ++it) {
		pEconomicCalendar = make_shared<CEconomicCalendar>();
		pt2 = it->second;
		try {
			s = pt2.get<string>(_T("country"));
			if (s.size() > 0) pEconomicCalendar->m_strCountry = s.c_str();
			s = pt2.get<string>(_T("event"));
			pEconomicCalendar->m_strEvent = s.c_str();
			s = pt2.get<string>(_T("impact"));
			pEconomicCalendar->m_strImpact = s.c_str();
			pEconomicCalendar->m_dEstimate = pt2.get<double>(_T("estimate"));
			pEconomicCalendar->m_dActual = pt2.get<double>(_T("actual"));
			pEconomicCalendar->m_dPrev = pt2.get<double>(_T("prev"));
			s = pt2.get<string>(_T("time"));
			pEconomicCalendar->m_strTime = s.c_str();
			s = pt2.get<string>(_T("unit"));
			pEconomicCalendar->m_strUnit = s.c_str();
		}
		catch (ptree_error&) {
			return false;
		}
		vEconomicCalendar.push_back(pEconomicCalendar);
	}
	return true;
}

bool CWorldMarket::ProcessFinnhubEPSSurprise(CWebDataPtr pWebData, vector<CEPSSurprisePtr>& vEPSSurprise) {
	ptree pt, pt2;
	string s;
	CEPSSurprisePtr pEPSSurprise = nullptr;
	long year = 0, month = 0, day = 0;
	CString str;
	string sError;

	if (!ConvertToJSON(pt, pWebData)) return false;

	for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
		pEPSSurprise = make_shared<CEPSSurprise>();
		pt2 = it->second;
		try {
			s = pt2.get<string>(_T("symbol"));
			pEPSSurprise->m_strSymbol = s.c_str();
			s = pt2.get<string>(_T("period"));
			str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pEPSSurprise->m_lDate = year * 10000 + month * 100 + day;
			pEPSSurprise->m_dEstimate = pt2.get<double>(_T("estimate"));
			pEPSSurprise->m_dActual = pt2.get<double>(_T("actual"));
		}
		catch (ptree_error&) {
			return false;
		}
		vEPSSurprise.push_back(pEPSSurprise);
	}
	sort(vEPSSurprise.begin(), vEPSSurprise.end(), CompareEPSSurprise); // 以日期早晚顺序排列。
	return true;
}

/// <summary>
/// 格式为：{"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
///        {"type":"ping"}
///        {"msg":"Subscribing to too many symbols","type":"error"}
/// 目前就这三种。
/// </summary>
/// <param name="pData"></param>
/// <returns></returns>
bool CWorldMarket::ProcessOneFinnhubWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3;
	string sType, sSymbol, sMessage;
	CString strMessage;
	double price = 0;
	double volume = 0;
	time_t time = 0;
	string code;
	CFinnhubWebSocketDataPtr pFinnhubDataPtr = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("type"));
			if (sType.compare(_T("trade")) == 0) { // 交易数据
				pt2 = pt.get_child(_T("data"));
				for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
					pt3 = it->second;
					pFinnhubDataPtr = make_shared<CFinnhubWebSocketData>();
					sSymbol = pt3.get<string>(_T("s"));
					pFinnhubDataPtr->m_strSymbol = sSymbol.c_str();
					code = pt3.get<string>(_T("c"));
					if (code.compare(_T("null")) == 0) code = _T("");
					pFinnhubDataPtr->m_strCode = code.c_str();
					pFinnhubDataPtr->m_dLastPrice = pt3.get<double>(_T("p"));
					pFinnhubDataPtr->m_dLastVolume = pt3.get<double>(_T("v"));
					pFinnhubDataPtr->m_iSeconds = pt3.get<time_t>(_T("t"));
					m_qFinnhubWebSocketData.push(pFinnhubDataPtr);
				}
			}
			else if (sType.compare(_T("ping")) == 0) { // ping  {\"type\":\"ping\"}
				// do nothing
			}
			else if (sType.compare(_T("error")) == 0) { // ERROR {\"msg\":\"Subscribing to too many symbols\",\"type\":\"error\"}
				sMessage = pt.get<string>(_T("msg"));
				strMessage = _T("Finnhub WebSocket error message: ");
				strMessage += sMessage.c_str();
				gl_systemMessage.PushInnerSystemInformationMessage(strMessage);
				return false;
			}
			else {
				// ERROR
				return false;
			}
		}
		else {
			return false;
		}
	}
	catch (ptree_error&) {
		return false;
	}

	return true;
}