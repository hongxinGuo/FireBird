/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Crweber.com����ӿڶ�ȡ�̡߳�
//
// Crweber.com�ķ������ӳٱȽϳ����ʶ��ڵȴ�500ms��ʼ��ȡ���յ������ݡ�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"
#include "Thread.h"
#include"WebData.h"

UINT ThreadReadCrweberIndex(LPVOID pParam) {
  CCrweberIndexWebData* pCrweberWebData = (CCrweberIndexWebData*)(pParam);

  if (pCrweberWebData->ReadWebData(200, 50, 20)) {
    CWebDataReceivedPtr pWebDataReceived = pCrweberWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueCrweberdotcomWebData.PushWebRTData(pWebDataReceived);
    }
  }

  return 12;
}