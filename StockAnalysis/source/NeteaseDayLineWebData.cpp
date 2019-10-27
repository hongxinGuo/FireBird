#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include "NeteaseDayLineWebData.h"

CNeteaseDayLineWebData::CNeteaseDayLineWebData() : CWebData() {
  m_fNeedProcessingCurrentWebData = true;

  m_strWebDataInquirePrefix = _T("http://quotes.money.163.com/service/chddata.html?code=");
  m_strWebDataInquireSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
}

CNeteaseDayLineWebData::~CNeteaseDayLineWebData() {
}

bool CNeteaseDayLineWebData::IsNeedProcessingCurrentWebData(void)
{
  if (m_fNeedProcessingCurrentWebData) return true;
  else return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���׵�������ʷ���ݲ���Ҫǰ�ڴ���ֱ��ʹ��ProcessWebDataStored�������ɡ��ʶ��˴�ֻ�ǽ�iCount����m_lByteRead���ɡ�
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  iCount = m_lByteRead; //
  pCurrentPos += m_lByteRead;
  return true;
}

void CNeteaseDayLineWebData::ProcessWebDataStored(void) {
  TRACE("��Ʊ%s��������Ϊ%d�ֽ�\n", m_strDownLoadingStockCode, m_lByteRead);
  ASSERT(m_lByteRead < 2048 * 1024);
  gl_ChinaStockMarket.ProcessDayLineData(this);
}

void CNeteaseDayLineWebData::ReportDataError(void)
{
  TRACE("����������ʷ��������,�׵�����\n");
  CString str;
  str = _T("����������ʷ��������");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseDayLineWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��hq.sinajs.cn\n");
  CString str;
  str = _T("Error reading http file ��http://quotes.money.163.com");
  gl_systemMessage.PushInformationMessage(str);
}

void CNeteaseDayLineWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");
  char buffer2[200];
  CString strStartDay;

  // ׼�������������������ʽ
  m_fNeedProcessingCurrentWebData = gl_ChinaStockMarket.CreateNeteaseDayLineInquiringStr(strMiddle, strStartDay);
  if (m_fNeedProcessingCurrentWebData) {
    SetDownLoadingStockCode(strMiddle);
    strMiddle += _T("&start=");
    strMiddle += strStartDay;
    strMiddle += _T("&end=");
    sprintf_s(buffer2, "%8d", gl_systemTime.GetDay());
    strMiddle += buffer2;
    CreateTotalInquiringString(strMiddle);
  }
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CNeteaseDayLineWebData::GetInquiringStr(CString& strInquire) {
  strInquire = _T("");
  return 0;
}

void CNeteaseDayLineWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadNeteaseDayLine, (LPVOID)this);
}

void CNeteaseDayLineWebData::SetDownLoadingStockCode(CString strStockCode)
{
  CString str = strStockCode.Left(1);
  CString strRight = strStockCode.Right(6);
  if (str.Compare(_T("0")) == 0) {
    m_strDownLoadingStockCode = _T("sh");
    m_strDownLoadingStockCode += strRight;
  }
  else if (str.Compare(_T("1")) == 0) {
    m_strDownLoadingStockCode = _T("sz");
    m_strDownLoadingStockCode += strRight;
  }
  else {
    m_strDownLoadingStockCode = strStockCode;
  }
}