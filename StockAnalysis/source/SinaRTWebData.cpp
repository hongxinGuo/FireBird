#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "SinaRTWebData.h"

bool CSinaRTWebData::sm_fCreatedOnce = false; // 初始时没有生成过实例

CSinaRTWebData::CSinaRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // 如果已经生成过一个实例了，则报错
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://hq.sinajs.cn/list=");
  m_strWebDataInquireSuffix = _T("");
}

CSinaRTWebData::~CSinaRTWebData() {
}

bool CSinaRTWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  if (pRTData->ReadSinaData(pCurrentPos, iCount)) {
    gl_QueueRTData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
    return true;
  }
  return false;
}

void CSinaRTWebData::ProcessWebDataStored(void) {
  gl_ChinaStockMarket.ProcessSinaRTDataReceivedFromWeb();
}

void CSinaRTWebData::ReportDataError(void)
{
  TRACE("数据有误,抛掉不用\n");
  CString str;
  str = _T("新浪实时数据有误");
  gl_systemMessage.PushInformationMessage(str);
}

void CSinaRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ：hq.sinajs.cn\n");
  CString str;
  str = _T("Error reading http file ：hq.sinajs.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CSinaRTWebData::InquireNextWebData(void)
{
  static int iCountUp = 0;
  CRTDataPtr pRTData = nullptr;

  CString strMiddle = _T("");

  // 申请下一批次股票实时数据
  if (gl_ChinaStockMarket.IsCheckTodayActiveStock() || !gl_ChinaStockMarket.SystemReady()) { // 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
    if (gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(strMiddle)) {
      if (gl_ChinaStockMarket.CountLoopRTDataInquiring()) {  // 遍历三遍全体股票池
        if (!gl_ChinaStockMarket.SystemReady()) { // 如果系统尚未设置好，则显示系统准备
          gl_systemMessage.PushInformationMessage(_T("完成系统初始化"));
        }
        gl_ChinaStockMarket.SetSystemReady(true); // 所有的股票实时数据都轮询三遍，当日活跃股票集已经建立，故而可以接受日线数据了。
        gl_ChinaStockMarket.ResetIT();
      }
    }
    gl_SinaRTWebData.CreateTotalInquiringString(strMiddle);
  }
  else { // 开市时使用今日活跃股票池
    GetInquiringStr(strMiddle);
    CreateTotalInquiringString(strMiddle);
  }
  SetWebDataReceived(false);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

int CSinaRTWebData::GetInquiringStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetSinaInquiringStockStr(strInquire);
}

void CSinaRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadSinaRTData, nullptr);
}