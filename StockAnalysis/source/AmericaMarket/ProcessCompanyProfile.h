#pragma once

#include"WebData.h"
#include"CompanyProFile.h"

bool ProcessCompanyProfile(CWebDataPtr pWebData);
bool ProcessCompanySymbol(CWebDataPtr pWebData);
bool ProcessStakeCandle(CWebDataPtr pWebData, CCompanyProfilePtr& pCompany);
