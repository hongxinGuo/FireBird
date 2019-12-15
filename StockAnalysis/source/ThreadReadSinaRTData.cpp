/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
// ���˵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadReadSinaRTData(LPVOID pParam) {
  CSinaWebRTData* pSinaWebRTData = (CSinaWebRTData*)pParam;

  const clock_t tt = clock();

  if (pSinaWebRTData->ReadWebData(100, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pSinaWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueSinaWebRTData.PushWebRTData(pWebDataReceived);
    }
  }
  gl_ChinaStockMarket.SetReadingSinaRTDataTime(clock() - tt);

  return 1;
}