#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include"boost/property_tree/xml_parser.hpp"

using namespace boost;
using namespace property_tree;
using namespace xml_parser;

#include "CrweberIndexWebData.h"

bool CCrweberIndexWebData::sm_fCreatedOnce = false; // 初始时没有生成过实例

CCrweberIndexWebData::CCrweberIndexWebData() : CWebData() {
  if (sm_fCreatedOnce) ASSERT(0); // 如果已经生成过一个实例了，则报错
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
  // 然后使用tidyxml将此html转换为xml文件制式存储
  return true;
}

void CCrweberIndexWebData::ProcessWebDataStored(void) {
  using boost::property_tree::ptree;

  // 读取xml文件。
  ptree pt;
  string str = _T("C:\\StockAnalysis\\crweberIndex.xml");
  //read_xml(str, pt);
  TRACE("crweber.com的字节数为%d\n", m_lByteRead);
}

void CCrweberIndexWebData::ReportDataError(void)
{
  CString str;
  str = _T("crweber.com data error");
  gl_systemMessage.PushInformationMessage(str);
}

void CCrweberIndexWebData::ReportCommunicationError(void)
{
  TRACE("Error reading http file ：www.crweber.com\n");
  CString str;
  str = _T("Error reading http file : http://www.crweber.com");
  gl_systemMessage.PushInformationMessage(str);
}

void CCrweberIndexWebData::InquireNextWebData(void)
{
  CString strMiddle = _T("");

  CreateTotalInquiringString(strMiddle);
  SetWebDataReceived(false);
  SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
  StartReadingThread();
}

int CCrweberIndexWebData::GetInquiringStr(CString& strInquire) {
  strInquire = _T("");
  return 0;
}

void CCrweberIndexWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadCrweberIndex, nullptr);
}