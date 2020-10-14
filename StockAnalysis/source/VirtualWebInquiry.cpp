#include"stdafx.h"
#include"afxinet.h"

#include"Thread.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_long CVirtualWebInquiry::m_lReadingThreadNumber = 0; // ��ǰִ�������ȡ�߳���

CVirtualWebInquiry::CVirtualWebInquiry() {
  m_pFile = nullptr;
  m_buffer.resize(2048 * 1024);
  m_lByteRead = 0;
  m_fWebError = false;
  m_dwWebErrorCode = 0;
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

void CVirtualWebInquiry::Reset(void) noexcept {
  m_lByteRead = 0;
  m_dwWebErrorCode = 0;
  m_fWebError = false;
}

bool CVirtualWebInquiry::ReadWebData(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime) {
  CInternetSession session(_T("�������Ϊ�գ������ʱ����ֶ��Դ��󡣵���Ӱ����Խ��"));
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
    // ʹ��try���󣬳���exception����ʱm_pFile == NULL����ת��catch����С�
    Sleep(lFirstDelayTime); // �������ӳ�lStartDelayTime���뼴�ɡ�
    while (!fDone) {
      do {
        ReadDataFromWebOnce(lCurrentByteReaded);
      } while (lCurrentByteReaded > 0);
      Sleep(lSecondDelayTime); // �ȴ�lSecondDelayTime������ٶ�һ�Σ�ȷ��û�������ݺ�ŷ��ء�
      if (!ReadDataFromWebOnce(lCurrentByteReaded)) {
        Sleep(lThirdDelayTime); // �ȴ�lThirdDelayTime�����������Σ�ȷ��û�������ݺ�ŷ��أ������������
        if (!ReadDataFromWebOnce(lCurrentByteReaded)) {
          fDone = true;
        }
      }
    }
    m_buffer.at(m_lByteRead) = 0x000; // �����0x000��β
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
    m_buffer.at(m_lByteRead + i) = buffer[i];
  }
  return uByteRead;
}

CWebDataPtr CVirtualWebInquiry::TransferWebDataToQueueData() {
  CWebDataPtr pWebDataReceived = make_shared<CWebData>();
  pWebDataReceived->m_pDataBuffer = new char[GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
  char* pDest = pWebDataReceived->m_pDataBuffer;
  for (int i = 0; i < GetByteReaded() + 1; i++) {
    *pDest++ = m_buffer.at(i);
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
    m_buffer.at(i) = buffer[i];
  }
  m_buffer.at(lTotalNumber) = 0x000;
  m_lByteRead = lTotalNumber;
}

void CVirtualWebInquiry::__TESTSetBuffer(CString str) {
  long i{ 0 };
  long lTotalNumber = str.GetLength();
  char* buffer = str.GetBuffer();
  for (i = 0; i < lTotalNumber; i++) {
    m_buffer.at(i) = buffer[i];
  }
  m_buffer.at(lTotalNumber) = 0x000;
  m_lByteRead = lTotalNumber;
}