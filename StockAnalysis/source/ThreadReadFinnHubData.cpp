//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FinnHub行情接口读取线程。
//
// Finnhub.io的服务器延迟较长，故而在等待300ms后即可开始读取接收到的数据。
// 由于从Finnhub上读取各种数据，且Finnhub限制读取频率不能超过每分钟60次，故而数据申请需要串行化。目前每两秒申请
// 一次。
// 在读取完网络数据后，需要清除状态，以允许下一次申请。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"WebInquirer.h"

UINT ThreadReadFinnHubData(not_null<CFinnhubWebInquiry*> pFinnhubWebData) {
  // 新浪实时数据库的读取时间，以下列数字比较合适，再快的话，可能会出现丢帧现象。
  // 完全克服的话，还需要使用其他方法来确定服务器是否发送结束，目前的方法只是读不到了就认为结束了。
  ASSERT(gl_pAmericaStakeMarket->IsWaitingFinHubData());
  gl_ThreadStatus.IncreaseRunningThread();
  if (pFinnhubWebData->ReadWebData(300, 100, 100)) {
    CWebDataPtr pWebDataReceived = pFinnhubWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushFinnHubData(pWebDataReceived);
    }
  }
  gl_pAmericaStakeMarket->SetWaitingFinnHubData(false); // 接收完网络数据后，清除状态。
  gl_ThreadStatus.DecreaseRunningThread();

  return 1;
}