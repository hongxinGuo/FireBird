#include"stdafx.h"
#include"globedef.h"
#include"Market.h"

#include"thread.h"
#include "TengxunRTWebData.h"

bool CTengxunRTWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CTengxunRTWebData::CTengxunRTWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
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
            str += _T(" ��Ѷʵʱ��⵽�����ڻ�Ծ״̬");
            gl_systemMessage.PushInnerSystemInformationMessage(str);
          }
        }
        else {
          str = pRTData->GetStockCode();
          str += _T(" ��Ч��Ʊ���루��Ѷʵʱ���ݣ�");
          gl_systemMessage.PushInnerSystemInformationMessage(str);
        }
      }
    }
#endif // DEBUG
    //gl_QueueRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
    return true;
  }
  return false;
}

void CTengxunRTWebData::ProcessWebDataStored(void) {
  //������ĺ������붨ʱ���ȴ���ÿ3��ִ��һ�μ��ɡ��������������κ����飬
  //gl_ChinaStockMarket.DistributeRTDataReceivedFromWebToProperStock();
}

void CTengxunRTWebData::ReportDataError(void)
{
  TRACE("��Ѷʵʱ��������,�׵�����\n");
  CString str;
  str = _T("��Ѷʵʱ��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��http://qt.gtimg.cn\n");
  CString str;
  str = _T("Error reading http file ��http://qt.gtimg.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // ������һ���ι�Ʊʵʱ����
  if (!gl_ChinaStockMarket.SystemReady() || gl_ChinaStockMarket.IsUsingTengxunRTDataReceiverAsTester()) { // ���ϵͳ��δ׼���ã���ʹ��ȫ�ֹ�Ʊ��
    gl_ChinaStockMarket.CreateSinaRTDataInquiringStr(strMiddle); // ��ѶҲʹ��������ʽ
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    GetInquiringStr(strMiddle);
  }
  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
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
  TRACE("����%d����Ѷʵʱ����\n", lNumberOfData);
  return true;
}