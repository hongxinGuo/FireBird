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
// 2.等待一段时间后（100ms）开始从服务器处接收数据。
// 3.将接收到的数据解码，成功的话存入相应股票的日线容器中，最后设置相应的标识。
//
// 此线程的正常返回值为2
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include"Market.h"

UINT ThreadReadNeteaseDayLine(LPVOID pParam) {
  CNeteaseDayLineWebData* pNeteaseDayLineWebData = (CNeteaseDayLineWebData*)pParam;
  static int siDelayTime = 600;
  static bool fStarted = false;
  static atomic_int siCount = 0;   // 初始计数器，计算执行了几次此线程。用于设定延迟时间
  CInternetSession session;
  CHttpFile* pFile = nullptr;
  long iCount = 0;
  bool fDone = false;
  char* pChar = pNeteaseDayLineWebData->GetBufferAddr();
  CString str;

  const clock_t tt = clock();
  ASSERT(pNeteaseDayLineWebData->IsReadingWebData());    // 调用此线程时已经设置了此标识
  try {
    pNeteaseDayLineWebData->SetReadingWebData(true);
    pNeteaseDayLineWebData->SetReadingSucceed(true);
    pNeteaseDayLineWebData->SetByteReaded(0);
    pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)pNeteaseDayLineWebData->GetInquiringString()));
    Sleep(siDelayTime);
    while (!fDone) {
      do {
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pNeteaseDayLineWebData->AddByteReaded(iCount);
        }
      } while (iCount > 0);
      Sleep(30); // 等待30毫秒后再读一次，确认没有新数据后去读第三次，否则继续读。
      iCount = pFile->Read(pChar, 1024);
      if (iCount > 0) {
        pChar += iCount;
        pNeteaseDayLineWebData->AddByteReaded(iCount);
      }
      else {
        Sleep(30); // 等待30毫秒后读第三次，确认没有新数据后才返回，否则继续读。
        iCount = pFile->Read(pChar, 1024);
        if (iCount > 0) {
          pChar += iCount;
          pNeteaseDayLineWebData->AddByteReaded(iCount);
        }
        else fDone = true;
      }
    }
    *pChar = 0x000; // 最后加上一个结束符0X000
    pNeteaseDayLineWebData->SetWebDataReceived(true);

    char* p = pNeteaseDayLineWebData->GetBufferAddr();
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(pNeteaseDayLineWebData->GetDownLoadingStockCode());
    if (pStock->m_pDayLineBuffer != nullptr) delete pStock->m_pDayLineBuffer;
    pStock->m_pDayLineBuffer = new char[pNeteaseDayLineWebData->GetByteReaded() + 1]; // 缓冲区需要多加一个字符长度（最后那个0x000）。
    char* pbuffer = pStock->m_pDayLineBuffer;
    for (int i = 0; i < pNeteaseDayLineWebData->GetByteReaded() + 1; i++) {
      *pbuffer++ = *p++;
    }
    pStock->m_lDayLineBufferLength = pNeteaseDayLineWebData->GetByteReaded();
    pStock->SetDayLineNeedProcess(true);
    gl_ChinaStockMarket.m_iDayLineNeedProcess++;
  }
  catch (CInternetException * e) {  // 出现错误的话，简单报错即可，无需处理
    e->Delete();
    pNeteaseDayLineWebData->SetReadingSucceed(false);
    pNeteaseDayLineWebData->SetWebDataReceived(false);
    pNeteaseDayLineWebData->SetByteReaded(0);
  }
  if (pFile) pFile->Close();
  if (pFile) {
    delete pFile;
    pFile = nullptr;
  }
  pNeteaseDayLineWebData->SetReadingWebData(false);
  if (!fStarted) {
    fStarted = true;
    siDelayTime = 50;
  }

  gl_ChinaStockMarket.SetReadingNeteaseDayDataTime(clock() - tt);

  return 2; // 此线程正常返回值为2
}