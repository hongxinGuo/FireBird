#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include"boost/property_tree/xml_parser.hpp"

using namespace boost;
using namespace property_tree;
using namespace xml_parser;

#include "CrweberIndexWebData.h"

bool CCrweberIndexWebData::sm_fCreatedOnce = false; // ��ʼʱû�����ɹ�ʵ��

CCrweberIndexWebData::CCrweberIndexWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // ����Ѿ����ɹ�һ��ʵ���ˣ��򱨴�
  else sm_fCreatedOnce = true;

  m_strWebDataInquirePrefix = _T("http://www.crweber.com");
  m_strWebDataInquireSuffix = _T("");
}

CCrweberIndexWebData::~CCrweberIndexWebData() {
}

bool CCrweberIndexWebData::SucceedReadingAndStoringOneWebData(char*& pCurrentPos, long& iCount)
{
  iCount = m_lByteRead; //
  pCurrentPos += m_lByteRead;
  CFile file;
  file.Open(_T("C:\\StockAnalysis\\crweberIndex.html"), CFile::modeCreate | CFile::modeReadWrite);
  file.Write(m_buffer, m_lByteRead);
  file.Close();
  // Ȼ��ʹ��tidyxml����htmlת��Ϊxml�ļ���ʽ�洢
  return true;
}

void CCrweberIndexWebData::ProcessWebDataStored(void) {
  using boost::property_tree::ptree;

  // ��ȡxml�ļ���
  ptree pt;
  string str = _T("C:\\StockAnalysis\\crweberIndex.xml");
  //read_xml(str, pt);
  TRACE("crweber.com���ֽ���Ϊ%d\n", m_lByteRead);
}

void CCrweberIndexWebData::ReportDataError(void)
{
  CString str;
  str = _T("crweber.com data error");
  gl_systemMessage.PushInformationMessage(str);
}

void CCrweberIndexWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ��www.crweber.com\n");
  CString str;
  str = _T("Error reading http file : http://www.crweber.com");
  gl_systemMessage.PushInformationMessage(str);
}

void CCrweberIndexWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");

  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
  StartReadingThread();
}

int CCrweberIndexWebData::GetInquiringStr(CString& strInquire) {
  strInquire = _T("");
  return 0;
}

void CCrweberIndexWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadCrweberIndex, nullptr);
}