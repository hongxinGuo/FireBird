/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时行情接口读取线程。
//
// 腾讯的服务器延迟不超过100ms，故而在等待100ms后即可开始读取接收到的数据。
//
// 此工作线程作为新浪实时行情线程的辅助和补充，故而需要在系统准备完毕后方可执行。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"WebInquirer.h"

UINT ThreadReadTengxunRTData(not_null<CTengxunRTWebInquiry*> pTengxunRTWebData) {
  // 腾讯实时数据库的读取时间，以下列数字比较合适，再快的话，可能会出现丢帧现象。
  // 完全克服的话，还需要使用其他方法来确定服务器是否发送结束，目前的方法只是读不到了就认为结束了。
  gl_ThreadStatus.IncreaseRunningThread();
  if (pTengxunRTWebData->ReadWebData()) {
    CWebDataPtr pWebDataReceived = pTengxunRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushTengxunRTData(pWebDataReceived);
    }
  }
  pTengxunRTWebData->SetReadingWebData(false);
  gl_ThreadStatus.DecreaseRunningThread();

  return 2;
}