#pragma once

#include"WebData.h"
#include"AmericaStake.h"
#include"ForexSymbol.h"

bool ProcessAmericaStakeProfile(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessAmericaStakeProfile2(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessAmericaStakeSymbol(CWebDataPtr pWebData);
bool ProcessAmericaStakeCandle(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessAmericaStakeQuote(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessAmericaForexExchange(CWebDataPtr pWebData, vector<CString>& vExchange);
bool ProcessAmericaForexSymbol(CWebDataPtr pWebData, vector<CForexSymbolPtr>& vForexSymbol);
bool ProcessForexCandle(CWebDataPtr pWebData, CForexSymbolPtr& pForexSymbol);
