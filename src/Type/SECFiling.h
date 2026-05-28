#pragma once

using std::vector;
using std::shared_ptr;

class CSECFiling final {
public:
	CSECFiling();
	CSECFiling(const CSECFiling&) = default;
	CSECFiling& operator=(const CSECFiling&) = default;
	CSECFiling(CSECFiling&&) = default;
	CSECFiling& operator=(CSECFiling&&) = default;
	~CSECFiling() = default;

	void Reset();

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
using CSECFilingsPtr = shared_ptr<vector<CSECFiling>>;
