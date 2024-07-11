#pragma once

#include"SetInsiderSentiment.h"

#include<memory>

class CInsiderSentiment final {
public:
	CInsiderSentiment();
	~CInsiderSentiment() = default;

	void Append(CSetInsiderSentiment& setInsiderTransaction) const;
	void Update(CSetInsiderSentiment& setInsiderTransaction) const;

	void Save(CSetInsiderSentiment& setInsiderTransaction) const;
	void Load(const CSetInsiderSentiment& setInsiderTransaction);

public:
	CString m_strSymbol;
	long m_lDate;
	long m_lChange;
	double m_mspr;
};

using CInsiderSentimentPtr = shared_ptr<CInsiderSentiment>;
using CInsiderSentimentsPtr = shared_ptr<vector<CInsiderSentimentPtr>>;
