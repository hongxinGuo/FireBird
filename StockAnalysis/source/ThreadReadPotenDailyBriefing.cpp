/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Poten.com����ӿڶ�ȡ�̡߳�
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
    CWebDataReceivedPtr pWebDataReceived = pPotenDailyBriefingWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebDataInquirer.PushPotenDailyBriefingData(pWebDataReceived);
    }
  }

  return 5;
}