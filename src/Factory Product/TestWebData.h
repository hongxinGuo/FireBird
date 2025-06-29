#pragma once

#include"WebData.h"

// 此结构只用于测试中
struct Test_FinnhubWebData {
	Test_FinnhubWebData(const long lIndex, const string& strSymbol, const string& strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = std::make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~Test_FinnhubWebData() = default;

public:
	long m_lIndex{ 0 };
	string m_strSymbol{ _T("") };
	CWebDataPtr m_pData{ nullptr };
};

extern Test_FinnhubWebData finnhubWebData0;
extern Test_FinnhubWebData finnhubWebData1;
extern Test_FinnhubWebData finnhubWebData2;

// 此结构只用于测试中
struct Test_TiingoWebData {
	Test_TiingoWebData(const long lIndex, const string& strSymbol, const string& strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = std::make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~Test_TiingoWebData() = default;

public:
	long m_lIndex{ 0 };
	string m_strSymbol{ _T("") };
	CWebDataPtr m_pData{ nullptr };
};
