#pragma once

#include"WebData.h"
#include"AmericaStake.h"

bool ProcessTiingoStockSymbol(CWebDataPtr pWebData, vector<CAmericaStakePtr>& vStake);
bool ProcessTiingoStockDayLine(CWebDataPtr pWebData, CAmericaStakePtr& pStock);
