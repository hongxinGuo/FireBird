#pragma once

#include"WebData.h"
#include"AmericaStake.h"

bool ProcessAmericaStakeProfile(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessAmericaStakeProfile2(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessAmericaStakeSymbol(CWebDataPtr pWebData);
bool ProcessAmericaStakeCandle(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessAmericaStakeQuote(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
bool ProcessAemricaForexExchange(CWebDataPtr pWebData, vector<CString>& vExchange);
