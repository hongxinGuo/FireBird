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

UINT ThreadReadNeteaseRTData(CNeteaseRTWebInquiry* pNeteaseRTWebData) {
  gl_ThreadStatus.IncreaseNumberOfRunningThread();
  if (pNeteaseRTWebData->ReadWebData(100, 30, 30)) {
    ASSERT(!pNeteaseRTWebData->IsReadingWebData());
    CWebDataPtr pWebDataReceived = pNeteaseRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushNeteaseRTData(pWebDataReceived);
    }
  }
  else ASSERT(!pNeteaseRTWebData->IsReadingWebData());
  gl_ThreadStatus.DecreaseNumberOfRunningThread();

  return 3;
}