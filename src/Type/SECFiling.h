#pragma once

#include"SetSECFilings.h"

using std::vector;
using std::shared_ptr;

class CSECFiling final {
public:
	CSECFiling();
	~CSECFiling() = default;

	void Append(CSetSECFilings& setSECFiling) const;
	void Load(const CSetSECFilings& setSECFiling);

public:
	string m_strSymbol;
	int m_iCIK;
	string m_strAccessNumber;
	string m_strForm;
	time_t m_iFiledDate;
	time_t m_iAcceptedDate;
	string m_strReportURL;
	string m_strFilingURL;
};

using CSECFilingPtr = shared_ptr<CSECFiling>;
using CSECFilingsPtr = shared_ptr<vector<CSECFilingPtr>>;
