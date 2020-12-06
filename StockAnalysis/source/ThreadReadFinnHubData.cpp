//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FinnHub行情接口读取线程。
//
// 新浪的服务器延迟不超过100ms，故而在等待100ms后即可开始读取接收到的数据。
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"WebInquirer.h"

UINT ThreadReadFinnHubData(not_null<CFinnhubWebInquiry*> pFinnhubWebData) {
  // 新浪实时数据库的读取时间，以下列数字比较合适，再快的话，可能会出现丢帧现象。
  // 完全克服的话，还需要使用其他方法来确定服务器是否发送结束，目前的方法只是读不到了就认为结束了。
  ASSERT(gl_pAmericaStakeMarket->IsWaitingFinHubData());
  gl_ThreadStatus.IncreaseRunningThread();
  if (pFinnhubWebData->ReadWebData(300, 300, 200)) {
    CWebDataPtr pWebDataReceived = pFinnhubWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushFinnHubData(pWebDataReceived);
    }
  }
  gl_pAmericaStakeMarket->SetWaitingFinnHubData(false);
  gl_ThreadStatus.DecreaseRunningThread();

  return 1;
}