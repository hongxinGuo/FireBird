#include"pch.h"
module;
#include"SetSECFilings.h"
export module FireBird.Type.SECFiling;
using std::vector;
using std::shared_ptr;

export {
	class CSECFiling final {
	public:
		CSECFiling();
		~CSECFiling() = default;

		void Append(CSetSECFilings& setSECFiling) const;
		void Load(const CSetSECFilings& setSECFiling);

	public:
		CString m_strSymbol;
		int m_iCIK;
		CString m_strAccessNumber;
		CString m_strForm;
		time_t m_iFiledDate;
		time_t m_iAcceptedDate;
		CString m_strReportURL;
		CString m_strFilingURL;
	};

	using CSECFilingPtr = shared_ptr<CSECFiling>;
	using CSECFilingsPtr = shared_ptr<vector<CSECFilingPtr>>;
}
