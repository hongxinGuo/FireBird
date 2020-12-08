#include"ProcessStakeCandle.h"
#include"ReadString.h"

#include"DayLine.h"

using namespace std;
#include<map>

static map<CString, long> s_mapCompanyCandle{ {"c", 0 }, {"h", 1}, {"l", 2},{"o", 3}, {"s", 4},{"c", 5}, {"v", 6} };

bool ProcessStakeCandle(CWebDataPtr pWebData) {
  vector<CDayLinePtr> m_vDayLine;
  CString strIndex;

  if (pWebData->GetCurrentPosData() != '{') return false;
  pWebData->IncreaseCurrentPos();
  while (pWebData->GetCurrentPos() < pWebData->GetBufferLength()) {
    strIndex = ReadString(pWebData, ',');
    switch (s_mapCompanyCandle.at(strIndex)) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    break;
    default:
    break;
    }
  }

  return true;
}