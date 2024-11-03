#pragma once

#include"StockSection.h"
#include "VirtualContainer.h"

class CContainerStockSymbol : public CVirtualContainer {
public:
	CContainerStockSymbol();
	// 只能有一个实例,不允许赋值。
	CContainerStockSymbol(const CContainerStockSymbol&) = delete;
	CContainerStockSymbol& operator=(const CContainerStockSymbol&) = delete;
	CContainerStockSymbol(const CContainerStockSymbol&&) noexcept = delete;
	CContainerStockSymbol& operator=(const CContainerStockSymbol&&) noexcept = delete;
	~CContainerStockSymbol() override = default;
	void Reset() override;
	bool CreateTotalStockContainer();
	CString GetItemSymbol(size_t lIndex) override;
	bool IsEmpty() override { return m_vStockSymbol.empty(); }
	size_t Size() override;

	[[nodiscard]] size_t GetIndex(const CString& strSymbol) const { return m_mapStockSymbol.at(strSymbol); }

	[[nodiscard]] bool IsStockSymbol(const CString& strSymbol) const {
		if (m_mapStockSymbol.contains(strSymbol)) return true;
		return false;
	}

	bool Delete(const CString& strSymbol);
	void Add(const CString& strSymbol);

	void LoadStockSectionDB() const;
	void UpdateStockSectionDB();

	void SetUpdateStockSection(const bool fFlag) noexcept { m_fUpdateStockSection = fFlag; }
	[[nodiscard]] bool IsUpdateStockSection() const noexcept { return m_fUpdateStockSection; }

	void SetStockSectionActiveFlag(const size_t lIndex, const bool fFlag) const noexcept { m_vStockSection.at(lIndex)->SetActive(fFlag); }
	[[nodiscard]] bool IsStockSectionActive(const size_t lIndex) const noexcept { return m_vStockSection.at(lIndex)->IsActive(); }

	void CreateStockSection(const CString& strFirstStockCode);

	[[nodiscard]] bool UpdateStockSection(const CString& strStockCode) const;
	[[nodiscard]] bool UpdateStockSection(size_t lIndex) const;

protected:
	vector<CString> m_vStockSymbol;
	map<CString, size_t> m_mapStockSymbol;
	vector<CString> m_vCurrentSectionStockCode; // 当前股票集的第一个代码。字符串的格式为600000.SS、sz000001
	vector<CStockSectionPtr> m_vStockSection; // 共2000个，上海深圳各1000，证券代码上三位是否已经被使用。
	bool m_fUpdateStockSection; // 更新StockSection标识
};

using CContainerStockSymbolPtr = shared_ptr<CContainerStockSymbol>;
