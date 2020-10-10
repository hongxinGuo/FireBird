#include"WebData.h"

void CWebData::__TEST_SetBuffer__(CString strBuffer) {
  m_pDataBuffer = new char[strBuffer.GetLength() + 1];
  m_lBufferLength = strBuffer.GetLength();
  char* pBuffer = strBuffer.GetBuffer();
  for (int i = 0; i < m_lBufferLength; i++) { m_pDataBuffer[i] = pBuffer[i]; }
  m_pDataBuffer[m_lBufferLength] = 0x000;
}