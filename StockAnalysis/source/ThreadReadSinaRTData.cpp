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
  CSinaRTWebData* pSinaWebRTData = (CSinaRTWebData*)pParam;

  const clock_t tt = clock();

  // ����ʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  if (pSinaWebRTData->ReadWebData(100, 50, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pSinaWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_queueWebInquire.PushSinaRTData(pWebDataReceived);
    }
  }
  gl_ChinaStockMarket.SetReadingSinaRTDataTime(clock() - tt);

  return 1;
}