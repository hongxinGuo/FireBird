/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Poten.com行情接口读取线程。
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "Thread.h"
#include"VirtualWebInquiry.h"
#include"WebInquirer.h"

UINT ThreadReadPotenDailyBriefing(not_null<CPotenDailyBriefingWebInquiry*> pPotenDailyBriefingWebInquiry) {
  if (pPotenDailyBriefingWebInquiry->ReadWebData()) {
    CWebDataPtr pWebDataReceived = pPotenDailyBriefingWebInquiry->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      pWebDataReceived->SetTime(static_cast<INT64>(pPotenDailyBriefingWebInquiry->GetInquiringDate()) * 1000000);
      gl_WebInquirer.PushPotenDailyBriefingData(pWebDataReceived);
    }
  }
  pPotenDailyBriefingWebInquiry->SetReadingWebData(false);

  return 6;
}