#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "TengxunRTWebData.h"

bool CTengxunRTWebData::sm_fCreatedOnce = false; // 初始时没有生成过实例

CTengxunRTWebData::CTengxunRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // 如果已经生成过一个实例了，则报错
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://qt.gtimg.cn/q=");
  m_strWebDataInquireSuffix = _T("");
}

CTengxunRTWebData::~CTengxunRTWebData() {
}

bool CTengxunRTWebData::SucceedReadingAndStoringOneWebData(void)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  CString strVolume;
  char buffer[200];

  if (pRTData->ReadTengxunData(m_pCurrentPos, m_lCurrentPos)) {
    pRTData->SetDataSource(__TENGXUN_RT_WEB_DATA__);
#ifdef DEBUG
    // 测试网易实时数据与新浪实时数据的同一性。
    if (gl_TESTpRTData != nullptr) {
      if (pRTData->GetStockCode().Compare(gl_TESTpRTData->GetStockCode()) == 0) {
        sprintf_s(buffer, "volume: %I64d, askvol1: %d, askvol2: %d, askvol3: %d, askvol4: %d, askvol5: %d",
          pRTData->GetVolume(), pRTData->GetVSell(0), pRTData->GetVSell(1), pRTData->GetVSell(2), pRTData->GetVSell(3), pRTData->GetVSell(4));
        strVolume = _T("2  ");
        strVolume += buffer;
        gl_systemMessage.PushInnerSystemInformationMessage(strVolume);
      }
    }
    if (gl_ChinaStockMarket.IsUsingNeteaseRTDataReceiverAsTester()) {
      CString str;
      if (pRTData->IsActive()) {
        CStockPtr pStock = nullptr;
        if ((pStock = gl_ChinaStockMarket.GetStockPtr(pRTData->GetStockCode())) != nullptr) {
          if (!pStock->IsActive()) {
            str = pStock->GetStockCode();
            str += _T(" 腾讯实时检测到不处于活跃状态");
            gl_systemMessage.PushInnerSystemInformationMessage(str);
          }
        }
        else {
          str = pRTData->GetStockCode();
          str += _T(" 无效股票代码（腾讯实时数据）");
          gl_systemMessage.PushInnerSystemInformationMessage(str);
        }
      }
    }
#endif // DEBUG
    //gl_QueueRTData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
    return true;
  }
  return false;
}

void CTengxunRTWebData::ProcessWebDataStored(void) {
  //将下面的函数移入定时调度处，每3秒执行一次即可。本函数无需做任何事情，
  //gl_ChinaStockMarket.DistributeRTDataReceivedFromWebToProperStock();
}

void CTengxunRTWebData::ReportDataError(void)
{
  TRACE("腾讯实时数据有误,抛掉不用\n");
  CString str;
  str = _T("腾讯实时数据有误");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ：http://qt.gtimg.cn\n");
  CString str;
  str = _T("Error reading http file ：http://qt.gtimg.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // 申请下一批次股票实时数据
  if (!gl_ChinaStockMarket.SystemReady() || gl_ChinaStockMarket.IsUsingTengxunRTDataReceiverAsTester()) { // 如果系统尚未准备好，则使用全局股票池
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(strMiddle); // 腾讯也使用新浪制式
  }
  else { // 开市时使用今日活跃股票池
    GetInquiringStr(strMiddle);
  }
  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

int CTengxunRTWebData::GetInquiringStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetTengxunInquiringStockStr(strInquire);
}

void CTengxunRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadTengxunRTData, nullptr);
}

bool CTengxunRTWebData::ReportStatus(long lNumberOfData)
{
  TRACE("读入%d个腾讯实时数据\n", lNumberOfData);
  return true;
}