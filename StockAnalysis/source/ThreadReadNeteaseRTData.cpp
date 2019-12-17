/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网易实时行情接口读取线程。
//
// 网易的服务器延迟不超过100ms，故而在等待100ms后即可开始读取接收到的数据。
//
// 此工作线程作为新浪实时行情线程的辅助和补充，故而需要在系统准备完毕后方可执行。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"globedef.h"
#include"Market.h"

UINT ThreadReadNeteaseRTData(LPVOID pParam) {
  CNeteaseWebRTData* pNeteaseWebRTData = (CNeteaseWebRTData*)pParam;

  if (pNeteaseWebRTData->ReadWebData(200, 50, 30)) {
    CWebDataReceivedPtr pWebDataReceived = pNeteaseWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueNeteaseWebRTData.PushWebRTData(pWebDataReceived);
    }
  }

  return 13;
}