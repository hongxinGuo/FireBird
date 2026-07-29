module;

export module FinnhubCompanySymbolChange;

import std;
using std::shared_ptr;
using std::vector;
using std::string;

export {
	class CFinnhubCompanySymbolChange final {
	public:
		CFinnhubCompanySymbolChange();
		CFinnhubCompanySymbolChange(const CFinnhubCompanySymbolChange&) = default;
		CFinnhubCompanySymbolChange& operator=(const CFinnhubCompanySymbolChange&) = default;
		CFinnhubCompanySymbolChange(CFinnhubCompanySymbolChange&&) noexcept = default;
		CFinnhubCompanySymbolChange& operator=(CFinnhubCompanySymbolChange&&) noexcept = default;
		~CFinnhubCompanySymbolChange() = default;

		void Reset() noexcept {
			m_iTime = 19800101;
			m_sTime = "1980-01-01";
			m_sNewSymbol.clear();
			m_sOldSymbol.clear();
		}

	public:
		long m_iTime{ 19800101 }; //
		string m_sTime{ "1980-01-01" };
		string m_sNewSymbol{ "" };
		string m_sOldSymbol{ "" };
	};

	using CCompanySymbolChangePtr = shared_ptr<CFinnhubCompanySymbolChange>;
	using CCompanySymbolChangesPtr = shared_ptr<vector<CFinnhubCompanySymbolChange>>;
}