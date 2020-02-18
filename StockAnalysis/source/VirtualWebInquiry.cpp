#include"stdafx.h"
#include"afxinet.h"

#include"Thread.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_long CVirtualWebInquiry::m_lReadingThreadNumber = 0; // 当前执行网络读取线程数

CVirtualWebInquiry::CVirtualWebInquiry() {
  m_pFile = nullptr;
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
  m_lByteRead = 0;
  m_pCurrentReadPos = m_buffer;
  m_lCurrentByteRead = 0;
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

bool CVirtualWebInquiry::ReadWebData(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime) {
  CInternetSession session;
  m_pFile = nullptr;
  bool fDone = false;
  bool fStatus = true;
  m_pCurrentReadPos = GetBufferAddr();
  m_lReadingThreadNumber++;
  try {
    ASSERT(IsReadingWebData());
    SetByteReaded(0);
    ASSERT(m_pFile == nullptr);
    ASSERT(m_lCurrentByteRead == 0);
    ASSERT(m_pCurrentReadPos == m_buffer);
    m_pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)GetInquiringString()));
    Sleep(lFirstDelayTime); // 服务器延迟lStartDelayTime毫秒即可。
    while (!fDone) {
      do {
        ReadDataFromWebOnce();
      } while (m_lCurrentByteRead > 0);
      Sleep(lSecondDelayTime); // 等待lSecondDelayTime毫秒后再读一次，确认没有新数据后才返回。
      if (!ReadDataFromWebOnce()) {
        Sleep(lThirdDelayTime); // 等待lThirdDelayTime毫秒后读第三次，确认没有新数据后才返回，否则继续读。
        if (!ReadDataFromWebOnce()) {
          fDone = true;
        }
      }
    }
    *m_pCurrentReadPos = 0x000; // 最后以0x000结尾
  }
  catch (CInternetException * exception) {
    m_dwWebErrorCode = exception->m_dwError;
    if (exception->m_dwError == 404) {
      TRACE(_T("Error Code 404\n"));
    }
    TRACE(_T("%s net error\n"), m_strConnection.GetBuffer());
    fStatus = false;;
  }
  if (m_pFile) m_pFile->Close();
  if (m_pFile) {
    delete m_pFile;
    m_pFile = nullptr;
  }
  m_lReadingThreadNumber--;
  ASSERT(m_lReadingThreadNumber >= 0);
  SetReadingWebData(false);
  return fStatus;
}

bool CVirtualWebInquiry::ReadDataFromWebOnce(void) {
  m_lCurrentByteRead = m_pFile->Read(m_pCurrentReadPos, 1024);
  if (m_lCurrentByteRead > 0) {
    m_pCurrentReadPos += m_lCurrentByteRead;
    AddByteReaded(m_lCurrentByteRead);
    return true;
  }
  else return false;
}

CWebDataPtr CVirtualWebInquiry::TransferWebDataToQueueData() {
  char* pSrc = GetBufferAddr();
  CWebDataPtr pWebDataReceived = make_shared<CWebData>();
  pWebDataReceived->m_pDataBuffer = new char[GetByteReaded() + 1]; // 缓冲区需要多加一个字符长度（最后那个0x000）。
  char* pDest = pWebDataReceived->m_pDataBuffer;
  for (int i = 0; i < GetByteReaded() + 1; i++) {
    *pDest++ = *pSrc++;
  }
  pWebDataReceived->m_lBufferLength = GetByteReaded();
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
  long i;
  for (i = 0; i < lTotalNumber; i++) {
    m_buffer[i] = buffer[i];
  }
  m_buffer[lTotalNumber] = 0x000;
  m_lByteRead = lTotalNumber;
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
}

void CVirtualWebInquiry::__TESTSetBuffer(CString str) {
  long i;
  long lTotalNumber = str.GetLength();
  char* buffer = str.GetBuffer();
  for (i = 0; i < lTotalNumber; i++) {
    m_buffer[i] = buffer[i];
  }
  m_buffer[lTotalNumber] = 0x000;
  m_lByteRead = lTotalNumber;
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
}