#include"stdafx.h"
#include"afxinet.h"

#include"Thread.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_long CVirtualWebInquiry::m_lReadingThreadNumber = 0; // 当前执行网络读取线程数
atomic_long CVirtualWebInquiry::m_lTotalByteReaded = 0;

CVirtualWebInquiry::CVirtualWebInquiry() {
  m_pFile = nullptr;
  m_lByteRead = 0;
  m_lByteReadCurrent = 0;
  m_fWebError = false;
  m_dwWebErrorCode = 0;
  m_strInquire = _T("");
  m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
  m_fReadingWebData = false; // 接收实时数据线程是否执行标识

  m_lInquiringNumber = 500; // 每次查询数量默认值为500

#ifdef DEBUG
  m_fReportStatus = false;
#else
  m_fReportStatus = false;
#endif
}

CVirtualWebInquiry::~CVirtualWebInquiry(void) {
}

void CVirtualWebInquiry::Reset(void) noexcept {
  m_lByteRead = 0;
  m_lByteReadCurrent = 0;
  m_dwWebErrorCode = 0;
  m_fWebError = false;
}

bool CVirtualWebInquiry::ReadWebData3(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime) {
  CInternetSession session(_T("如果此项为空，则测试时会出现断言错误。但不影响测试结果"));
  m_pFile = nullptr;
  bool fDone = false;
  bool fStatus = true;
  CString str1, strLeft;

  ASSERT(IsReadingWebData());
  ASSERT(m_pFile == nullptr);

  m_lReadingThreadNumber++;

  long lCurrentByteReaded = 0;
  SetWebError(false);
  SetByteReaded(0);
  try {
    m_pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)GetInquiringString()));
    // 使用try语句后，出现exception（此时m_pFile == NULL）会转至catch语句中。
    Sleep(lFirstDelayTime); // 服务器延迟lStartDelayTime毫秒即可。
    while (!fDone) {
      do {
        ReadDataFromWebOnce(lCurrentByteReaded);
      } while (lCurrentByteReaded > 0);
      Sleep(lSecondDelayTime); // 等待lSecondDelayTime毫秒后再读一次，确认没有新数据后才返回。
      if (!ReadDataFromWebOnce(lCurrentByteReaded)) {
        Sleep(lThirdDelayTime); // 等待lThirdDelayTime毫秒后读第三次，确认没有新数据后才返回，否则继续读。
        if (!ReadDataFromWebOnce(lCurrentByteReaded)) {
          fDone = true;
        }
      }
    }
    m_buffer[m_lByteRead] = 0x000; // 最后以0x000结尾
    m_pFile->Close();

    m_lTotalByteReaded += m_lByteRead; //
  }
  catch (CInternetException* exception) {
    SetWebError(true);
    if (m_pFile != nullptr) m_pFile->Close();
    m_dwWebErrorCode = exception->m_dwError;
    str1 = GetInquiringString();

    strLeft = str1.Left(80);
    TRACE(_T("%s net error, Error Code %d\n"), (LPCTSTR)strLeft, exception->m_dwError);
    str1 = _T("Error Web : ") + strLeft + _T("\n");
    gl_systemMessage.PushInnerSystemInformationMessage(str1);
    fStatus = false;
    exception->Delete();
  }
  if (m_pFile != nullptr) {
    delete m_pFile;
    m_pFile = nullptr;
  }
  m_lReadingThreadNumber--;
  ASSERT(m_lReadingThreadNumber >= 0);
  SetReadingWebData(false);
  return fStatus;
}

