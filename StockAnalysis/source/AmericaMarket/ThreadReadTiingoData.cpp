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

UINT ThreadReadTiingoData(not_null<CTiingoWebInquiry*> pFinnhubWebData) {
  // ����ʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  ASSERT(!gl_pWorldMarket->IsTiingoDataReceived());
  if (pFinnhubWebData->ReadWebData()) { // 800�����ȡʱ�䣬Ŀǰ������ı�
    CWebDataPtr pWebDataReceived = pFinnhubWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushTiingoData(pWebDataReceived);
    }
  }
  else {
    while (gl_WebInquirer.GetFinnhubDataSize() > 0) gl_WebInquirer.PopFinnhubData();
    gl_pWorldMarket->SetTiingoInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Finnhub���������־��
  }
  pFinnhubWebData->SetReadingWebData(false);
  gl_pWorldMarket->SetTiingoDataReceived(true); // �������������ݺ����״̬��

  return 8;
}