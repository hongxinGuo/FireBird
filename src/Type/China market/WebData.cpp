#include"pch.h"

#include"WebData.h"
#include"JsonParse.h"

CWebData::CWebData() {
	m_tpTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(0));
	m_strStockCode = _T("");
	m_lCurrentPos = 0;
	m_lCurrentParagraphStartPos = 0;
	m_strErrorMessage = "";
}

CWebData::~CWebData() {
}

bool CWebData::GetData(char* buffer, size_t lDataLength) const {
	if (lDataLength + m_lCurrentPos > m_sDataBuffer.size()) return false;
	for (size_t i = 0; i < lDataLength; i++) {
		buffer[i] = m_sDataBuffer.at(i + m_lCurrentPos);
	}
	return true;
}

bool CWebData::SetData(const char* buffer, size_t lDataLength) {
	if (lDataLength + m_lCurrentPos > m_sDataBuffer.size()) return false;
	for (size_t i = 0; i < lDataLength; i++) {
		m_sDataBuffer.at(i + m_lCurrentPos) = buffer[i];
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 返回的stringView包括最后的';'分号
// 提取数据后将当前位置移至该数据之后
//
// var hq_str_sh601006="大秦铁路,27.55,27.25,26.91,27.55,26.20,26.91,26.92,
//                     22114263,589824680,4695,26.91,57590,26.90,14700,26.89,14300,
//                     26.88,15100,26.87,3100,26.92,8900,26.93,14230,26.94,25150,26.95,15220,26.96,2008-01-11,15:05:32,00";
//
// 无效数据格式为：var hq_str_sh688801="";
//
//////////////////////////////////////////////////////////////////////////////////////////////
string_view CWebData::GetCurrentSinaData() {
	const string_view svCurrentTotal = string_view(m_sDataBuffer.c_str() + m_lCurrentPos, m_sDataBuffer.size() - m_lCurrentPos);
	const auto lStart = svCurrentTotal.find_first_of('v');
	const auto lEnd = svCurrentTotal.find_first_of(';');
	ASSERT(lStart <= svCurrentTotal.length());
	ASSERT(lEnd <= svCurrentTotal.length());
	ASSERT(lStart <= lEnd);
	IncreaseCurrentPos(lEnd + 1); // 将当前位置移至当前数据结束处之后
	return svCurrentTotal.substr(lStart, lEnd - lStart + 1); // 包括最后的字符';'
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时数据以字符'v'开始，以字符';'结束。
// 返回的stringView包括最后的';'分号
// 提取数据后将当前位置移至该数据之后
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
	const auto lStart = svCurrentTotal.find_first_of('v');
	const auto lEnd = svCurrentTotal.find_first_of(';');
	ASSERT(lStart <= svCurrentTotal.length());
	ASSERT(lEnd <= svCurrentTotal.length());
	ASSERT(lStart <= lEnd);
	IncreaseCurrentPos(lEnd + 1); // 将当前位置移至当前数据结束处之后
	return svCurrentTotal.substr(lStart, lEnd - lStart + 1);
}

bool CWebData::CreateJson(json& js, long lBeginPos, long lEndPos) const {
	return ::CreateJsonWithNlohmann(js, m_sDataBuffer, lBeginPos, lEndPos);
}

void CWebData::Test_SetBuffer_(string strBuffer) {
	m_sDataBuffer.resize(strBuffer.length());
	for (long i = 0; i < m_sDataBuffer.size(); i++) {
		m_sDataBuffer.at(i) = strBuffer[i];
	}
}
