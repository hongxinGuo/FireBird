//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FinnHub����ӿڶ�ȡ�̡߳�
//
// Finnhub.io�ķ������ӳٽϳ����ʶ��ڵȴ�300ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
// ���ڴ�Finnhub�϶�ȡ�������ݣ���Finnhub���ƶ�ȡƵ�ʲ��ܳ���ÿ����60�Σ��ʶ�����������Ҫ���л���Ŀǰÿ��������
// һ�Ρ�
// �ڶ�ȡ���������ݺ���Ҫ���״̬����������һ�����롣
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"WebInquirer.h"

UINT ThreadReadFinnHubData(not_null<CFinnhubWebInquiry*> pFinnhubWebData) {
  // ����ʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  ASSERT(gl_pAmericaStakeMarket->IsWaitingFinHubData());
  gl_ThreadStatus.IncreaseRunningThread();
  if (pFinnhubWebData->ReadWebData(300, 100, 100)) {
    CWebDataPtr pWebDataReceived = pFinnhubWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushFinnHubData(pWebDataReceived);
    }
  }
  gl_pAmericaStakeMarket->SetWaitingFinnHubData(false); // �������������ݺ����״̬��
  gl_ThreadStatus.DecreaseRunningThread();

  return 1;
}