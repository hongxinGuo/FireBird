#pragma once

#include"WebData.h"
#include"AmericaStake.h"

bool ProcessAmericaStakeProfile(CWebDataPtr pWebData);
bool ProcessAmericaStakeSymbol(CWebDataPtr pWebData);
bool ProcessAmericaStakeCandle(CWebDataPtr pWebData, CAmericaStakePtr& pStake);
