#pragma once

class CStockExchange;

using std::shared_ptr;
using std::string;
using std::vector;

class CContainerStockExchange final {
public:
	CContainerStockExchange();
	CContainerStockExchange(const CContainerStockExchange& other) = delete;
	CContainerStockExchange(CContainerStockExchange&& other) noexcept = delete;
	CContainerStockExchange& operator=(const CContainerStockExchange& other) = delete;
	CContainerStockExchange& operator=(CContainerStockExchange&& other) noexcept = delete;
	~CContainerStockExchange() = default;

	void Reset();
	void Reserve(size_t size);

	shared_ptr<CStockExchange> GetItem(const size_t lIndex) const { return m_vStockExchange.at(lIndex); }
	shared_ptr<CStockExchange> GetItem(const string& strExchangeSymbol) const;
	string GetItemExchangeCode(const size_t lIndex) const;
	auto Size() const noexcept { return m_mapStockExchange.size(); }

	bool LoadDB();

protected:
	vector<shared_ptr<CStockExchange>> m_vStockExchange;
	std::unordered_map<string, size_t> m_mapStockExchange;
};

extern CContainerStockExchange gl_dataContainerStockExchange;
