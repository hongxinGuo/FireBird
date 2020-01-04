#include"stdafx.h"
#include"Thread.h"

#include"globedef.h"
#include"Market.h"

#include"WebData.h"

CWebData::CWebData() {
  m_pFile = nullptr;
  m_lCurrentPos = 0;
  m_lByteRead = 0;
  m_lCurrentReadPos = 0;
  m_lCurrentByteRead = 0;
  m_strInquire = _T("");
  m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
  m_fReadingWebData = false; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ

#ifdef DEBUG
  m_fReportStatus = false;
#else
  m_fReportStatus = false;
#endif
}

bool CWebData::ReadWebData(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime) {
  CInternetSession session;
  m_pFile = nullptr;
  bool fDone = false;
  m_lCurrentReadPos = 0;

  try {
    ASSERT(IsReadingWebData());
    SetByteReaded(0);
    ASSERT(m_pFile == nullptr);
    ASSERT(m_lCurrentByteRead == 0);
    ASSERT(m_lCurrentReadPos == 0);
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
    m_buffer.at(m_lCurrentReadPos) = 0x000; // �����0x000��β
  }
  catch (CInternetException*) {
    TRACE(_T("net error\n"));
    return false;
  }
  if (m_pFile) m_pFile->Close();
  if (m_pFile) {
    delete m_pFile;
    m_pFile = nullptr;
  }
  SetReadingWebData(false);

  return true;
}

bool CWebData::ReadDataFromWebOnce(void) {
  char buffer[2048];
  m_lCurrentByteRead = m_pFile->Read(buffer, 1024);
  if (m_lCurrentByteRead > 0) {
    for (int i = 0; i < m_lCurrentByteRead; i++) {
      m_buffer.at(m_lCurrentReadPos++) = buffer[i];
    }
    AddByteReaded(m_lCurrentByteRead);
    return true;
  }
  else return false;
}

CWebDataReceivedPtr CWebData::TransferWebDataToQueueData() {
  CWebDataReceivedPtr pWebDataReceived = make_shared<CWebDataReceived>();
  pWebDataReceived->m_pDataBuffer = new char[GetByteReaded() + 1]; // ��������Ҫ���һ���ַ����ȣ�����Ǹ�0x000����
  pWebDataReceived->m_lBufferLength = GetByteReaded();
  char* pbuffer = pWebDataReceived->m_pDataBuffer;

  for (int i = 0; i < GetByteReaded() + 1; i++) {
    *pbuffer++ = m_buffer.at(i);
  }
  return pWebDataReceived;
}

/////////////////////////////////////////////////////////////////////////
//
// ���Ǵ���Ψһ�Ľӿں���
//
//////////////////////////////////////////////////////////////////////////
bool CWebData::GetWebData(void) {
  if (!IsReadingWebData()) {
    InquireNextWebData();
  }
  return true;
}

bool CWebData::ReportStatus(long lNumberOfData) {
  TRACE("����%d��ʵʱ����\n", lNumberOfData);
  return true;
}

void CWebData::CreateTotalInquiringString(CString strMiddle) {
  m_strInquire = m_strWebDataInquirePrefix + strMiddle + m_strWebDataInquireSuffix;
}

void CWebData::TransferWebDataToBuffer(vector<char>& buffer) {
  for (int i = 0; i < m_lByteRead + 1; i++) {
    buffer.at(i) = m_buffer.at(i);
  }
}

void CWebData::__TESTSetBuffer(char* buffer, long lTotalNumber) {
  long i;
  for (i = 0; i < lTotalNumber; i++) {
    m_buffer[i] = buffer[i];
  }
  m_buffer[lTotalNumber] = 0x000;
  m_lByteRead = lTotalNumber;
  m_lCurrentPos = 0;
}

void CWebData::__TESTSetBuffer(CString str) {
  long i;
  long lTotalNumber = str.GetLength();
  char* buffer = str.GetBuffer();
  for (i = 0; i < lTotalNumber; i++) {
    m_buffer[i] = buffer[i];
  }
  m_buffer[lTotalNumber] = 0x000;
  m_lByteRead = lTotalNumber;
  m_lCurrentPos = 0;
}