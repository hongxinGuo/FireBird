//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FinnHub����ӿڶ�ȡ�̡߳�
//
// ���˵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"WebInquirer.h"

UINT ThreadReadFinnHubData(not_null<CFinnhubWebInquiry*> pFinnhubWebData) {
  // ����ʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  ASSERT(gl_pAmericaStakeMarket->IsWaitingFinHubData());
  gl_ThreadStatus.IncreaseRunningThread();
  if (pFinnhubWebData->ReadWebData(300, 300, 200)) {
    CWebDataPtr pWebDataReceived = pFinnhubWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushFinnHubData(pWebDataReceived);
    }
  }
  gl_pAmericaStakeMarket->SetWaitingFinnHubData(false);
  gl_ThreadStatus.DecreaseRunningThread();

  return 1;
}