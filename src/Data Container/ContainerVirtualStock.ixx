module;

export module ContainerVirtualStock;

import VirtualContainer;
import VirtualStock;
import std;


using std::unordered_map;
using std::vector;
using std::shared_ptr;
using std::string;

export {
	class CContainerVirtualStock : public CVirtualContainer {
	public:
		CContainerVirtualStock();
		~CContainerVirtualStock() override = default;

		void Reset() override;
		string GetItemSymbol(size_t lIndex) override;
		vector<string> GetSymbols();

		bool IsEmpty() override { return m_vStock.empty(); }
		size_t Size() override { return m_vStock.size(); }
		void Reserve(size_t size) {
			m_vStock.reserve(size);
			m_vStock.clear();
			m_mapSymbol.reserve(size);
			m_mapSymbol.clear();
		}

		bool IsSymbol(const string& strSymbol) const { return m_mapSymbol.contains(strSymbol); }
		bool IsSymbol(const shared_ptr<CVirtualStock>& p) const;
		bool IsUpdateProfileDB() noexcept;
		bool IsUpdateDayLine() noexcept;
		size_t GetDayLineNeedUpdateNumber() const;
		bool IsUpdateDayLineDB() noexcept;

		bool IsUpdateDB() const noexcept { return m_bUpdateDB; }
		void SetUpdateDB(const bool fFlag) noexcept { m_bUpdateDB = fFlag; }

		shared_ptr<CVirtualStock> Get(const size_t lIndex);
		shared_ptr<CVirtualStock> Get(const string& strSymbol);

		string GetItemExchangeCode(const size_t lIndex) const;

		size_t GetOffset(const string& strSymbol) const { return m_mapSymbol.at(strSymbol); }
		size_t GetOffset(const shared_ptr<CVirtualStock>& pStock) const;

		void Add(const shared_ptr<CVirtualStock>& pStock);
		void Delete(const shared_ptr<CVirtualStock>& pStock);
		void Delete(const string& strSymbol);
		void UpdateSymbolMap();
		void Sort();

	protected:
		vector<shared_ptr<CVirtualStock>> m_vStock; //Note 此容器中真实存储的，为CVirtualStock类的各继承类，使用时需要使用dynamic_pointer_cast<>转换成对应的继承类。
		unordered_map<string, size_t> m_mapSymbol; // m_vStock在容器中的具体偏移量。使用unordered_map是为了提高查询效率。注意：当删除股票时，需要更新此map中所有股票的偏移量。

		bool m_bUpdateDB{ false };
	};
}
