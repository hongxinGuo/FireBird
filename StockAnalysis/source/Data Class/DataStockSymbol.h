#pragma once

#include"StockSection.h"
#include"ChinaStock.h"
#include"SetStockSection.h"

#include<memory>
#include<vector>
#include<map>

class CDataStockSymbol final : public CObject {
public:
	CDataStockSymbol();
	// 只能有一个实例,不允许赋值。
	CDataStockSymbol(const CDataStockSymbol&) = delete;
	CDataStockSymbol& operator=(const CDataStockSymbol&) = delete;
	CDataStockSymbol(const CDataStockSymbol&&) noexcept = delete;
	CDataStockSymbol& operator=(const CDataStockSymbol&&) noexcept = delete;
	~CDataStockSymbol() override = default;
	void Reset(void);
	bool CreateTotalStockContainer(void);

	long GetIndex(const CString strSymbol) const { return m_mapStockSymbol.at(strSymbol); }
	size_t GetStockSize(void) const noexcept { return m_vStockSymbol.size(); }

	bool IsStockSymbol(const CString strSymbol) const {
		if (m_mapStockSymbol.contains(strSymbol)) return true;
		else return false;
	}

	bool Delete(CString strSymbol);
	void Add(CString strSymbol);

	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber);
	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber);

	void LoadStockSectionDB(void);
	bool UpdateStockSectionDB(void);

	void SetUpdateStockSection(bool fFlag) noexcept { m_fUpdateStockSection = fFlag; }
	bool IsUpdateStockSection(void) const noexcept { return m_fUpdateStockSection; }

	void SetStockSectionActiveFlag(long lIndex, bool fFlag) const noexcept {
		m_vStockSection.at(lIndex)->SetActive(fFlag);
	}

	bool IsStockSectionActive(long lIndex) const noexcept { return m_vStockSection.at(lIndex)->IsActive(); }

	void CreateStockSection(CString strFirstStockCode);

	bool UpdateStockSection(CString strStockCode);
	bool UpdateStockSection(long lIndex);

	long GetNextIndex(long& lIndex);

private:

protected:
	vector<CString> m_vStockSymbol;
	map<CString, long> m_mapStockSymbol;
	vector<CString> m_vCurrentSectionStockCode; // 当前股票集的第一个代码。字符串的格式为600000.SS、sz000001
	vector<CStockSectionPtr> m_vStockSection; // 共2000个，上海深圳各1000，证券代码上三位是否已经被使用。
	bool m_fUpdateStockSection; // 更新StockSection标识

private:
	long m_lNextSinaStockInquiringMiddleStrIndex;
	long m_lNeteaseRTDataInquiryIndex;
};

typedef shared_ptr<CDataStockSymbol> CDataStockSymbolPtr;
