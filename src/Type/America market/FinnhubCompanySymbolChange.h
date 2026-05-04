#pragma once

using std::shared_ptr;
using std::vector;

class CCompanySymbolChange final {
public:
	CCompanySymbolChange();
	CCompanySymbolChange(const CCompanySymbolChange&) = default;
	CCompanySymbolChange& operator=(const CCompanySymbolChange&) = default;
	CCompanySymbolChange(CCompanySymbolChange&&) noexcept = default;
	CCompanySymbolChange& operator=(CCompanySymbolChange&&) noexcept = default;
	~CCompanySymbolChange() = default;

	void Reset() noexcept {
		m_iTime = 19800101;
		m_sTime = "1980-01-01";
		m_sNewSymbol.clear();
		m_sOldSymbol.clear();
	}

public:
	long m_iTime{ 19800101 }; //
	string m_sTime{ "1980-01-01" };
	string m_sNewSymbol{ "" };
	string m_sOldSymbol{ "" };
};

using CCompanySymbolChangePtr = shared_ptr<CCompanySymbolChange>;
using CCompanySymbolChangesPtr = shared_ptr<vector<CCompanySymbolChange>>;
