#pragma once

#include"SetInsiderSentiment.h"

#include<memory>

class CInsiderSentiment final {
public:
	CInsiderSentiment();
	virtual ~CInsiderSentiment() = default;

	void Append(CSetInsiderSentiment& setInsiderTransaction);
	void Update(CSetInsiderSentiment& setInsiderTransaction);

	void Save(CSetInsiderSentiment& setInsiderTransaction);
	void Load(CSetInsiderSentiment& setInsiderTransaction);

public:
	CString m_strSymbol;
	long m_lDate;
	long m_lChange;
	double m_mspr;
};

using CInsiderSentimentPtr = shared_ptr<CInsiderSentiment>;
using CInsiderSentimentVectorPtr = shared_ptr<vector<CInsiderSentimentPtr>>;
