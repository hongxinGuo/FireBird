#include"stdafx.h"
#include"afxinet.h"

#include"Thread.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_long CVirtualWebInquiry::m_lTotalByteReaded = 0;

CVirtualWebInquiry::CVirtualWebInquiry() : CObject() {
  m_pFile = nullptr;
  m_lByteRead = 0;
  m_lByteReadCurrent = 0;
  m_fWebError = false;
  m_dwWebErrorCode = 0;
  m_strInquire = _T("");
  m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
  m_fReadingWebData = false; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ

  m_lInquiringNumber = 500; // ÿ�β�ѯ����Ĭ��ֵΪ500

  m_tCurrentInquiryTime = 0;

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

///////////////////////////////////////////////////////////////////////////
//
// �������ȡ���ݡ�ÿ�ζ�1KB��ֱ��������Ϊֹ��
//
//
///////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::ReadWebData(void) {
  CInternetSession session(_T("�������Ϊ�գ������ʱ����ֶ��Դ��󡣵���Ӱ����Խ��"));
  m_pFile = nullptr;
  bool fStatus = true;
  CString str1, strLeft;
  ASSERT(IsReadingWebData());
  ASSERT(m_pFile == nullptr);
  time_t tt = 0;
  char buffer[1024];

  gl_ThreadStatus.IncreaseWebInquiringThread();

  SetWebError(false);
  SetByteReaded(0);
  tt = GetTickCount64();
  try {    // ʹ��try���󣬳���exception����ʱm_pFile == NULL����ת��catch����С�
    m_pFile = dynamic_cast<CHttpFile*>(session.OpenURL((LPCTSTR)GetInquiringString()));
    do {
      m_lByteReadCurrent = m_pFile->Read(buffer, 1024); // Ŀǰ����һ�ζ�ȡ��ֵ����Ϊ4M��������8M���㹻�ˡ�
      ASSERT(m_lByteReadCurrent <= 1024);
      for (long l = 0; l < m_lByteReadCurrent; l++) {
        m_buffer[m_lByteRead++] = buffer[l];
      }
    } while (m_lByteReadCurrent > 0);
    ASSERT(m_lByteRead < 1024 * 1024 * 16);
    m_lTotalByteReaded += m_lByteRead;
    m_buffer[m_lByteRead] = 0x000; // �����0x000��β
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
  m_tCurrentInquiryTime = GetTickCount64() - tt;
  gl_ThreadStatus.DecreaseWebInquiringThread();

  ASSERT(gl_ThreadStatus.GetNumberOfWebInquiringThread() >= 0);
  return fStatus;
}

///////////////////////////////////////////////////////////////////////////
//
// �������ȡ���ݡ�ÿ�ζ�1KB���������ȴ����ɺ��룬���ȴ����Ρ�
//
//
///////////////////////////////////////////////////////////////////////////

bool CVirtualWebInquiry::ReadWebData3(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime) {
  CInternetSession session(_T("�������Ϊ�գ������ʱ����ֶ��Դ��󡣵���Ӱ����Խ��"));
  m_pFile = nullptr;
  bool fDone = false;
  bool fStatus = true;
  CString str1, strLeft;
  time_t tt = 0;

  ASSERT(IsReadingWebData());
  ASSERT(m_pFile == nullptr);

  gl_ThreadStatus.IncreaseWebInquiringThread();

  long lCurrentByteReaded = 0;
  SetWebError(false);
  SetByteReaded(0);
  tt = GetTickCount64();
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
    ASSERT(m_lByteRead < 1024 * 1024 * 16);
    m_buffer[m_lByteRead] = 0x000; // �����0x000��β
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
  m_tCurrentInquiryTime = GetTickCount64() - tt;
  gl_ThreadStatus.DecreaseWebInquiringThread();
  ASSERT(gl_ThreadStatus.GetNumberOfWebInquiringThread() >= 0);
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
  for (int i = 0; i < GetByteReaded() + 1; i++) {// ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x00)
    pWebDataReceived->SetData(i, m_buffer[i]);
  }
  pWebDataReceived->SetBufferLength(GetByteReaded());
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