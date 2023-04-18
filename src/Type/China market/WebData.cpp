#include"pch.h"

#include"JsonParse.h"
#include"WebData.h"

CWebData::CWebData() {
	m_tTime = 0;
	m_strStockCode = _T("");
	m_sDataBuffer.resize(DefaultWebDataBufferSize_); // 大多数情况下，1M缓存就足够了，无需再次分配内存。需要在此执行一次，否则测试无法初始化。不知为何。
	m_lBufferLength = DefaultWebDataBufferSize_;
	m_lCurrentPos = 0;

	m_pCurrentPos = &m_sDataBuffer.at(0);
	m_pEndPos = &m_sDataBuffer.at(DefaultWebDataBufferSize_ - 1) + 1;

	m_fJSonContentType = false;
	m_fParsed = false;
	m_fNoRightToAccess = false;
	m_strErrorMessage = "";
}

CWebData::~CWebData() {
	m_sDataBuffer.resize(0);
}

bool CWebData::GetData(char* buffer, INT64 lDataLength) const {
	if (lDataLength + m_lCurrentPos > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		buffer[i] = m_sDataBuffer.at(i + m_lCurrentPos);
	}
	return true;
}

bool CWebData::SetData(const char* buffer, INT64 lDataLength) {
	if (lDataLength + m_lCurrentPos > m_lBufferLength) return false;
	for (INT64 i = 0; i < lDataLength; i++) {
		m_sDataBuffer.at(i + m_lCurrentPos) = buffer[i];
	}
	return true;
}

bool CWebData::CheckNoRightToAccess(const string& sCode, const string& sMessage) {
	string s;
	ASSERT(m_fParsed);
	try {
		s = m_js.at(sCode);
		m_strErrorMessage = s.c_str();
		if (s == sMessage) {
			m_fNoRightToAccess = true;
			return true;
		}
		else return false;
	}
	catch (json::exception&) {
		m_fNoRightToAccess = false;
		m_strErrorMessage = "";
		return false;
	}
}

bool CWebData::CreateJson(long lBeginPos, long lEndPos) {
	m_fParsed = ::CreateJsonWithNlohmann(m_js, m_sDataBuffer, lBeginPos, lEndPos);
	SetJSonContentType(true);

	return m_fParsed;
}

void CWebData::Test_SetBuffer_(CString strBuffer) {
	m_lBufferLength = strBuffer.GetLength();
	m_sDataBuffer.resize(m_lBufferLength);
	const char* pBuffer = strBuffer.GetBuffer();
	for (INT64 i = 0; i < m_lBufferLength; i++) {
		m_sDataBuffer.at(i) = pBuffer[i];
	}
	m_pCurrentPos = &m_sDataBuffer.at(0);
	m_pEndPos = &m_sDataBuffer.at(m_lBufferLength - 1) + 1;
}
