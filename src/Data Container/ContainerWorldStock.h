#pragma once

#include"WorldStock.h"
#include"ContainerVirtualStock.h"

class CContainerWorldStock final : public CContainerVirtualStock {
public:
	CContainerWorldStock();
	// 只能有一个实例,不允许赋值。
	CContainerWorldStock(const CContainerWorldStock&) = delete;
	CContainerWorldStock& operator=(const CContainerWorldStock&) = delete;
	CContainerWorldStock(const CContainerWorldStock&&) noexcept = delete;
	CContainerWorldStock& operator=(const CContainerWorldStock&&) noexcept = delete;
	~CContainerWorldStock() override = default;
	void Reset() override;

	CWorldStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CWorldStock>(Get(lIndex)); }
	CWorldStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CWorldStock>(Get(strStockCode)); }

	void ResetEPSSurprise();
	void ResetPeer();
	void ResetBasicFinancial();
	void ResetDayLine();

	bool LoadDB();
	void UpdateProfileDB();

	bool UpdateBasicFinancialDB();
	static void UpdateBasicFinancialAnnualDB(const vector<CWorldStockPtr>& vStock);
	static void UpdateBasicFinancialQuarterDB(const vector<CWorldStockPtr>& vStock);
	void UpdateBasicFinancialMetricDB(const vector<CWorldStockPtr>& vStock);
	static void ClearUpdateBasicFinancialFlag(const vector<CWorldStockPtr>& vStock);

	static bool ValidateStockSymbol(const CWorldStockPtr& pStock);

	bool IsUpdateCompanyNewsDB();
	bool IsUpdateBasicFinancialDB();
	bool IsUpdateInsiderTransactionDB();
	bool IsUpdateInsiderSentimentDB();
	bool IsSaveEPSSurpriseDB();
	bool IsSaveSECFilingsDB();
};
