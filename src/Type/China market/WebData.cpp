#include"pch.h"

#include"JsonParse.h"
#include"WebData.h"

CWebData::CWebData() {
	m_tTime = 0;
	m_strStockCode = _T("");
	m_sDataBuffer.resize(DefaultWebDataBufferSize_); // 大多数情况下，1M缓存就足够了，无需再次分配内存。需要在此执行一次，否则测试无法初始化。不知为何。
	m_lCurrentPos = 0;
	m_lCurrentParagraphStartPos = 0;

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

//////////////////////////////////////////////////////////////////////////////////////////////
//
// var hq_str_sh601006="大秦铁路,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32,00";
//
// 无效数据格式为：var hq_str_sh688801="";
//
// 返回的stringView包括最后的';'分号
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
string_view CWebData::GetCurrentSinaData() {
	const string_view svCurrentTotal = string_view(m_sDataBuffer.c_str() + m_lCurrentPos, m_sDataBuffer.size() - m_lCurrentPos);
	const long lStart = svCurrentTotal.find_first_of('v');
	const long lEnd = svCurrentTotal.find_first_of(';');
	if (lStart > svCurrentTotal.length() || lEnd > svCurrentTotal.length() || lStart > lEnd) {
		throw std::exception(_T("GetCurrentSinaData() out of range"));
	}
	const string_view svCurrent = svCurrentTotal.substr(lStart, lEnd - lStart + 1);
	IncreaseCurrentPos(lStart); // 将当前位置移至当前数据开始处
	return svCurrent;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时数据以字符'v'开始，以字符';'结束。
// 返回的stringView包括最后的';'分号
//
// 当所有的查询股票皆为非上市时，返回一个21个字符串：v_pv_none_match=\"1\";\n
//
//
// 20230627日结构：
// v_sh600026="1~中远海能~600026~12.25~12.40~12.34~366190~150067~216123~
//						 12.24~36~12.23~803~12.22~501~12.21~905~12.20~1899~12.25~1026~12.26~1158~12.27~645~12.28~615~12.29~139~~
//						 20230627155915~-0.15~-1.21~12.44~11.95~12.25/366190/445074382~366190~44507~1.05~23.12~~
//						 12.44~11.95~3.95~425.66~584.42~1.79~13.64~11.16~1.01~561~12.15~13.33~40.10~~~1.53~44507.4382~0.0000~0~~GP-A~1.66~-8.85~0.00~
//						 7.73~4.16~21.04~8.58~1.16~7.64~-11.81~3474776395~4770776395~7.26~-34.49~3474776395~~~17.22~0.16~~CNY~0~___D__F__N";\n
//
///////////////////////////////////////////////////////////////////////////////////////////
string_view CWebData::GetCurrentTengxunData() {
	const string_view svCurrentTotal = string_view(m_sDataBuffer.c_str() + m_lCurrentPos, m_sDataBuffer.size() - m_lCurrentPos);
	const long lStart = svCurrentTotal.find_first_of('v');
	const long lEnd = svCurrentTotal.find_first_of(';');
	if (lStart > svCurrentTotal.length() || lEnd > svCurrentTotal.length() || lStart > lEnd) {
		throw std::exception(_T("GetCurrentTengxunData() out of range"));
	}
	const string_view svCurrent = svCurrentTotal.substr(lStart, lEnd - lStart + 1);
	IncreaseCurrentPos(lStart); // 将当前位置移至当前数据开始处
	return svCurrent;
}

bool CWebData::CreateJson(json& js, long lBeginPos, long lEndPos) const {
	return ::CreateJsonWithNlohmann(js, m_sDataBuffer, lBeginPos, lEndPos);
}

void CWebData::Test_SetBuffer_(CString strBuffer) {
	m_sDataBuffer.resize(strBuffer.GetLength());
	const char* pBuffer = strBuffer.GetBuffer();
	for (long i = 0; i < m_sDataBuffer.size(); i++) {
		m_sDataBuffer.at(i) = pBuffer[i];
	}
}
