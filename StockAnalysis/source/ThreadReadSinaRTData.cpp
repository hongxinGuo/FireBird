/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪实时行情接口读取线程。
//
// 新浪的服务器延迟不超过100ms，故而在等待100ms后即可开始读取接收到的数据。
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadReadSinaRTData(LPVOID pParam) {
  CSinaWebRTData* pSinaWebRTData = (CSinaWebRTData*)pParam;

  const clock_t tt = clock();

  if (pSinaWebRTData->ReadWebData(50, 30, 20)) {
    CWebDataReceivedPtr pWebDataReceived = pSinaWebRTData->TransferWebDataToQueueData();
    if (pWebDataReceived != nullptr) {
      gl_QueueSinaWebRTData.PushWebRTData(pWebDataReceived);
    }
  }
  gl_ChinaStockMarket.SetReadingSinaRTDataTime(clock() - tt);

  return 1;
}