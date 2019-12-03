#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "SinaRTWebData.h"

CSinaRTWebData::CSinaRTWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
}

CSinaRTWebData::~CSinaRTWebData() {
}

void CSinaRTWebData::ProcessCurrentWebData(void) {
  CSingleLock sl(&gl_ProcessSinaRTDataQueue);

  if (IsWebDataReceived()) {
    if (IsReadingSucceed()) { //网络通信一切顺利？
      sl.Lock();
      if (sl.IsLocked()) {
        ResetCurrentPos();
        if (SucceedReadingAndStoringWebData()) {
          ProcessWebDataStored();
        }
      }
      sl.Unlock();
    }
  }
  else {  // 网络通信出现错误
    ReportCommunicationError();
  }
}

bool CSinaRTWebData::SucceedReadingAndStoringOneWebData(void) {
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadSinaData(this)) {
    pRTData->SetDataSource(__SINA_RT_WEB_DATA__); // 从新浪实时行情服务器处接收到的数据
    gl_QueueSinaRTData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
    //gl_QueueSinaRTDataForSave.PushRTData(pRTData); // 同时存入待存储实时数据队列
    return true;
  }
  return false;
}

void CSinaRTWebData::ProcessWebDataStored(void) {
  //将下面的函数移入定时调度处，每3秒执行一次即可。本函数无需做任何事情，
  //gl_ChinaStockMarket.DistributeRTDataReceivedFromWebToProperStock();
}

void CSinaRTWebData::ReportDataError(void) {
  TRACE("数据有误,抛掉不用\n");
  CString str;
  str = _T("新浪实时数据有误");
  gl_systemMessage.PushInformationMessage(str);
}

void CSinaRTWebData::ReportCommunicationError(void) {
  TRACE("Error reading http file ：hq.sinajs.cn\n");
  CString str;
  str = _T("Error reading http file ：hq.sinajs.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CSinaRTWebData::InquireNextWebData(void) {
  static int iCountUp = 0;
  CRTDataPtr pRTData = nullptr;
  static int iTotalInquiringStocks = 0;

  CString strMiddle = _T("");

  // 申请下一批次股票实时数据
  if (gl_ChinaStockMarket.IsCheckTodayActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
    iTotalInquiringStocks += GetInquiringStr(strMiddle, 900, false);
    if (iTotalInquiringStocks > 36000) {
      if (!gl_ChinaStockMarket.SystemReady()) { // 如果系统尚未设置好，则显示系统准备
        gl_systemMessage.PushInformationMessage(_T("完成系统初始化"));
      }
      gl_ChinaStockMarket.SetSystemReady(true); // 所有的股票实时数据都轮询三遍，当日活跃股票集已经建立，故而可以接受日线数据了。
    }
  }
  else { // 开市时使用今日活跃股票池
    GetInquiringStr(strMiddle, 900, true);
  }
  CreateTotalInquiringString(strMiddle);

  SetWebDataReceived(false);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

int CSinaRTWebData::GetInquiringStr(CString& strInquire, long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(strInquire, lTotalNumber, fSkipUnactiveStock);
}

void CSinaRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadSinaRTData, nullptr);
}

bool CSinaRTWebData::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个新浪实时数据\n", lNumberOfData);
  return true;
}