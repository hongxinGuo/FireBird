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
#include"ChinaMarket.h"

UINT ThreadReadNeteaseRTData(LPVOID pParam) {
  CNeteaseRTWebInquiry* pNeteaseRTWebData = (CNeteaseRTWebInquiry*)pParam;

  if (pNeteaseRTWebData->ReadWebData(50, 30, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pNeteaseRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebDataInquirer.PushNeteaseRTData(pWebDataReceived);
    }
  }

  return 3;
}