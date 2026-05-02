#pragma once

#include"SetInsiderTransaction.h"

#include<memory>

class CInsiderTransaction final {
public:
	CInsiderTransaction();
	CInsiderTransaction(const CInsiderTransaction&) = default;
	CInsiderTransaction& operator=(const CInsiderTransaction&) = default;
	CInsiderTransaction(CInsiderTransaction&&) noexcept = default;
	CInsiderTransaction& operator=(CInsiderTransaction&&) noexcept = default;
	~CInsiderTransaction() = default;

	void Append(CSetInsiderTransaction& setInsiderTransaction) const;
	void Update(CSetInsiderTransaction& setInsiderTransaction) const;

	void Save(CSetInsiderTransaction& setInsiderTransaction) const;
	void Load(const CSetInsiderTransaction& setInsiderTransaction);

public:
	string m_strSymbol;
	string m_strPersonName;
	INT64 m_lShare;
	INT64 m_lChange;
	long m_lFilingDate;
	long m_lTransactionDate;
	string m_strTransactionCode;
	double m_dTransactionPrice;
};

using CInsiderTransactionPtr = shared_ptr<CInsiderTransaction>;
using CInsiderTransactionsPtr = shared_ptr<vector<CInsiderTransaction>>;
