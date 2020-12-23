#pragma once

#include"WebData.h"
#include"AmericaStake.h"
#include"ForexSymbol.h"
#include"Country.h"
#include"EPSSurprise.h"

bool ProcessFinnhubStockProfile(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessFinnhubStockProfile2(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessFinnhubStockSymbol(CWebDataPtr pWebData, vector<CAmericaStakePtr>& vStake);
bool ProcessFinnhubStockCandle(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessFinnhubStockQuote(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessFinnhubForexExchange(CWebDataPtr pWebData, vector<CString>& vExchange);
bool ProcessFinnhubForexSymbol(CWebDataPtr pWebData, vector<CForexSymbolPtr>& vForexSymbol);
bool ProcessFinnhubForexCandle(CWebDataPtr pWebData, CForexSymbolPtr& pForexSymbol);
bool ProcessFinnhubCountryList(CWebDataPtr pWebData, vector<CCountryPtr>& vCountry);
bool ProcessFinnhubStockPeer(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessFinnhubEconomicCalendar(CWebDataPtr pWebData, vector<CEconomicCalendarPtr>& m_vEconomicCalendar);
bool ProcessFinnhubEPSSurprise(CWebDataPtr pWebData, vector<CEPSSurprisePtr>& vEPSSurprise);
