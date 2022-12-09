#pragma once

#include"SetInsiderSentiment.h"


#include<memory>

class CInsiderSentiment : public CObject {
public:
	CInsiderSentiment();

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

typedef shared_ptr<CInsiderSentiment> CInsiderSentimentPtr;
typedef shared_ptr<vector<CInsiderSentimentPtr>> CInsiderSentimentVectorPtr;
