#pragma once

#include"WebData.h"
#include"WorldStock.h"
#include"FinnhubForexSymbol.h"
#include"Country.h"
#include"EPSSurprise.h"

bool ProcessFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr& pStock);
bool ProcessFinnhubStockProfile2(CWebDataPtr pWebData, CWorldStockPtr& pStock);
bool ProcessFinnhubStockSymbol(CWebDataPtr pWebData, vector<CWorldStockPtr>& vStake);
bool ProcessFinnhubStockCandle(CWebDataPtr pWebData, CWorldStockPtr& pStock);
bool ProcessFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr& pStock);
bool ProcessFinnhubForexExchange(CWebDataPtr pWebData, vector<CString>& vExchange);
bool ProcessFinnhubForexSymbol(CWebDataPtr pWebData, vector<CForexSymbolPtr>& vForexSymbol);
bool ProcessFinnhubForexCandle(CWebDataPtr pWebData, CForexSymbolPtr& pForexSymbol);
bool ProcessFinnhubCountryList(CWebDataPtr pWebData, vector<CCountryPtr>& vCountry);
bool ProcessFinnhubStockPeer(CWebDataPtr pWebData, CWorldStockPtr& pStock);
bool ProcessFinnhubEconomicCalendar(CWebDataPtr pWebData, vector<CEconomicCalendarPtr>& m_vEconomicCalendar);
bool ProcessFinnhubEPSSurprise(CWebDataPtr pWebData, vector<CEPSSurprisePtr>& vEPSSurprise);
