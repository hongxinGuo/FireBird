#include"Market.h"
#include"Thread.h"
#include"WebData.h"

UINT ThreadProcessWebData(LPVOID pParam) {
  CWebData* pWebData = (CWebData*)pParam;

  pWebData->GetDataByUsingThread();

  return 15;
}