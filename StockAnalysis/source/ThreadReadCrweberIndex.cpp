/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Crweber.com����ӿڶ�ȡ�̡߳�
//
// Crweber.com�ķ������ӳٱȽϳ����ʶ��ڵȴ�500ms��ʼ��ȡ���յ������ݡ�
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"ChinaMarket.h"
#include "Thread.h"
#include"VirtualWebInquiry.h"

UINT ThreadReadCrweberIndex(LPVOID pParam) {
  CCrweberIndexWebInquiry* pCrweberIndexWebData = (CCrweberIndexWebInquiry*)(pParam);
  if (pCrweberIndexWebData->ReadWebData(500, 50, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pCrweberIndexWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushCrweberData(pWebDataReceived);
    }
  }

  return 5;
}