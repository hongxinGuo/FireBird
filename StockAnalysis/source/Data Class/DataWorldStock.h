#pragma once

#include"WorldStock.h"

#include<vector>
#include<map>

class CDataWorldStock : public CObject {
public:
	CDataWorldStock();
	~CDataWorldStock() override = default;
	void Reset(void);

	bool IsStock(CString strSymbol) const {
		if (m_mapWorldStock.contains(strSymbol)) return true;
		else return false;
	}

	bool IsStock(const CWorldStockPtr pStock) const { return IsStock(pStock->GetSymbol()); }
	CWorldStockPtr GetStock(const long lIndex) const { return m_vWorldStock.at(lIndex); }
	CWorldStockPtr GetStock(const CString &strSymbol) const { return m_vWorldStock.at(m_mapWorldStock.at(strSymbol)); }
	long GetIndex(const CString &strSymbol) const { return m_mapWorldStock.at(strSymbol); }
	long GetStockIndex(const CString &strSymbol) const { return m_mapWorldStock.at(strSymbol); }
	size_t GetStockSize(void) const noexcept { return m_vWorldStock.size(); }
	long GetLastStockSize(void) const noexcept { return m_lLastTotalWorldStock; }

	bool SortStock(void);

	bool IsStockProfileNeedUpdate(void);
	bool IsCompanyNewsNeedUpdate(void);
	bool IsBasicFinancialNeedUpdate(void);

	void ResetEPSSurprise(void);
	void ResetPeer(void);
	void ResetBasicFinancial(void);
	void ResetDayLine(void);

	bool Delete(CWorldStockPtr pStock);
	void Add(CWorldStockPtr pStock);

	bool LoadDB(void);
	bool UpdateProfileDB(void);

	bool UpdateBasicFinancialDB(void);
	bool UpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock);
	bool UpdateBasicFinancialQuarterDB(vector<CWorldStockPtr> vStock);
	bool UpdateBasicFinancialMetricDB(vector<CWorldStockPtr> vStock);
	void ClearUpdateBasicFinancialFlag(vector<CWorldStockPtr> vStock);

	bool CheckStockSymbol(CWorldStockPtr pStock);

	bool IsNeedSaveDayLine(void);
	bool IsNeedSaveInsiderTransaction(void);
	bool IsNeedSaveInsiderSentiment(void);

protected:
	vector<CWorldStockPtr> m_vWorldStock;
	map<CString, long> m_mapWorldStock;
	long m_lLastTotalWorldStock;
};
