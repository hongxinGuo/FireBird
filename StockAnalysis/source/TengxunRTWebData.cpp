#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "TengxunRTWebData.h"

CTengxunRTWebData::CTengxunRTWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://qt.gtimg.cn/q=");
  m_strWebDataInquireSuffix = _T("");
}

CTengxunRTWebData::~CTengxunRTWebData() {
}

bool CTengxunRTWebData::GetWebData(void) {
  if (!IsReadingWebData()) {
    InquireNextWebData();
  }
  return true;
}

bool CTengxunRTWebData::ReadPrefix(void) {
  char buffer[50];
  CString str = _T("v_pv_none_match=\"1\";\n"); // 此为无效股票查询到的数据格式，共21个字符

  strncpy_s(buffer, m_pCurrentPos, 21);
  buffer[21] = 0x000;
  CString str1 = buffer;

  if (str1.Compare(str) == 0) {
    IncreaseCurrentPos(21);
  }
  return true;
}

bool CTengxunRTWebData::SucceedReadingAndStoringOneWebData(void) {
  CRTDataPtr pRTData = make_shared<CRTData>();
  CString strVolume;
  char buffer[200];

  pRTData->SetDataSource(__TENGXUN_RT_WEB_DATA__);
  if (gl_ChinaStockMarket.IsUsingTengxunRTDataReceiverAsTester()) {
    CString str;
    if (pRTData->IsActive()) {
      CStockPtr pStock = nullptr;
      if ((pStock = gl_ChinaStockMarket.GetStockPtr(pRTData->GetStockCode())) != nullptr) {
        if (!pStock->IsActive()) {
          str = pStock->GetStockCode();
          str += _T(" 腾讯实时检测到不处于活跃状态");
          //gl_systemMessage.PushInnerSystemInformationMessage(str);
        }
      }
      else {
        str = pRTData->GetStockCode();
        str += _T(" 无效股票代码（腾讯实时数据）");
        gl_systemMessage.PushInnerSystemInformationMessage(str);
      }
    }
  }
  gl_QueueTengxunRTData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
  return true;
  return false;
}

void CTengxunRTWebData::ProcessWebDataStored(void) {
}

void CTengxunRTWebData::ReportDataError(void) {
  TRACE("腾讯实时数据有误,抛掉不用\n");
  CString str;
  str = _T("腾讯实时数据有误");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::ReportCommunicationError(void) {
  TRACE("Error reading http file ：http://qt.gtimg.cn\n");
  CString str;
  str = _T("Error reading http file ：http://qt.gtimg.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::InquireNextWebData(void) {
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // 申请下一批次股票实时数据。
  // 申请腾讯实时数据时，如果遇到不存在的股票代码，服务器会返回v_pv_none_match="1";，导致系统故障，
  // 故而现在只使用有效股票代码。
  if (!gl_ChinaStockMarket.SystemReady() || gl_ChinaStockMarket.IsUsingTengxunRTDataReceiverAsTester()) { // 如果系统尚未准备好，则使用全局股票池
    GetInquiringStr(strMiddle, 900, false);
  }
  else { // 开市时使用今日活跃股票池
    GetInquiringStr(strMiddle, 900, true);
  }

  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

int CTengxunRTWebData::GetInquiringStr(CString& strInquire, long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetTengxunInquiringStockStr(strInquire, lTotalNumber, fSkipUnactiveStock);
}

void CTengxunRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadTengxunRTData, this);
}

bool CTengxunRTWebData::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个腾讯实时数据\n", lNumberOfData);
  return true;
}