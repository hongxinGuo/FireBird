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
    if (pPotenDailyBriefingWebData->GetByteReaded() > 40 * 1024) { // ��poten.com��ȡ�����ݴ�С�������30KBʱ����û��ʵ�����ݣ����账��
      CWebDataReceivedPtr pWebDataReceived = pPotenDailyBriefingWebData->TransferWebDataToQueueData();
      if (pWebDataReceived != nullptr) {
        pWebDataReceived->m_lTime = (INT64)(pPotenDailyBriefingWebData->GetInquiringDay()) * 1000000;
        gl_WebDataInquirer.PushPotenDailyBriefingData(pWebDataReceived);
      }
    }
  }

  return 5;
}