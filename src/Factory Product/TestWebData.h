#pragma once

#include"WebData.h"

// 此结构只用于测试中
struct Test_FinnhubWebData {
	Test_FinnhubWebData(const int index, const string& strSymbol, const string& strData) {
		m_index = index;
		m_strSymbol = strSymbol;
		m_pData = std::make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~Test_FinnhubWebData() = default;

public:
	int m_index{ 0 };
	string m_strSymbol{ "" };
	CWebDataPtr m_pData{ nullptr };
};

// 空数据
inline Test_FinnhubWebData finnhubWebData0(0, "AAPL", R"({})");
// 无权访问数据
inline Test_FinnhubWebData finnhubWebData1(1, "AAPL", R"({"error":"You don't have access to this resource."})");
// 空数据
inline Test_FinnhubWebData finnhubWebData2(2, "AAPL", R"([])");

// 此结构只用于测试中
struct Test_TiingoWebData {
	Test_TiingoWebData(const long lIndex, const string& strSymbol, const string& strData) {
		m_index = lIndex;
		m_strSymbol = strSymbol;
		m_pData = std::make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~Test_TiingoWebData() = default;

public:
	long m_index{ 0 };
	string m_strSymbol{ "" };
	CWebDataPtr m_pData{ nullptr };
};
