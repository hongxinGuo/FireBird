/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Poten.com行情接口读取线程。
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"ChinaMarket.h"
#include "Thread.h"
#include"WebData.h"

UINT ThreadReadPotenDailyBriefing(LPVOID pParam) {
  CPotenDailyBriefingWebData* pPotenDailyBriefingWebData = (CPotenDailyBriefingWebData*)(pParam);
  if (pPotenDailyBriefingWebData->ReadWebData(500, 50, 30)) {
    if (pPotenDailyBriefingWebData->GetByteReaded() > 40 * 1024) { // 从poten.com读取的数据大小如果低于30KB时，其没有实际内容，无需处理
      CWebDataReceivedPtr pWebDataReceived = pPotenDailyBriefingWebData->TransferWebDataToQueueData();
      if (pWebDataReceived != nullptr) {
        pWebDataReceived->m_lTime = (INT64)(pPotenDailyBriefingWebData->GetInquiringDay()) * 1000000;
        gl_WebDataInquirer.PushPotenDailyBriefingData(pWebDataReceived);
      }
    }
  }

  return 5;
}