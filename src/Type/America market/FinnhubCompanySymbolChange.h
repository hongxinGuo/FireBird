#pragma once

using std::shared_ptr;
using std::vector;

class CCompanySymbolChange final {
public:
	CCompanySymbolChange();
	~CCompanySymbolChange() = default;

public:
	long m_iTime{ 19800101 }; //
	string m_sTime{ "1980-01-01" };
	string m_sNewSymbol{ "" };
	string m_sOldSymbol{ "" };
};

using CCompanySymbolChangePtr = shared_ptr<CCompanySymbolChange>;
using CCompanySymbolChangesPtr = shared_ptr<vector<CCompanySymbolChangePtr>>;
