#include"WebData.h"

bool CWebData::GetData(char* buffer, long lDataLength, long lStartPosition) {
  if (lDataLength + lStartPosition > m_lBufferLength) return false;
  for (long i = 0; i < lDataLength; i++) {
    buffer[i] = m_pDataBuffer[i + lStartPosition];
  }
  return true;
}

bool CWebData::GetData(char* buffer, long lDataLength) {
  if (lDataLength + m_lCurrentPos > m_lBufferLength) return false;
  for (long i = 0; i < lDataLength; i++) {
    buffer[i] = m_pDataBuffer[i + m_lCurrentPos];
  }
  return true;
}

bool CWebData::SetData(char* buffer, long lDataLength, long lStartPosition) {
  if (lDataLength + lStartPosition > m_lBufferLength) return false;
  for (long i = 0; i < lDataLength; i++) {
    m_pDataBuffer[i + lStartPosition] = buffer[i];
  }
  return true;
}

bool CWebData::SetData(char* buffer, long lDataLength) {
  if (lDataLength + m_lCurrentPos > m_lBufferLength) return false;
  for (long i = 0; i < lDataLength; i++) {
    m_pDataBuffer[i + m_lCurrentPos] = buffer[i];
  }
  return true;
}

void CWebData::__TEST_SetBuffer__(CString strBuffer) {
  //m_pDataBuffer = new char[strBuffer.GetLength() + 1];
  m_lBufferLength = strBuffer.GetLength();
  char* pBuffer = strBuffer.GetBuffer();
  for (int i = 0; i < m_lBufferLength; i++) { m_pDataBuffer[i] = pBuffer[i]; }
  m_pDataBuffer[m_lBufferLength] = 0x000;
}