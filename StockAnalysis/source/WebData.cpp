#include"pch.h"

#include"Accessory.h"
#include"WebData.h"

#include<nlohmann/json.hpp>
using namespace nlohmann;

CWebData::CWebData() : CObject() {
	m_tTime = 0;
	m_strStockCode = _T("");
	m_sDataBuffer.resize(__DefaultWebDataBufferSize__); // 大多数情况下，1M缓存就足够了，无需再次分配内存。需要在此执行一次，否则测试无法初始化。不知为何。
	m_lBufferLength = __DefaultWebDataBufferSize__;
	m_lCurrentPos = 0;

	m_fJSonContentType = false;
	m_fSucceedParsed = false;
	m_ppt = make_shared<ptree>();
}

CWebData::~CWebData() {
	m_sDataBuffer.resize(0);
	if (m_ppt != nullptr) m_ppt = nullptr;
	if (m_pjs != nullptr) m_pjs = nullptr;
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
	m_fSucceedParsed = ConvertToJSON(pt, m_sDataBuffer);
	return m_fSucceedParsed;
}

bool CWebData::CreatePTree(long lBeginPos, long lEndPos)
{
	if (lBeginPos > 0)	m_sDataBuffer.erase(m_sDataBuffer.begin(), m_sDataBuffer.begin() + lBeginPos);
	if (lEndPos > 0) m_sDataBuffer.resize(m_sDataBuffer.size() - lEndPos);
	if (m_ppt == nullptr) m_ppt = make_shared<ptree>();
	m_fSucceedParsed = ConvertToJSON(m_ppt, m_sDataBuffer);
	return m_fSucceedParsed;
}

bool CWebData::CreateJSon(json& js, long lBeginPos, long lEndPos) {
	if (lBeginPos > 0)	m_sDataBuffer.erase(m_sDataBuffer.begin(), m_sDataBuffer.begin() + lBeginPos);
	if (lEndPos > 0) m_sDataBuffer.resize(m_sDataBuffer.size() - lEndPos);
	js = json::parse(m_sDataBuffer);

	return true;
}

bool CWebData::CreateJSon(long lBeginPos, long lEndPos) {
	if (lBeginPos > 0)	m_sDataBuffer.erase(m_sDataBuffer.begin(), m_sDataBuffer.begin() + lBeginPos);
	if (lEndPos > 0) m_sDataBuffer.resize(m_sDataBuffer.size() - lEndPos);
	if (m_pjs == nullptr) m_pjs = make_shared<nlohmann::json>();
	try {
		*m_pjs = json::parse(m_sDataBuffer);
	}
	catch (json::parse_error& e) {
		m_pjs = nullptr;
		return false;
	}

	return true;
}

void CWebData::__TEST_SetBuffer(CString strBuffer) {
	m_lBufferLength = strBuffer.GetLength();
	char* pBuffer = strBuffer.GetBuffer();
	for (INT64 i = 0; i < m_lBufferLength; i++) { m_sDataBuffer.at(i) = pBuffer[i]; }
	m_sDataBuffer.resize(m_lBufferLength);
}