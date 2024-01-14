#include"pch.h"

#include"JsonParse.h"
#include"WebData.h"

CWebData::CWebData() {
	m_tTime = 0;
	m_strStockCode = _T("");
	m_sDataBuffer.resize(DefaultWebDataBufferSize_); // 大多数情况下，1M缓存就足够了，无需再次分配内存。需要在此执行一次，否则测试无法初始化。不知为何。
	m_lCurrentPos = 0;
	m_lCurrentParagraphStartPos = 0;

	m_fParsed = false;
	m_fNoRightToAccess = false;
	m_strErrorMessage = "";
}

CWebData::~CWebData() {
	m_sDataBuffer.resize(0);
}

bool CWebData::GetData(char* buffer, long lDataLength) const {
	if (lDataLength + m_lCurrentPos > m_sDataBuffer.size()) return false;
	for (long i = 0; i < lDataLength; i++) {
		buffer[i] = m_sDataBuffer.at(i + m_lCurrentPos);
	}
	return true;
}

bool CWebData::SetData(const char* buffer, long lDataLength) {
	if (lDataLength + m_lCurrentPos > m_sDataBuffer.size()) return false;
	for (long i = 0; i < lDataLength; i++) {
		m_sDataBuffer.at(i + m_lCurrentPos) = buffer[i];
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 检查读取的网络数据是否被网站所禁止提取。
/// 目前只有Finnhub网站有此选项，其字符串为：You don't have access to this resource.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	return m_fParsed;
}

void CWebData::Test_SetBuffer_(CString strBuffer) {
	m_sDataBuffer.resize(strBuffer.GetLength());
	const char* pBuffer = strBuffer.GetBuffer();
	for (long i = 0; i < m_sDataBuffer.size(); i++) {
		m_sDataBuffer.at(i) = pBuffer[i];
	}
}
