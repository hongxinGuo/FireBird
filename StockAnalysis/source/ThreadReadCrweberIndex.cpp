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
//#include"ChinaMarket.h"
#include "Thread.h"
#include"VirtualWebInquiry.h"
#include"WebInquirer.h"

UINT ThreadReadCrweberIndex(CCrweberIndexWebInquiry* pCrweberIndexWebData) {
  gl_ThreadStatus.IncreaseNumberOfRunningThread();
  if (pCrweberIndexWebData->ReadWebData(500, 50, 30)) {
    CWebDataPtr pWebDataReceived = pCrweberIndexWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushCrweberData(pWebDataReceived);
    }
  }
  gl_ThreadStatus.DecreaseNumberOfRunningThread();

  return 5;
}