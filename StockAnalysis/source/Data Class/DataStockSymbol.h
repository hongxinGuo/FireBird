#pragma once

#include"StockSection.h"
#include"ChinaStock.h"
#include"SetStockSection.h"

using namespace std;
#include<memory>
#include<vector>
#include<map>

class CDataStockSymbol : public CObject {
public:
	CDataStockSymbol();
	~CDataStockSymbol();
	void Reset(void);
	bool CreateTotalStockContainer(void);

	long GetIndex(CString strSymbol) { return m_mapStockSymbol.at(strSymbol); }
	size_t GetStockSize(void) noexcept { return m_vStockSymbol.size(); }
	bool IsStockSymbol(CString strSymbol) { if (m_mapStockSymbol.find(strSymbol) == m_mapStockSymbol.end()) return false; else return true; }

	bool Delete(CString strSymbol);
	void Add(CString strSymbol);

	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber);
	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber);

	void LoadStockSectionDB(void);
	bool UpdateStockSectionDB(void);

	void SetUpdateStockSection(bool fFlag) noexcept { m_fUpdateStockSection = fFlag; }
	bool IsUpdateStockSection(void) const noexcept { return m_fUpdateStockSection; }
	void SetStockSectionActiveFlag(long lIndex, bool fFlag) noexcept { m_vStockSection.at(lIndex)->SetActive(fFlag); }
	bool IsStockSectionActive(long lIndex) const noexcept { return m_vStockSection.at(lIndex)->IsActive(); }

	void CreateStockSection(CString strFirstStockCode);

	bool UpdateStockSection(CString strStockCode);
	bool UpdateStockSection(long lIndex);

	long IncreaseIndex(long& lIndex, long lEndPosition);

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
