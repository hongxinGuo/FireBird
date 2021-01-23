#pragma once

#include"WebData.h"
#include"WorldStock.h"

bool ProcessTiingoStockSymbol(CWebDataPtr pWebData, vector<CWorldStockPtr>& vStake);
bool ProcessTiingoStockDayLine(CWebDataPtr pWebData, CWorldStockPtr& pStock);
