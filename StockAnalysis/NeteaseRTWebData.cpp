#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "NeteaseRTWebData.h"

bool CNeteaseRTWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CNeteaseRTWebData::CNeteaseRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://api.money.126.net/data/feed/");
  m_strWebDataInquireSuffix = _T("");
}

CNeteaseRTWebData::~CNeteaseRTWebData() {
}

bool CNeteaseRTWebData::ReadPrefix(char*& pCurrentPos, long& iCount)
{
  static char buffer1[200];
  static CString strHeader = _T("_ntes_quote_callback({");

  strncpy_s(buffer1, pCurrentPos, 22); // ����"_ntes_quote_callback"
  buffer1[22] = 0x000;
  CString str1;
  str1 = buffer1;
  if (strHeader.Compare(str1) != 0) { // ���ݸ�ʽ����
    return false;
  }
  pCurrentPos += 22;
  iCount += 22;

  return true;
}

bool CNeteaseRTWebData::IsReadingFinished(const char* const pCurrentPos, const long iCount)
{
  if ((*pCurrentPos == ' ') || (iCount >= (m_lByteRead - 4))) { // ����β����
    return true;
  }
  else {
    return false;
  }
}

bool CNeteaseRTWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  CRTDataPtr pRTData = make_shared<CRTData>();
  CString strVolume;
  char buffer[200];

  if (pRTData->ReadNeteaseData(pCurrentPos, iCount)) {
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
    //gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���.����ʵʱ����ȱ���ܳɽ����һ�ֻ����Ϊ�������ݣ��ʶ���ʱ��ʹ�á�
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
    gl_ChinaStockMarket.CreateNeteaseRTDataInquiringStr(strMiddle);
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    GetInquiringStr(strMiddle);
  }
  CreateTotalInquiringString(strMiddle);

  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CNeteaseRTWebData::GetInquiringStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetNeteaseInquiringStockStr(strInquire);
}

void CNeteaseRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseRTData, nullptr);
}