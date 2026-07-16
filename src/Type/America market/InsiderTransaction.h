#pragma once

using std::string;
using std::vector;
using std::shared_ptr;

class CInsiderTransaction final {
public:
	CInsiderTransaction();
	CInsiderTransaction(const CInsiderTransaction&) = default;
	CInsiderTransaction& operator=(const CInsiderTransaction&) = default;
	CInsiderTransaction(CInsiderTransaction&&) noexcept = default;
	CInsiderTransaction& operator=(CInsiderTransaction&&) noexcept = default;
	~CInsiderTransaction() = default;

public:
	string m_strSymbol;
	string m_strPersonName;
	long long m_lShare;
	long long m_lShareChange;
	int m_lFilingDate;
	int m_lTransactionDate;
	string m_strTransactionCode;
	double m_dTransactionPrice;
};

using CInsiderTransactionPtr = shared_ptr<CInsiderTransaction>;
using CInsiderTransactionsPtr = shared_ptr<vector<CInsiderTransaction>>;
