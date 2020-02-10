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
#include"WebInquirer.h"

UINT ThreadReadNeteaseRTData(LPVOID pParam) {
  CNeteaseRTWebInquiry* pNeteaseRTWebData = (CNeteaseRTWebInquiry*)pParam;

  if (pNeteaseRTWebData->ReadWebData(50, 30, 30)) {
    CWebDataPtr pWebDataReceived = pNeteaseRTWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_WebInquirer.PushNeteaseRTData(pWebDataReceived);
    }
  }

  return 3;
}