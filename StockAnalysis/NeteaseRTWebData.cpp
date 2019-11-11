/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// ����ʵʱ����ȱ�ٹؼ��Եĳɽ����һ��ʶ��޷���Ϊ�������ݣ�ֻ����Ϊ�����á�
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "NeteaseRTWebData.h"

CNeteaseRTWebData::CNeteaseRTWebData() : CWebData() {
  m_strWebDataInquirePrefix = _T("http://api.money.126.net/data/feed/");
  m_strWebDataInquireSuffix = _T("");
}

CNeteaseRTWebData::~CNeteaseRTWebData() {
}

bool CNeteaseRTWebData::ReadPrefix(void)
{
  static char buffer1[200];
  static CString strHeader = _T("_ntes_quote_callback({");

  strncpy_s(buffer1, m_pCurrentPos, 22); // ����"_ntes_quote_callback({"
  buffer1[22] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
    return false;
  }
  IncreaseCurrentPos(22);

  return true;
}

bool CNeteaseRTWebData::IsReadingFinished(void)
{
  if ((*m_pCurrentPos == ' ') || (m_lCurrentPos >= (m_lByteRead - 4))) { // ����β����
    return true;
  }
  else {
    return false;
  }
}

bool CNeteaseRTWebData::ReportStatus(long lNumberOfData)
{
  TRACE("����%d������ʵʱ����\n", lNumberOfData);
  return true;
}

bool CNeteaseRTWebData::SucceedReadingAndStoringOneWebData(void)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  CString strVolume;
  char buffer[200];

  if (pRTData->ReadNeteaseData(this)) {
    pRTData->SetDataSource(__NETEASE_RT_WEB_DATA__);
#ifdef DEBUG
    // ��������ʵʱ����������ʵʱ���ݵ�ͬһ�ԡ�
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
            str += _T(" ����ʵʱ��⵽�����ڻ�Ծ״̬");
            gl_systemMessage.PushInnerSystemInformationMessage(str);
          }
        }
        else {
          str = pRTData->GetStockCode();
          str += _T(" ��Ч��Ʊ���루����ʵʱ���ݣ�");
          gl_systemMessage.PushInnerSystemInformationMessage(str);
        }
      }
    }
#endif // DEBUG
    //gl_QueueSinaRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���.����ʵʱ����ȱ���ܳɽ����һ�ֻ����Ϊ�������ݣ��ʶ���ʱ��ʹ�á�
    return true;
  }
  return false;
}

void CNeteaseRTWebData::ProcessWebDataStored(void) {
  //������ĺ������붨ʱ���ȴ���ÿ3��ִ��һ�μ��ɡ��������������κ����飬
  //gl_ChinaStockMarket.DistributeRTDataReceivedFromWebToProperStock();
}

void CNeteaseRTWebData::ReportDataError(void)
{
  TRACE("����ʵʱ��������,�׵�����\n");
  CString str;
  str = _T("����ʵʱ��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��http://api.money.126.net/data/feed/\n");
  CString str;
  str = _T("Error reading http file ��http://api.money.126.net/data/feed/");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseRTWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");

  // ������һ���ι�Ʊʵʱ����
  if (!gl_ChinaStockMarket.SystemReady() || gl_ChinaStockMarket.IsUsingNeteaseRTDataReceiverAsTester()) { // ���ϵͳ��δ׼���ã���ʹ��ȫ�ֹ�Ʊ��
    GetInquiringStr(strMiddle, 900, false);
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    GetInquiringStr(strMiddle);
  }
  CreateTotalInquiringString(strMiddle);

  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CNeteaseRTWebData::GetInquiringStr(CString& strInquire, long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetNeteaseInquiringStockStr(strInquire, lTotalNumber, fSkipUnactiveStock);
}

void CNeteaseRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseRTData, nullptr);
}