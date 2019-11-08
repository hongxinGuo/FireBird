#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "NetEaseRTWebDataForBoost.h"

CNetEaseRTWebDataForBoost::CNetEaseRTWebDataForBoost() : CWebData() {
  m_fNeedProcessingCurrentWebData = true;

  m_strWebDataInquirePrefix = _T("http://api.money.126.net/data/feed/");
  m_strWebDataInquireSuffix = _T("");
}

CNetEaseRTWebDataForBoost::~CNetEaseRTWebDataForBoost() {
}

bool CNetEaseRTWebDataForBoost::IsNeedProcessingCurrentWebData(void)
{
  if (m_fNeedProcessingCurrentWebData) return true;
  else return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���׵�������ʷ���ݲ���Ҫǰ�ڴ���ֱ��ʹ��ProcessWebDataStored�������ɡ��ʶ��˴�ֻ�ǽ�iCount����m_lByteRead���ɡ�
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNetEaseRTWebDataForBoost::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  pCurrentPos[m_lByteRead] = 0x000;
  iCount = m_lByteRead; //
  pCurrentPos += m_lByteRead;
  return true;
}

void CNetEaseRTWebDataForBoost::ProcessWebDataStored(void) {
  TRACE(_T("��Ʊ%s��������Ϊ%d�ֽ�\n"), m_strDownLoadingStockCode, m_lByteRead);
  ASSERT(m_lByteRead < 2048 * 1024);
  gl_ChinaStockMarket.ReadNeteaseRTDataUsingPropertyTree(this);
}

void CNetEaseRTWebDataForBoost::ReportDataError(void)
{
  TRACE("����ʵʱ��������,�׵�����\n");
  CString str;
  str = _T("����ʵʱ��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CNetEaseRTWebDataForBoost::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��http://api.money.126.net/data/feed/\n");
  CString str;
  str = _T("Error reading http file ��http://api.money.126.net/data/feed/");
  gl_systemMessage.PushInformationMessage(str);
}

void CNetEaseRTWebDataForBoost::InquireNextWebData(void)
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

int CNetEaseRTWebDataForBoost::GetInquiringStr(CString& strInquire) {
  return gl_ChinaStockMarket.GetNeteaseInquiringStockStr(strInquire);
}

void CNetEaseRTWebDataForBoost::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseRTData, nullptr);
}