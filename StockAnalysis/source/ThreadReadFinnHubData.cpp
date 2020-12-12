//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub����ӿڶ�ȡ�̡߳�
//
// Finnhub.io�ķ������ӳٽϳ����ʶ��ڵȴ�800ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
// ���ڴ�Finnhub�϶�ȡ�������ݣ���Finnhub���ƶ�ȡƵ�ʲ��ܳ���ÿ����60�Σ��ʶ�����������Ҫ���л���Ŀǰÿ��������
// һ�Ρ�
// �ڶ�ȡ���������ݺ���Ҫ���״̬����������һ�����롣
// ʹ��800���룬���ÿ0.5�����һ��GetFinnhubDataFromWeb����֤������һ����ܹ����յ����ݣ�����ÿ���ֻ�Ϊ1.5�롣
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"WebInquirer.h"

UINT ThreadReadFinnhubData(not_null<CFinnhubWebInquiry*> pFinnhubWebData) {
  // ����ʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  ASSERT(gl_pAmericaStakeMarket->IsWaitingFinHubData());
  gl_ThreadStatus.IncreaseRunningThread();
  if (pFinnhubWebData->ReadWebData(800, 400, 100)) { // 800�����ȡʱ�䣬Ŀǰ������ı�
    CWebDataPtr pWebDataReceived = pFinnhubWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushFinnhubData(pWebDataReceived);
    }
  }
  gl_pAmericaStakeMarket->SetWaitingFinnhubData(false); // �������������ݺ����״̬��
  gl_ThreadStatus.DecreaseRunningThread();

  return 1;
}