////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读取网易日线历史数据的线程。
//
// 读取数据时多试几次，防止网络延迟导致数据截断。目前在读完数据后，在测试两次，都没有数据后才返回。
//
// 有时fReading == false的断言会失败，但程序中只有两处设置此变量，故估计是不同线程同时操作的原因，需要改为
// 同步事件模式唤醒此线程为好。研究之。（在调用此线程前就设置，就不会出现故障了，可见时启动线程时会出现延时所致）。
//
// 此线程最终的功能，要比现在只是提取数据要多一些，应该加入解码和存储功能（研究之，功能多会涉及更多的数据同步问题，需要权衡）。
// 亦即此线程的功能为：
// 1.从管道处读取需要提取日线历史数据的股票代码，将该代码处理成网易日线服务器所要求的格式，然后发送给日线服务器；
// 2.等待一段时间后（200ms）开始从服务器处接收数据。
// 3.将接收到的数据解码，成功的话存入相应股票的日线容器中，最后设置相应的标识。
//
// 此线程的正常返回值为4
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include"ChinaMarket.h"

UINT ThreadReadNeteaseDayLine(LPVOID pParam) {
  CNeteaseDayLineWebInquiry* pNeteaseDayLineWebData = (CNeteaseDayLineWebInquiry*)pParam;

  if (pNeteaseDayLineWebData->ReadWebData(/*siDelayTime*/ 200, 30, 30)) {
    // 将读取的日线数据放入相关股票的日线数据缓冲区中，并设置相关标识。
    CChinaStockPtr pStock = gl_ChinaStockMarket.GetStock(pNeteaseDayLineWebData->GetDownLoadingStockCode());
    pStock->TransferNeteaseDayLineWebDataToBuffer(pNeteaseDayLineWebData);
  }

  return 4; // 此线程正常返回值为4
}