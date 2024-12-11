#include"pch.h"
module;
#include"FinnhubStockExchange.h"
export module FireBird.Container.Finnhub.StockExchange;

export {
	class CContainerFinnhubStockExchange final {
	public:
		CContainerFinnhubStockExchange();
		CContainerFinnhubStockExchange(const CContainerFinnhubStockExchange& other) = delete;
		CContainerFinnhubStockExchange(CContainerFinnhubStockExchange&& other) noexcept = delete;
		CContainerFinnhubStockExchange& operator=(const CContainerFinnhubStockExchange& other) = delete;
		CContainerFinnhubStockExchange& operator=(CContainerFinnhubStockExchange&& other) noexcept = delete;
		~CContainerFinnhubStockExchange() = default;
		void Reset();

		CFinnhubStockExchangePtr GetExchange(const size_t lIndex) const { return m_vFinnhubStockExchange.at(lIndex); }
		CString GetExchangeCode(const size_t lIndex) const { return m_vFinnhubStockExchange.at(lIndex)->m_strCode; }
		auto Size() const noexcept { return m_mapFinnhubStockExchange.size(); }

		bool LoadDB();

	protected:
		vector<CFinnhubStockExchangePtr> m_vFinnhubStockExchange;
		map<CString, size_t> m_mapFinnhubStockExchange;
	};
}
