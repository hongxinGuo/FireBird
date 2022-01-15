#include"pch.h"
#include"WebData.h"

CWebData::CWebData() : CObject() {
	m_tTime = 0;
	m_strStockCode = _T("");
	m_lBufferLength = 0;
	m_lCurrentPos = 0;
}

CWebData::~CWebData() {
}

bool CWebData::GetData(char* buffer, INT64 lDataLength, INT64 lStartPosition) {
	if (lDataLength + lStartPosition > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		buffer[i] = m_vDataBuffer.at(i + lStartPosition);
	}
	return true;
}

bool CWebData::GetData(char* buffer, INT64 lDataLength) {
	if (lDataLength + m_lCurrentPos > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		buffer[i] = m_vDataBuffer.at(i + m_lCurrentPos);
	}
	return true;
}

bool CWebData::SetData(char* buffer, INT64 lDataLength, INT64 lStartPosition) {
	if (lDataLength + lStartPosition > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		m_vDataBuffer.at(i + lStartPosition) = buffer[i];
	}
	return true;
}

bool CWebData::SetData(char* buffer, INT64 lDataLength) {
	if (lDataLength + m_lCurrentPos > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		m_vDataBuffer.at(i + m_lCurrentPos) = buffer[i];
	}
	return true;
}

void CWebData::__TEST_SetBuffer__(CString strBuffer) {
	m_lBufferLength = strBuffer.GetLength();
	char* pBuffer = strBuffer.GetBuffer();
	for (INT64 i = 0; i < m_lBufferLength; i++) { m_vDataBuffer.at(i) = pBuffer[i]; }
	m_vDataBuffer.at(m_lBufferLength) = 0x000;
}