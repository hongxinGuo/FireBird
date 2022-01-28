#pragma once

#include"ChinaStock.h"
#include"SetChinaStockSymbol.h"

using namespace std;
#include<vector>
#include<map>

class CDataChinaStockSymbol : public CObject {
public:
	CDataChinaStockSymbol();
	~CDataChinaStockSymbol();
	void Reset(void);

	long GetIndex(CString strSymbol) { return m_mapStockSymbol.at(strSymbol); }
	size_t GetStockSize(void) noexcept { return m_vStockSymbol.size(); }
	bool IsStockSymbol(CString strSymbol) { if (m_mapStockSymbol.find(strSymbol) == m_mapStockSymbol.end()) return false; else return true; }

	bool Delete(CString strSymbol);
	void Add(CString strSymbol);

	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber);
	CString GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber);

private:
	long IncreaseIndex(long& lIndex, long lEndPosition);

protected:
	vector<CString> m_vStockSymbol;
	map<CString, long> m_mapStockSymbol;
};
