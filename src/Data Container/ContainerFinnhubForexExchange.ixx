#include"pch.h"
export module FireBird.Container.Finnhub.ForexExchange;

export {
	class CContainerFinnhubForexExchange final {
	public:
		CContainerFinnhubForexExchange();
		CContainerFinnhubForexExchange(const CContainerFinnhubForexExchange& other) = delete;
		CContainerFinnhubForexExchange(CContainerFinnhubForexExchange&& other) noexcept = delete;
		CContainerFinnhubForexExchange& operator=(const CContainerFinnhubForexExchange& other) = delete;
		CContainerFinnhubForexExchange& operator=(CContainerFinnhubForexExchange&& other) noexcept = delete;
		~CContainerFinnhubForexExchange() = default;
		void Reset();

		bool IsExchange(const CString& strExchange) const noexcept { return m_mapForexExchange.contains(strExchange); }
		size_t Size() const noexcept { return m_vForexExchange.size(); }
		CString GetExchange(const size_t lIndex) const { return m_vForexExchange.at(lIndex); }
		bool Delete(const CString& strForexExchange);
		void Add(const CString& strForexExchange);

		bool LoadDB();
		bool UpdateDB();

		bool IsNeedUpdate() const noexcept { return m_llLastTotalForexExchange < m_vForexExchange.size(); }
		void SetLastSize(size_t lSize) noexcept { m_llLastTotalForexExchange = lSize; }

	protected:
		vector<CString> m_vForexExchange;
		map<CString, size_t> m_mapForexExchange;
		size_t m_llLastTotalForexExchange{ 0 };
	};
}
