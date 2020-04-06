/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Poten.com行情接口读取线程。
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include"ChinaMarket.h"
#include "Thread.h"
#include"VirtualWebInquiry.h"
#include"WebInquirer.h"

UINT ThreadReadPotenDailyBriefing(CPotenDailyBriefingWebInquiry* pPotenDailyBriefingWebInquiry) {
  gl_ThreadStatus.IncreaseNumberOfRunningThread();
  if (pPotenDailyBriefingWebInquiry->ReadWebData(500, 50, 30)) {
    CWebDataPtr pWebDataReceived = pPotenDailyBriefingWebInquiry->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      pWebDataReceived->m_lTime = (INT64)(pPotenDailyBriefingWebInquiry->GetInquiringDay()) * 1000000;
      gl_WebInquirer.PushPotenDailyBriefingData(pWebDataReceived);
    }
  }
  gl_ThreadStatus.DecreaseNumberOfRunningThread();

  return 6;
}