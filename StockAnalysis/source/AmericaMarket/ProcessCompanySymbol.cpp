#include"stdafx.h"
#include"globedef.h"

#include"ReadString.h"

#include"ProcessCompanySymbol.h"

bool ProcessCompanySymbolData(CWebDataPtr pWebData) {
  CCompanySymbolPtr pSymbol = nullptr;

  if (pWebData->GetCurrentPosData() != '[') return false;
  pWebData->IncreaseCurrentPos();
  while (pWebData->GetCurrentPos() < pWebData->GetBufferLength()) {
    if ((pSymbol = ReadOneSymbol(pWebData)) != nullptr) {
      if (!gl_pAmericaStakeMarket->IsCompanySymbol(pSymbol->m_strSymbol)) { // 新代码？
        gl_pAmericaStakeMarket->AddCompanySymbol(pSymbol);
      }
    }
    else return false;
    if (!(pWebData->GetCurrentPosData() == ',')) { // 读完了？
      if (pWebData->GetCurrentPosData() == ']') return true; // 读完了
    }
    else {
      pWebData->IncreaseCurrentPos();
    }
  }
  return false;
}

static map<CString, long> s_mapCompanySymbol{ {"description", 0 }, {"displaySymbol", 1}, {"symbol", 2}, {"type", 3}, {"currency", 4} };

CCompanySymbolPtr ReadOneSymbol(CWebDataPtr pWebData) {
  CString strDescription = _T("");
  CString strSymbol;
  CString strDisplaySymbol;
  CString strType;
  CString strCurrency;
  CString strTemp, strTemp2;
  CCompanySymbolPtr pSymbol = make_shared<CCompanySymbol>();
  char buffer[1000];
  int i1 = 0;
  long lCurrentPos = pWebData->GetCurrentPos();
  while (pWebData->GetData(lCurrentPos) != '}') buffer[i1++] = pWebData->GetData(lCurrentPos++);
  buffer[i1] = '}';
  buffer[i1 + 1] = 0x000;

  while (pWebData->GetCurrentPosData() != '{') pWebData->IncreaseCurrentPos();
  pWebData->IncreaseCurrentPos(); // 跨过字符{
  for (int i = 0; i < 5; i++) { // 共五个数据
    strTemp = ReadString(pWebData);
    if (s_mapCompanySymbol.find(strTemp) != s_mapCompanySymbol.end()) {
      while (pWebData->GetCurrentPosData() != '"') pWebData->IncreaseCurrentPos();
      strTemp2 = ReadString(pWebData);
      switch (s_mapCompanySymbol.at(strTemp)) {
      case 0:
      pSymbol->m_strDescription = strTemp2;
      break;
      case 1:
      pSymbol->m_strDisplaySymbol = strTemp2;
      break;
      case 2:
      pSymbol->m_strSymbol = strTemp2;
      break;
      case 3:
      pSymbol->m_strType = strTemp2;
      break;
      case 4:
      pSymbol->m_strCurrency = strTemp2;
      break;
      default:
      break;
      }
      while (pWebData->GetCurrentPosData() != '"') pWebData->IncreaseCurrentPos();
    }
  }
  while (pWebData->GetCurrentPosData() != '}') pWebData->IncreaseCurrentPos();
  pWebData->IncreaseCurrentPos();
  return pSymbol;
}