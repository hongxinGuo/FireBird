module;

export module EPSSurprise;

import std;
using std::string;
using std::vector;
using std::shared_ptr;
using std::chrono::local_days;

export {
	class CEPSSurprise final {
	public:
		CEPSSurprise();
		CEPSSurprise(const CEPSSurprise&) = default;
		CEPSSurprise& operator=(const CEPSSurprise&) = default;
		CEPSSurprise(CEPSSurprise&&) noexcept = default;
		CEPSSurprise& operator=(CEPSSurprise&&) noexcept = default;
		~CEPSSurprise() = default;

	public:
		string m_strSymbol;
		local_days m_lDate;
		double m_dActual;
		double m_dEstimate;
	};

	using CEPSSurprisePtr = shared_ptr<CEPSSurprise>;
	using CEPSSurprisesPtr = shared_ptr<vector<CEPSSurprise>>;
}