#include"stdafx.h"
#include"globedef.h"
#include"thread.h"
#include"market.h"

#include"Accessory.h"

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

bool CCrweberIndexWebData::SucceedReadingAndStoringOneWebData(void)
{
  m_lCurrentPos = 0;
  CString str, str1, strHead = _T("");
  CString strValue, strTime;
  long lUpdateDay = 0;

  while (m_lCurrentPos < m_lByteRead) {
    str = GetNextString();
    strHead = str.Left(10);
    if (strHead.Compare(_T("Updated by")) == 0) {
      strTime = GetNextString(); // 当前时间
      lUpdateDay = ConvertStringToTime(strTime);

      for (int i = 0; i < 4; i++) GetNextString(); // 抛掉4个没用字符串

      str1 = GetNextString(); // "VLCC"
      gl_CrweberIndex.m_dTD1 = GetOneValue();
      gl_CrweberIndex.m_dTD2 = GetOneValue();
      gl_CrweberIndex.m_dTD3C = GetOneValue();
      gl_CrweberIndex.m_dTD15 = GetOneValue();
      gl_CrweberIndex.m_dVLCC_USGSPORE = GetOneValue();

      str1 = GetNextString(); // "SUEZMAX"
      gl_CrweberIndex.m_dTD5 = GetOneValue();
      gl_CrweberIndex.m_dTD20 = GetOneValue();
      gl_CrweberIndex.m_dTD6 = GetOneValue();
      gl_CrweberIndex.m_dSUEZMAX_CBSUSG = GetOneValue();

      str1 = GetNextString(); // "AFRAMAX"
      gl_CrweberIndex.m_dTD7 = GetOneValue();
      gl_CrweberIndex.m_dTD9 = GetOneValue();
      gl_CrweberIndex.m_dTD19 = GetOneValue();
      gl_CrweberIndex.m_dTD8 = GetOneValue();

      str1 = GetNextString(); // "PANAMAX"
      gl_CrweberIndex.m_dTD21 = GetOneValue();
      gl_CrweberIndex.m_dTD12 = GetOneValue();

      str1 = GetNextString(); // "CPP"
      gl_CrweberIndex.m_dTC2 = GetOneValue();
      gl_CrweberIndex.m_dTC3 = GetOneValue();
      gl_CrweberIndex.m_dTC14 = GetOneValue();
      gl_CrweberIndex.m_dCPP_USGCBS = GetOneValue();
      gl_CrweberIndex.m_dTC1 = GetOneValue();
      gl_CrweberIndex.m_dTC5 = GetOneValue();
      gl_CrweberIndex.m_dTC4 = GetOneValue();

      CString strDay = ConvertValueToString(lUpdateDay, 1);
      gl_systemMessage.PushInnerSystemInformationMessage(strDay);
      TRACE("m_lastUpdateDay = %d,  m_lDay = %d, m_UpdateDay = %d\n", gl_CrweberIndex.m_lLastUpdateDay, gl_CrweberIndex.m_lDay, lUpdateDay);
      if (lUpdateDay > gl_CrweberIndex.m_lLastUpdateDay) {
        gl_CrweberIndex.m_lDay = lUpdateDay;
        gl_CrweberIndex.m_lLastUpdateDay = lUpdateDay;
        gl_CrweberIndex.m_fTodayUpdated = true;
      }
    }

    strHead = str.Left(6);
    if (strHead.Compare(_T("Tanker")) == 0) {
      for (int i = 0; i < 7; i++) GetNextString(); // "CPP"
      str1 = GetNextString();
      gl_CrweberIndex.m_dVLCC_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString();
      gl_CrweberIndex.m_dVLCC_TC_3YEAR = ConvertStringToTC(str1);

      GetNextString();
      GetNextString();
      str1 = GetNextString();
      gl_CrweberIndex.m_dSUEZMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString();
      gl_CrweberIndex.m_dSUEZMAX_TC_3YEAR = ConvertStringToTC(str1);

      GetNextString();
      GetNextString();
      str1 = GetNextString();
      gl_CrweberIndex.m_dAFRAMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString();
      gl_CrweberIndex.m_dAFRAMAX_TC_3YEAR = ConvertStringToTC(str1);

      GetNextString();
      GetNextString();
      str1 = GetNextString();
      gl_CrweberIndex.m_dPANAMAX_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString();
      gl_CrweberIndex.m_dPANAMAX_TC_3YEAR = ConvertStringToTC(str1);

      GetNextString();
      GetNextString();
      str1 = GetNextString();
      gl_CrweberIndex.m_dMR_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString();
      gl_CrweberIndex.m_dMR_TC_3YEAR = ConvertStringToTC(str1);

      GetNextString();
      GetNextString();
      str1 = GetNextString();
      gl_CrweberIndex.m_dHANDY_TC_1YEAR = ConvertStringToTC(str1);
      str1 = GetNextString();
      gl_CrweberIndex.m_dHANDY_TC_3YEAR = ConvertStringToTC(str1);

      m_lCurrentPos = m_lByteRead; //
    }
  }
  for (int i = 0; i < 1024 * 1024; i++) {
    m_buffer[i] = 0x000;
  }
  m_lCurrentPos = m_lByteRead; //

  return true;
}

