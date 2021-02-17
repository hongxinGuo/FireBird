#pragma once

#include"WebData.h"
#include"WorldStock.h"

bool ProcessTiingoStockSymbol(CWebDataPtr pWebData, vector<CWorldStockPtr>& vStock);
bool ProcessTiingoStockDayLine(CWebDataPtr pWebData, CWorldStockPtr& pStock);
