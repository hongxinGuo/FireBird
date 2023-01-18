#pragma once

#include"StockSection.h"

class CDataStockSymbol final {
public:
	CDataStockSymbol();
	// 只能有一个实例,不允许赋值。
	CDataStockSymbol(const CDataStockSymbol&) = delete;
	CDataStockSymbol& operator=(const CDataStockSymbol&) = delete;
	CDataStockSymbol(const CDataStockSymbol&&) noexcept = delete;
	CDataStockSymbol& operator=(const CDataStockSymbol&&) noexcept = delete;
	~CDataStockSymbol() = default;
	void Reset(void);
	bool CreateTotalStockContainer(void);

	[[nodiscard]] long GetIndex(const CString& strSymbol) const { return m_mapStockSymbol.at(strSymbol); }
	[[nodiscard]] size_t GetStockSize(void) const noexcept { return m_vStockSymbol.size(); }

	[[nodiscard]] bool IsStockSymbol(const CString& strSymbol) const {
		if (m_mapStockSymbol.contains(strSymbol)) return true;
		return false;
	}

	bool Delete(const CString& strSymbol);
	void Add(const CString& strSymbol);

	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber);
	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber);

	void LoadStockSectionDB(void);
	bool UpdateStockSectionDB(void);

	void SetUpdateStockSection(const bool fFlag) noexcept { m_fUpdateStockSection = fFlag; }
	[[nodiscard]] bool IsUpdateStockSection(void) const noexcept { return m_fUpdateStockSection; }

	void SetStockSectionActiveFlag(const long lIndex, const bool fFlag) const noexcept { m_vStockSection.at(lIndex)->SetActive(fFlag); }

	[[nodiscard]] bool IsStockSectionActive(const long lIndex) const noexcept { return m_vStockSection.at(lIndex)->IsActive(); }

	void CreateStockSection(const CString& strFirstStockCode);

	[[nodiscard]] bool UpdateStockSection(const CString& strStockCode) const;
	[[nodiscard]] bool UpdateStockSection(long lIndex) const;

	long GetNextIndex(long lIndex);

private:

protected:
	vector<CString> m_vStockSymbol;
	map<CString, size_t> m_mapStockSymbol;
	vector<CString> m_vCurrentSectionStockCode; // 当前股票集的第一个代码。字符串的格式为600000.SS、sz000001
	vector<CStockSectionPtr> m_vStockSection; // 共2000个，上海深圳各1000，证券代码上三位是否已经被使用。
	bool m_fUpdateStockSection; // 更新StockSection标识

private:
	long m_lNextSinaStockInquiringMiddleStrIndex;
	long m_lNeteaseRTDataInquiryIndex;
};

using CDataStockSymbolPtr = shared_ptr<CDataStockSymbol>;