bool CVirtualWebInquiry::ReadWebData(void) {
  CInternetSession session(_T("如果此项为空，则测试时会出现断言错误。但不影响测试结果"));
  m_pFile = nullptr;
  bool fStatus = true;
  CString str1, strLeft;
  ASSERT(IsReadingWebData());
  ASSERT(m_pFile == nullptr);
  char buffer[1000];
  long lTemp = 0;

  m_lReadingThreadNumber++;

  SetWebError(false);
  SetByteReaded(0);
  try {    // 使用try语句后，出现exception（此时m_pFile == NULL）会转至catch语句中。
    m_pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)GetInquiringString()));
    do {
      m_lByteReadCurrent = m_pFile->Read(m_bufferTemp, 1024); // 目前最大的一次读取数值大致为1M，故设置2M即足够了。
      for (long l = 0; l < m_lByteReadCurrent; l++) {
        m_buffer[m_lByteRead++] = m_bufferTemp[l];
      }
    } while (m_lByteReadCurrent > 0);
    ASSERT(m_lByteRead < 1024 * 1024 * 8);
    m_lTotalByteReaded += m_lByteRead;
    m_buffer[m_lByteRead] = 0x000; // 最后以0x000结尾
    /*
    if ((lTemp = m_pFile->Read(buffer, 1000)) > 0) {
      TRACE("overflow\n");
    }
    */
    m_pFile->Close();
  }
  catch (CInternetException* exception) {
    SetWebError(true);
    if (m_pFile != nullptr) m_pFile->Close();
    m_dwWebErrorCode = exception->m_dwError;
    str1 = GetInquiringString();

    strLeft = str1.Left(80);
    TRACE(_T("%s net error, Error Code %d\n"), (LPCTSTR)strLeft, exception->m_dwError);
    str1 = _T("Error Web : ") + strLeft + _T("\n");
    gl_systemMessage.PushInnerSystemInformationMessage(str1);
    fStatus = false;
    exception->Delete();
  }
  if (m_pFile != nullptr) {
    delete m_pFile;
    m_pFile = nullptr;
  }
  m_lReadingThreadNumber--;
  ASSERT(m_lReadingThreadNumber >= 0);
  SetReadingWebData(false);
  return fStatus;
}

bool CVirtualWebInquiry::ReadDataFromWebOnce(long& lCurrentByteReaded) {
  lCurrentByteReaded = ReadWebFile();
  if (lCurrentByteReaded > 0) {
    AddByteReaded(lCurrentByteReaded);
    return true;
  }
  else return false;
}

UINT CVirtualWebInquiry::ReadWebFile(void) {
  char buffer[1024];
  const UINT uByteRead = m_pFile->Read(buffer, 1024);
  for (int i = 0; i < uByteRead; i++) {
    m_buffer[m_lByteRead + i] = buffer[i];
  }
  return uByteRead;
}

CWebDataPtr CVirtualWebInquiry::TransferWebDataToQueueData() {
  CWebDataPtr pWebDataReceived = make_shared<CWebData>();
  pWebDataReceived->Resize(GetByteReaded() + 1);
  for (int i = 0; i < GetByteReaded() + 1; i++) {// 缓冲区需要多加一个字符长度（最后那个0x00)
    pWebDataReceived->SetData(i, m_buffer[i]);
  }
  pWebDataReceived->SetBufferLength(GetByteReaded());
  pWebDataReceived->ResetCurrentPos();
  return pWebDataReceived;
}

/////////////////////////////////////////////////////////////////////////
//
// 这是此类唯一的接口函数
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
  if (!IsReadingWebData()) {
    if (PrepareNextInquiringStr()) {
      SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
      StartReadingThread();
      return true;
    }
    else return false;
  }
  else return false;
}

bool CVirtualWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("读入%d个实时数据\n", lNumberOfData);
  return true;
}

void CVirtualWebInquiry::CreateTotalInquiringString(CString strMiddle) {
  m_strInquire = m_strWebDataInquirePrefix + strMiddle + m_strWebDataInquireSuffix;
}

void CVirtualWebInquiry::__TESTSetBuffer(char* buffer, long lTotalNumber) {
  for (long i = 0; i < lTotalNumber; i++) {
    m_buffer[i] = buffer[i];
  }
  m_buffer[lTotalNumber] = 0x000;
  m_lByteRead = lTotalNumber;
}

void CVirtualWebInquiry::__TESTSetBuffer(CString str) {
  long lTotalNumber = str.GetLength();
  char* buffer = str.GetBuffer();
  for (long i = 0; i < lTotalNumber; i++) {
    m_buffer[i] = buffer[i];
  }
  m_buffer[lTotalNumber] = 0x000;
  m_lByteRead = lTotalNumber;
}