/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
// ���˵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
// ����߳������ڸ���֤ȯ�����ģ�ֻ���г�����ʱ����ִ�С�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"WebInquirer.h"

UINT ThreadReadSinaStakeRTData(not_null<CSinaStakeRTWebInquiry*> pSinaStakeRTWebData) {
  // ����ʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  gl_ThreadStatus.IncreaseRunningThread();
  if (pSinaStakeRTWebData->ReadWebData()) {
    CWebDataPtr pWebDataReceived = pSinaStakeRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushSinaStakeRTData(pWebDataReceived);
    }
  }
  gl_ThreadStatus.DecreaseRunningThread();

  return 6;
}