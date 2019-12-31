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
#include"Market.h"
#include "Thread.h"
#include"WebData.h"

UINT ThreadReadCrweberIndex(LPVOID pParam) {
  CCrweberIndexWebData* pCrweberIndexWebData = (CCrweberIndexWebData*)(pParam);

  if (pCrweberIndexWebData->ReadWebData(500, 50, 20)) {
    CWebDataReceivedPtr pWebDataReceived = pCrweberIndexWebData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueCrweberdotcomWebData.PushWebRTData(pWebDataReceived);
    }
  }

  return 5;
}