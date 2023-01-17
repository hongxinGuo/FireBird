#pragma once

#include"WorldStock.h"
#include"ContainerVirtualStock.h"

class CContainerWorldStock : public CContainerVirtualStock {
public:
	CContainerWorldStock();
	~CContainerWorldStock() override = default;
	void Reset(void) override;

	bool IsStock(const CWorldStockPtr pStock) const { return IsSymbol(pStock->GetSymbol()); }
	size_t GetLastStockSize(void) const noexcept { return m_lLastTotalWorldStock; }

	bool IsCompanyNewsNeedUpdate(void);
	bool IsBasicFinancialNeedUpdate(void);

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

	bool IsNeedSaveInsiderTransaction(void);
	bool IsNeedSaveInsiderSentiment(void);

protected:
	size_t m_lLastTotalWorldStock;
};
