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
#include"WebInquirer.h"

UINT ThreadReadSinaRTData(not_null<CSinaRTWebInquiry*> pSinaRTWebData) {
  // ����ʵʱ���ݿ�Ķ�ȡʱ�䣬���������ֱȽϺ��ʣ��ٿ�Ļ������ܻ���ֶ�֡����
  // ��ȫ�˷��Ļ�������Ҫʹ������������ȷ���������Ƿ��ͽ�����Ŀǰ�ķ���ֻ�Ƕ������˾���Ϊ�����ˡ�
  if (pSinaRTWebData->ReadWebData()) {
    CWebDataPtr pWebDataReceived = pSinaRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushSinaRTData(pWebDataReceived);
    }
  }
  pSinaRTWebData->SetReadingWebData(false);

  return 1;
}