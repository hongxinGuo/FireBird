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
#include"Market.h"

UINT ThreadReadTengxunRTData(LPVOID pParam) {
  CTengxunWebRTData* pTengxunWebRTData = (CTengxunWebRTData*)pParam;

  if (pTengxunWebRTData->ReadWebData(100, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pTengxunWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueTengxunWebRTData.PushWebRTData(pWebDataReceived);
    }
  }

  return 10;
}