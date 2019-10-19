#include"stdafx.h"

#include"globedef.h"
#include"Market.h"

#include"CWebData.h"

bool CWebData::GetWebData(void)
{
  static int iCountUp = 0;
  CRTDataPtr pRTData = nullptr;
  long i = 0;

  if (!IsReadingWebData()) {
    ProcessWebData();

    CString strTemp = _T("");

    // 申请下一批次股票实时数据
    if (gl_ChinaStockMarket.IsCheckTodayActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
      gl_stSinaRTDataInquire.strInquire = m_strSinaRTDataInquire; // 设置查询新浪实时数据的字符串头
      if (CreateSinaRTDataInquiringStr(strTemp)) {
        if (++m_lCountLoopRTDataInquiring >= 3) {  // 遍历三遍全体股票池
          if (!gl_ChinaStockMarket.SystemReady()) { // 如果系统尚未设置好，则显示系统准备
            gl_systemMessage.PushInformationMessage(_T("完成系统初始化"));
          }
          gl_ChinaStockMarket.SetSystemReady(true); // 所有的股票实时数据都轮询三遍，当日活跃股票集已经建立，故而可以接受日线数据了。
          gl_ChinaStockMarket.ResetIT();
        }
      }
      gl_stSinaRTDataInquire.strInquire += strTemp;
    }
    else { // 开市时使用今日活跃股票池
      gl_stSinaRTDataInquire.strInquire = m_strSinaRTDataInquire;
      GetInquiringStockStr(gl_stSinaRTDataInquire.strInquire);
    }
    SetWebDataReceived(false);
    SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
    StartReadingThread();
    // AfxBeginThread(ThreadReadSinaRTData, nullptr);
  }

  return true;
}

void CWebData::ProcessWebData(void) {
  CRTDataPtr pRTData = nullptr;
  char* pCurrentPos = nullptr;
  long i = 0;

  if (IsWebDataReceived()) {
    if (IsReadingSucceed()) { //网络通信一切顺利？
      pCurrentPos = m_buffer;
      long  iCount = 0;
      while (iCount < m_lByteRead) { // 新浪实时数据基本没有错误，不需要抛掉最后一组数据了。
        pRTData = make_shared<CRTData>();
        if (pRTData->ReadSinaData(pCurrentPos, m_lByteRead)) {
          i++;
          gl_QueueRTData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
        }
        else {
          TRACE("新浪实时数据有误,抛掉不用\n");
          CString str;
          str = _T("新浪实时数据有误");
          gl_systemMessage.PushInformationMessage(str);
          iCount = m_lByteRead; // 后面的数据可能出问题，抛掉不用。
        }
      }
      TRACE("读入%d个新浪实时数据\n", i);
      // 处理接收到的实时数据
      gl_ChinaStockMarket.ProcessRTDataReceivedFromWeb();
    }
    else {  // 网络通信出现错误
      TRACE("Error reading http file ：hq.sinajs.cn\n");
      CString str;
      str = _T("Error reading http file ：hq.sinajs.cn");
      gl_systemMessage.PushInformationMessage(str);
    }
  }

}

void CWebData::SetWebDataReceived(bool fFlag) {
  CSingleLock singleLock(&m_WebDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fWebDataReceived = fFlag;
    singleLock.Unlock();
  }
}

bool CWebData::IsWebDataReceived(void) {
  CSingleLock singleLock(&m_WebDataReceivedLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fWebDataReceived;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}

void CWebData::SetReadingWebData(bool fFlag) {
  CSingleLock singleLock(&m_ReadingWebDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_fReadingWebData = fFlag;
    singleLock.Unlock();
  }
}

bool CWebData::IsReadingWebData(void) {
  CSingleLock singleLock(&m_ReadingWebDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const bool fFlag = m_fReadingWebData;
    singleLock.Unlock();
    return fFlag;
  }
  ASSERT(0);
  return false; // 此分支不可能执行到，只为了消除编译器的警告而存在
}
