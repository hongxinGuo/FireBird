#pragma once

#include"WebData.h"
#include"CompanySymbol.h"

CCompanySymbolPtr ReadOneSymbol(CWebDataPtr pWebData);
bool ProcessCompanySymbolData(CWebDataPtr pWebData);
