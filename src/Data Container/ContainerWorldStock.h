#pragma once

#include"WorldStock.h"
#include"ContainerVirtualStock.h"

class CContainerWorldStock final : public CContainerVirtualStock {
public:
	CContainerWorldStock();
	~CContainerWorldStock() override = default;
	void Reset(void) override;

	CWorldStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CWorldStock>(Get(lIndex)); };
	CWorldStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CWorldStock>(Get(strStockCode)); };
	size_t GetLastStockSize(void) const noexcept { return m_lLastTotalWorldStock; }

	void ResetEPSSurprise(void);
	void ResetPeer(void);
	void ResetBasicFinancial(void);
	void ResetDayLine(void);

	bool LoadDB(void);
	bool UpdateProfileDB(void);

	bool UpdateBasicFinancialDB(void);
	bool UpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock);
	bool UpdateBasicFinancialQuarterDB(vector<CWorldStockPtr> vStock);
	bool UpdateBasicFinancialMetricDB(vector<CWorldStockPtr> vStock);
	void ClearUpdateBasicFinancialFlag(vector<CWorldStockPtr> vStock);

	bool CheckStockSymbol(CWorldStockPtr pStock);

	bool IsUpdateCompanyNewsDB(void);
	bool IsUpdateBasicFinancialDB(void);
	bool IsSaveInsiderTransaction(void);
	bool IsSaveInsiderSentiment(void);

protected:
	size_t m_lLastTotalWorldStock;
};
