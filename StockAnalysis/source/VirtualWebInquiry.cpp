#include"stdafx.h"
#include"afxinet.h"

#include"Thread.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_long CVirtualWebInquiry::m_lReadingThreadNumber = 0; // ��ǰִ�������ȡ�߳���

CVirtualWebInquiry::CVirtualWebInquiry() {
  m_pFile = nullptr;
  m_pCurrentPos = m_buffer;
  m_lCurrentPos = 0;
  m_lByteRead = 0;
  m_pCurrentReadPos = m_buffer;
  m_lCurrentByteRead = 0;
  m_strInquire = _T("");
  m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
  m_fReadingWebData = false; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ

  m_lInquiringNumber = 500; // ÿ�β�ѯ����Ĭ��ֵΪ500

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
    Sleep(lFirstDelayTime); // �������ӳ�lStartDelayTime���뼴�ɡ�
    while (!fDone) {
      do {
        ReadDataFromWebOnce();
      } while (m_lCurrentByteRead > 0);
      Sleep(lSecondDelayTime); // �ȴ�lSecondDelayTime������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      if (!ReadDataFromWebOnce()) {
        Sleep(lThirdDelayTime); // �ȴ�lThirdDelayTime�����������Σ�ȷ��û�������ݺ�ŷ��أ������������
        if (!ReadDataFromWebOnce()) {
          fDone = true;
        }
      }
    }
    *m_pCurrentReadPos = 0x000; // �����0x000��β
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
  pWebDataReceived->m_pDataBuffer = new char[GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
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
// ���Ǵ���Ψһ�Ľӿں���
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
  if (!IsReadingWebData()) {
    if (PrepareNextInquiringStr()) {
      SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
      StartReadingThread();
      return true;
    }
    else return false;
  }
  else return false;
}

bool CVirtualWebInquiry::ReportStatus(long lNumberOfData) {
  TRACE("����%d��ʵʱ����\n", lNumberOfData);
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