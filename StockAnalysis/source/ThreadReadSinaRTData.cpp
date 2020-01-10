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
  CSinaRTWebData* pSinaRTWebData = (CSinaRTWebData*)pParam;

  const clock_t tt = clock();

  // ����ʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  if (pSinaRTWebData->ReadWebData(50, 30, 20)) {
    CWebDataReceivedPtr pWebDataReceived = pSinaRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebDataInquirer.PushSinaRTData(pWebDataReceived);
    }
  }
  gl_ChinaStockMarket.SetReadingSinaRTDataTime(clock() - tt);

  return 1;
}