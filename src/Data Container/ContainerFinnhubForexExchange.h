#pragma once

class CContainerFinnhubForexExchange final {
public:
	CContainerFinnhubForexExchange();
	CContainerFinnhubForexExchange(const CContainerFinnhubForexExchange& other) = delete;
	CContainerFinnhubForexExchange(CContainerFinnhubForexExchange&& other) noexcept = delete;
	CContainerFinnhubForexExchange& operator=(const CContainerFinnhubForexExchange& other) = delete;
	CContainerFinnhubForexExchange& operator=(CContainerFinnhubForexExchange&& other) noexcept = delete;
	~CContainerFinnhubForexExchange() = default;
	void Reset();

	bool IsExchange(const string& strExchange) const noexcept { return m_mapForexExchange.contains(strExchange); }
	size_t Size() const noexcept { return m_vForexExchange.size(); }
	string GetExchange(const size_t lIndex) const { return m_vForexExchange.at(lIndex); }
	bool Delete(const string& strForexExchange);
	void Add(const string& strForexExchange);

	bool LoadDB();
	bool UpdateDB();

	bool IsNeedUpdate() const noexcept { return m_llLastTotalForexExchange < m_vForexExchange.size(); }
	void SetLastSize(size_t lSize) noexcept { m_llLastTotalForexExchange = lSize; }

protected:
	vector<string> m_vForexExchange;
	map<string, size_t> m_mapForexExchange;
	size_t m_llLastTotalForexExchange{ 0 };
};
