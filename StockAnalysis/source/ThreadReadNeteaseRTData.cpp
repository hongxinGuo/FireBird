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
#include"Market.h"

UINT ThreadReadNeteaseRTData(LPVOID pParam) {
  CNeteaseRTWebData* pNeteaseWebRTData = (CNeteaseRTWebData*)pParam;

  if (pNeteaseWebRTData->ReadWebData(50, 30, 20)) {
    CWebDataReceivedPtr pWebDataReceived = pNeteaseWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_queueWebInquire.PushNeteaseRTData(pWebDataReceived);
    }
  }

  return 3;
}