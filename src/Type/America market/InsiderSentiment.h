#pragma once

class CInsiderSentiment final {
public:
	CInsiderSentiment();
	CInsiderSentiment(const CInsiderSentiment& other);
	CInsiderSentiment& operator=(const CInsiderSentiment& other);
	CInsiderSentiment(CInsiderSentiment&&) noexcept;
	CInsiderSentiment& operator=(CInsiderSentiment&&) noexcept;

	~CInsiderSentiment() = default;

public:
	string m_strSymbol;
	long m_lDate;
	long m_lChange;
	double m_mspr;
};

using CInsiderSentimentPtr = shared_ptr<CInsiderSentiment>;
using CInsiderSentimentsPtr = shared_ptr<vector<CInsiderSentiment>>;
