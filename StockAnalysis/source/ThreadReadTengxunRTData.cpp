/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��Ѷʵʱ����ӿڶ�ȡ�̡߳�
//
// ��Ѷ�ķ������ӳٲ�����100ms���ʶ��ڵȴ�100ms�󼴿ɿ�ʼ��ȡ���յ������ݡ�
//
// �˹����߳���Ϊ����ʵʱ�����̵߳ĸ����Ͳ��䣬�ʶ���Ҫ��ϵͳ׼����Ϻ󷽿�ִ�С�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Market.h"

UINT ThreadReadTengxunRTData(LPVOID pParam) {
  CTengxunWebRTData* pTengxunWebRTData = (CTengxunWebRTData*)pParam;

  if (pTengxunWebRTData->ReadWebData(100, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pTengxunWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueTengxunWebRTData.PushWebRTData(pWebDataReceived);
    }
  }

  return 10;
}