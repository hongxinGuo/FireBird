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
  CString str = _T("v_pv_none_match=\"1\";\n"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����21���ַ�

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
          str += _T(" ��Ѷʵʱ��⵽�����ڻ�Ծ״̬");
          //gl_systemMessage.PushInnerSystemInformationMessage(str);
        }
      }
      else {
        str = pRTData->GetStockCode();
        str += _T(" ��Ч��Ʊ���루��Ѷʵʱ���ݣ�");
        gl_systemMessage.PushInnerSystemInformationMessage(str);
      }
    }
  }
  gl_QueueTengxunRTData.PushRTData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
  return true;
  return false;
}

void CTengxunRTWebData::ProcessWebDataStored(void) {
}

void CTengxunRTWebData::ReportDataError(void) {
  TRACE("��Ѷʵʱ��������,�׵�����\n");
  CString str;
  str = _T("��Ѷʵʱ��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::ReportCommunicationError(void) {
  TRACE("Error reading http file ��http://qt.gtimg.cn\n");
  CString str;
  str = _T("Error reading http file ��http://qt.gtimg.cn");
  gl_systemMessage.PushInformationMessage(str);
}

void CTengxunRTWebData::InquireNextWebData(void) {
  CString strMiddle = _T("");
  ASSERT(gl_ChinaStockMarket.SystemReady());

  // ������һ���ι�Ʊʵʱ���ݡ�
  // ������Ѷʵʱ����ʱ��������������ڵĹ�Ʊ���룬�������᷵��v_pv_none_match="1";������ϵͳ���ϣ�
  // �ʶ�����ֻʹ����Ч��Ʊ���롣
  if (!gl_ChinaStockMarket.SystemReady() || gl_ChinaStockMarket.IsUsingTengxunRTDataReceiverAsTester()) { // ���ϵͳ��δ׼���ã���ʹ��ȫ�ֹ�Ʊ��
    GetInquiringStr(strMiddle, 900, false);
  }
  else { // ����ʱʹ�ý��ջ�Ծ��Ʊ��
    GetInquiringStr(strMiddle, 900, true);
  }

  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CTengxunRTWebData::GetInquiringStr(CString& strInquire, long lTotalNumber, bool fSkipUnactiveStock) {
  return gl_ChinaStockMarket.GetTengxunInquiringStockStr(strInquire, lTotalNumber, fSkipUnactiveStock);
}

void CTengxunRTWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadTengxunRTData, this);
}

bool CTengxunRTWebData::ReportStatus(long lNumberOfData) {
  TRACE("����%d����Ѷʵʱ����\n", lNumberOfData);
  return true;
}