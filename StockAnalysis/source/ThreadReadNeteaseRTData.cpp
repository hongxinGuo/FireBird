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
  CNeteaseRTWebData* pNeteaseRTWebData = (CNeteaseRTWebData*)pParam;

  if (pNeteaseRTWebData->ReadWebData(50, 30, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pNeteaseRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebDataInquirer.PushNeteaseRTData(pWebDataReceived);
    }
  }

  return 3;
}