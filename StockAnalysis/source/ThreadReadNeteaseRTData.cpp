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
  CNeteaseWebRTData* pNeteaseWebRTData = (CNeteaseWebRTData*)pParam;

  if (pNeteaseWebRTData->ReadWebData(200, 50, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pNeteaseWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueNeteaseWebRTData.PushWebRTData(pWebDataReceived);
    }
  }

  return 13;
}