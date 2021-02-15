/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Poten.com����ӿڶ�ȡ�̡߳�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include"ChinaMarket.h"
#include "Thread.h"
#include"VirtualWebInquiry.h"
#include"WebInquirer.h"

UINT ThreadReadPotenDailyBriefing(not_null<CPotenDailyBriefingWebInquiry*> pPotenDailyBriefingWebInquiry) {
  gl_ThreadStatus.IncreaseRunningThread();
  if (pPotenDailyBriefingWebInquiry->ReadWebData()) {
    CWebDataPtr pWebDataReceived = pPotenDailyBriefingWebInquiry->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      pWebDataReceived->SetTime(static_cast<INT64>(pPotenDailyBriefingWebInquiry->GetInquiringDate()) * 1000000);
      gl_WebInquirer.PushPotenDailyBriefingData(pWebDataReceived);
    }
  }
  pPotenDailyBriefingWebInquiry->SetReadingWebData(false);
  gl_ThreadStatus.DecreaseRunningThread();

  return 6;
}