#pragma once

#include"SetInsiderTransaction.h"

#include<memory>

class CInsiderTransaction final : public CObject {
public:
	CInsiderTransaction();

	void Append(CSetInsiderTransaction& setInsiderTransaction);
	void Update(CSetInsiderTransaction& setInsiderTransaction);

	void Save(CSetInsiderTransaction& setInsiderTransaction);
	void Load(CSetInsiderTransaction& setInsiderTransaction);

public:
	CString m_strSymbol;
	CString m_strPersonName;
	INT64 m_lShare;
	INT64 m_lChange;
	long m_lFilingDate;
	long m_lTransactionDate;
	CString m_strTransactionCode;
	double m_dTransactionPrice;
};

typedef shared_ptr<CInsiderTransaction> CInsiderTransactionPtr;
typedef shared_ptr<vector<CInsiderTransactionPtr>> CInsiderTransactionVectorPtr;
