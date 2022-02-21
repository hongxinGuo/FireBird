#include"pch.h"

#include"Accessory.h"
#include"WebData.h"

CWebData::CWebData() : CObject() {
	m_tTime = 0;
	m_strStockCode = _T("");
	m_sDataBuffer.resize(1024 * 1024); // 大多数情况下，1M缓存就足够了，无需再次分配内存。需要在此执行一次，否则测试无法初始化。不知为何。
	m_lBufferLength = 1024 * 1024;
	m_lCurrentPos = 0;
}

CWebData::~CWebData() {
}

bool CWebData::GetData(char* buffer, INT64 lDataLength, INT64 lStartPosition) {
	if (lDataLength + lStartPosition > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		buffer[i] = m_sDataBuffer.at(i + lStartPosition);
	}
	return true;
}

bool CWebData::GetData(char* buffer, INT64 lDataLength) {
	if (lDataLength + m_lCurrentPos > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		buffer[i] = m_sDataBuffer.at(i + m_lCurrentPos);
	}
	return true;
}

bool CWebData::SetData(char* buffer, INT64 lDataLength, INT64 lStartPosition) {
	if (lDataLength + lStartPosition > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		m_sDataBuffer.at(i + lStartPosition) = buffer[i];
	}
	return true;
}

bool CWebData::SetData(char* buffer, INT64 lDataLength) {
	if (lDataLength + m_lCurrentPos > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		m_sDataBuffer.at(i + m_lCurrentPos) = buffer[i];
	}
	return true;
}

bool CWebData::CreatePTree(ptree& pt, long lBeginPos, long lEndPos) {
	if (lBeginPos > 0)	m_sDataBuffer.erase(m_sDataBuffer.begin(), m_sDataBuffer.begin() + lBeginPos);
	if (lEndPos > 0) m_sDataBuffer.resize(m_sDataBuffer.size() - lEndPos);
	return ConvertToJSON(pt, m_sDataBuffer);
}

void CWebData::__TEST_SetBuffer__(CString strBuffer) {
	m_lBufferLength = strBuffer.GetLength();
	char* pBuffer = strBuffer.GetBuffer();
	for (INT64 i = 0; i < m_lBufferLength; i++) { m_sDataBuffer.at(i) = pBuffer[i]; }
	m_sDataBuffer.resize(m_lBufferLength);
}