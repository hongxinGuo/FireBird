/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Crweber.com行情接口读取线程。
//
// Crweber.com的服务器延迟比较长，故而在等待500ms后开始读取接收到的数据。
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