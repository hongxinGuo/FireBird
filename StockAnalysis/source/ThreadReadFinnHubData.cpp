//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Finnhub行情接口读取线程。
//
// Finnhub.io的服务器延迟较长，故而在等待800ms后即可开始读取接收到的数据。
// 由于从Finnhub上读取各种数据，且Finnhub限制读取频率不能超过每分钟60次，故而数据申请需要串行化。目前每两秒申请
// 一次。
// 在读取完网络数据后，需要清除状态，以允许下一次申请。
// 使用800毫秒，配合每0.5秒调用一次GetFinnhubDataFromWeb，保证在申请一秒后能够接收到数据，这样每次轮回为1.5秒。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"WebInquirer.h"

UINT ThreadReadFinnhubData(not_null<CFinnhubWebInquiry*> pFinnhubWebData) {
  // 新浪实时数据库的读取时间，以下列数字比较合适，再快的话，可能会出现丢帧现象。
  // 完全克服的话，还需要使用其他方法来确定服务器是否发送结束，目前的方法只是读不到了就认为结束了。
  ASSERT(!gl_pAmericaMarket->IsFinnhubDataReceived());
  gl_ThreadStatus.IncreaseRunningThread();
  if (pFinnhubWebData->ReadWebData(800, 400, 100)) { // 800毫秒读取时间，目前不允许改变
    CWebDataPtr pWebDataReceived = pFinnhubWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushFinnhubData(pWebDataReceived);
    }
  }
  else {
    while (gl_WebInquirer.GetFinnhubDataSize() > 0) gl_WebInquirer.PopFinnhubData();
    gl_pAmericaMarket->SetFinnhubInquiring(false); // 当工作线程出现故障时，需要清除Finnhub数据申请标志。
  }
  gl_pAmericaMarket->SetFinnhubDataReceived(true); // 接收完网络数据后，清除状态。
  gl_ThreadStatus.DecreaseRunningThread();

  return 1;
}