double CCrweberIndexWebData::ConvertStringToTC(CString str) {
  char buffer[200];
  int iTotal = str.GetLength();
  int i = 0, j = 0;

  while (i < iTotal) {
    if ((str[i] != ',') && (str[i] != ' ')) buffer[j++] = str[i];
    i++;
  }
  buffer[j] = 0x000;
  double abc = atof(buffer);

  return abc;
}

long CCrweberIndexWebData::ConvertStringToTime(CString str) {
  char buffer1[20];
  char* pChar = str.GetBuffer();
  while (*pChar != ' ') pChar++;
  pChar++;
  int i = 0;
  while (*pChar != ' ') buffer1[i++] = *pChar++;
  pChar++;
  buffer1[i] = 0x000;
  CString strTime = buffer1;
  int month = 1, day, year;
  if (strTime.Compare(_T("October")) == 0) month = 10;
  else if (strTime.Compare(_T("November")) == 0) month = 11;
  else if (strTime.Compare(_T("December")) == 0) month = 12;

  i = 0;
  while (*pChar != ' ') buffer1[i++] = *pChar++;
  pChar++;
  buffer1[i] = 0x000;
  strTime = buffer1;
  day = atol(strTime);
  i = 0;
  while (*pChar != ' ') buffer1[i++] = *pChar++;
  buffer1[i] = 0x000;
  strTime = buffer1;
  year = atol(strTime);

  return year * 10000 + month * 100 + day;
}

double CCrweberIndexWebData::GetOneValue(void) {
  CString str, strValue;
  double dValue = 0.0;

  str = GetNextString(); // "TD1\r\n   "
  GetNextString(); // 无用数据
  GetNextString(); // 无用数据
  strValue = GetNextString(); // TD1指数的当前值
  dValue = atof(strValue.GetBuffer());
  return dValue;
}

CString CCrweberIndexWebData::GetNextString(void) {
  bool fFound = false;
  char buffer[10000];
  long iBufferCount = 0;

  while ((*m_pCurrentPos != 0x000) && !fFound) {
    if (*m_pCurrentPos == '<') { // 无用配置字符
      while (*m_pCurrentPos != '>') {
        IncreaseCurrentPos();
      }
      IncreaseCurrentPos();
      while ((*m_pCurrentPos == 0x00a) || (*m_pCurrentPos == 0x00d) || (*m_pCurrentPos == ' ')) { // 跨过回车、换行和空格符
        IncreaseCurrentPos();
      }
    }
    else fFound = true;
  }
  if (*m_pCurrentPos == 0x000) { // 读到结尾处了
    ASSERT(m_lCurrentPos >= m_lByteRead);
    return _T("");
  }
  while (*m_pCurrentPos != '<') {
    buffer[iBufferCount++] = *m_pCurrentPos++;
    m_lCurrentPos++;
  }
  buffer[iBufferCount] = 0x000;
  CString str;
  str = buffer;
  return str;
}

void CCrweberIndexWebData::ProcessWebDataStored(void) {
  if (gl_CrweberIndex.IsTodayUpdated()) {
    gl_ChinaStockMarket.SaveCrweberIndexData();
    gl_systemMessage.PushInformationMessage(_T("crweber油运指数已更新"));
    gl_CrweberIndex.m_fTodayUpdated = false;
  }
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

int CCrweberIndexWebData::GetInquiringStr(CString& strInquire, long, bool) {
  strInquire = _T("");
  return 0;
}

void CCrweberIndexWebData::StartReadingThread(void) {
  AfxBeginThread(ThreadReadCrweberIndex, nullptr);
}