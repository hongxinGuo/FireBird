/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʵʱ����ӿڶ�ȡ�̡߳�
//
// ���׵ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
// �˹����߳���Ϊ����ʵʱ�����̵߳ĸ����Ͳ��䣬�ʶ���Ҫ��ϵͳ׼����Ϻ󷽿�ִ�С�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"WebInquirer.h"

UINT ThreadReadNeteaseRTData(not_null<CNeteaseRTWebInquiry*> pNeteaseRTWebData) {
  gl_ThreadStatus.IncreaseRunningThread();
  if (pNeteaseRTWebData->ReadWebData()) {
    CWebDataPtr pWebDataReceived = pNeteaseRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushNeteaseRTData(pWebDataReceived);
    }
  }
  else {
    TRACE("��ȡ����ʵʱ���ݳ���\n");
  }
  pNeteaseRTWebData->SetReadingWebData(false);
  gl_ThreadStatus.DecreaseRunningThread();

  return 3;
}