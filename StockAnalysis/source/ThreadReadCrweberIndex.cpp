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
  CCrweberIndexWebData* pCrweberIndexWebData = (CCrweberIndexWebData*)(pParam);

  if (pCrweberIndexWebData->ReadWebData(500, 50, 20)) {
    CWebDataReceivedPtr pWebDataReceived = pCrweberIndexWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueCrweberdotcomWebData.PushWebRTData(pWebDataReceived);
    }
  }

  return 5;
}