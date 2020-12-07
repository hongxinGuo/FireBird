#include"stdafx.h"
#include"globedef.h"

#include"ReadString.h"

#include"ProcessCompanyProfile.h"

bool ProcessCompanyProfile(CWebDataPtr pWebData) {
  CCompanyProfilePtr pProfile = nullptr;

  if (pWebData->GetCurrentPosData() != '{') return false;
  while (pWebData->GetCurrentPos() < pWebData->GetBufferLength()) {
    if ((pProfile = ReadOneProfile(pWebData)) != nullptr) {
      if (gl_pAmericaStakeMarket->IsCompanySymbol(pProfile->m_strTicker)) { // 不是新代码？
        gl_pAmericaStakeMarket->AddCompanyProfile(pProfile);
        return true;
      }
    }
    else return false;
  }
  return false;
}

static map<CString, long> s_mapCompanyProfile{ {"country", 0 }, {"currency", 1}, {"exchange", 2},{"finnhubIndustry", 3}, {"ipo", 4},{"logo", 5}, {"marketCapitalization", 6},
                                             {"name", 7}, {"phone", 8}, {"shareOutstanding", 9}, {"ticker", 10}, {"weburl", 11} };

CCompanyProfilePtr ReadOneProfile(CWebDataPtr pWebData) {
  CString strDescription = _T("");
  CString strSymbol;
  CString strDisplaySymbol;
  CString strType;
  CString strCurrency;
  CString strTemp, strTemp2;
  CCompanyProfilePtr pProfile = make_shared<CCompanyProfile>();
  char buffer[1000];
  int i1 = 0;
  long lCurrentPos = pWebData->GetCurrentPos();
  while (pWebData->GetData(lCurrentPos) != '}') buffer[i1++] = pWebData->GetData(lCurrentPos++);
  buffer[i1] = '}';
  buffer[i1 + 1] = 0x000;

  char buffer2[30];
  int i2 = 0;
  bool fFinished = false;

  while (pWebData->GetCurrentPosData() != '{') pWebData->IncreaseCurrentPos();
  pWebData->IncreaseCurrentPos(); // 跨过字符{
  for (int i = 0; i < 12; i++) { // 共五个数据
    strTemp = ReadString(pWebData);
    if (s_mapCompanyProfile.find(strTemp) != s_mapCompanyProfile.end()) {
      switch (s_mapCompanyProfile.at(strTemp)) {
      case 6:
      case 9: // 这两个需要特别处理
      while (pWebData->GetCurrentPosData() != ':') pWebData->IncreaseCurrentPos();
      pWebData->IncreaseCurrentPos();
      i2 = 0;
      while (pWebData->GetCurrentPosData() != ',') {
        buffer2[i2++] = pWebData->GetCurrentPosData();
        pWebData->IncreaseCurrentPos();
      }
      buffer2[i2] = 0x000;
      if (s_mapCompanyProfile.at(strTemp) == 6) {
        pProfile->m_strMarketCapitalization = buffer2;
      }
      else {
        pProfile->m_strShareOutstanding = buffer2;
      }
      break;
      default:
      while (pWebData->GetCurrentPosData() != '"') pWebData->IncreaseCurrentPos();
      strTemp2 = ReadString(pWebData);
      switch (s_mapCompanyProfile.at(strTemp)) {
      case 0:
      pProfile->m_strCountry = strTemp2;
      break;
      case 1:
      pProfile->m_strCurrency = strTemp2;
      break;
      case 2:
      pProfile->m_strExchange = strTemp2;
      break;
      case 3:
      pProfile->m_strFinnHubIndustry = strTemp2;
      break;
      case 4:
      int year, month, day;
      sscanf_s(strTemp2.GetBuffer(), _T("%4d-%2d-%2d"), &year, &month, &day);
      pProfile->m_lIPODate = year * 10000 + month * 100 + day;
      break;
      case 5:
      pProfile->m_strLogo = strTemp2;
      break;
      case 7:
      pProfile->m_strName = strTemp2;
      break;
      case 8:
      pProfile->m_strPhone = strTemp2;
      break;
      case 10:
      pProfile->m_strTicker = strTemp2;
      break;
      case 11:
      pProfile->m_strWebURL = strTemp2;
      fFinished = true;
      break;
      default:
      TRACE("OUT OUT RANGE\n");
      break;
      }
      }
      while ((pWebData->GetCurrentPosData() != '"') && !fFinished) pWebData->IncreaseCurrentPos();
    }
  }
  while (pWebData->GetCurrentPosData() != '}') pWebData->IncreaseCurrentPos();
  pWebData->IncreaseCurrentPos();
  return pProfile;
}