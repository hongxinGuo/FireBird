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
#include"VirtualWebInquiry.h"

UINT ThreadReadPotenDailyBriefing(LPVOID pParam) {
  CPotenDailyBriefingWebInquiry* pPotenDailyBriefingWebData = (CPotenDailyBriefingWebInquiry*)(pParam);
  if (pPotenDailyBriefingWebData->ReadWebData(500, 50, 30)) {
    CWebDataPtr pWebDataReceived = pPotenDailyBriefingWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      pWebDataReceived->m_lTime = (INT64)(pPotenDailyBriefingWebData->GetInquiringDay()) * 1000000;
      gl_WebInquirer.PushPotenDailyBriefingData(pWebDataReceived);
    }
  }
  return 5;
}