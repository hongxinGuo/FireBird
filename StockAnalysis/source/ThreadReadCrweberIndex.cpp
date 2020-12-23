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

UINT ThreadReadCrweberIndex(not_null<CCrweberIndexWebInquiry*> pCrweberIndexWebData) {
  gl_ThreadStatus.IncreaseRunningThread();
  if (pCrweberIndexWebData->ReadWebData()) {
    CWebDataPtr pWebDataReceived = pCrweberIndexWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushCrweberData(pWebDataReceived);
    }
  }
  gl_ThreadStatus.DecreaseRunningThread();

  return 5;
}