/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪实时行情接口读取线程。
//
// 新浪的服务器延迟不超过100ms，故而在等待100ms后即可开始读取接收到的数据。
//
// 这个线程是用于更新证券代码库的，只在市场交易时间外执行。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"WebInquirer.h"

UINT ThreadReadSinaStakeRTData(not_null<CSinaStakeRTWebInquiry*> pSinaStakeRTWebData) {
  // 新浪实时数据库的读取时间，以下列数字比较合适，再快的话，可能会出现丢帧现象。
  // 完全克服的话，还需要使用其他方法来确定服务器是否发送结束，目前的方法只是读不到了就认为结束了。
  gl_ThreadStatus.IncreaseRunningThread();
  if (pSinaStakeRTWebData->ReadWebData()) {
    CWebDataPtr pWebDataReceived = pSinaStakeRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushSinaStakeRTData(pWebDataReceived);
    }
  }
  gl_ThreadStatus.DecreaseRunningThread();

  return 6;
}