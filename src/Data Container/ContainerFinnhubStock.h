#pragma once

#include"FinnhubStock.h"
#include"ContainerVirtualStock.h"

class CContainerFinnhubStock final : public CContainerVirtualStock {
public:
	CContainerFinnhubStock();
	// 只能有一个实例,不允许赋值。
	CContainerFinnhubStock(const CContainerFinnhubStock&) = delete;
	CContainerFinnhubStock& operator=(const CContainerFinnhubStock&) = delete;
	CContainerFinnhubStock(const CContainerFinnhubStock&&) noexcept = delete;
	CContainerFinnhubStock& operator=(const CContainerFinnhubStock&&) noexcept = delete;
	~CContainerFinnhubStock() override = default ;
	void Reset() override;

	CFinnhubStockPtr GetItem(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubStock>(Get(lIndex)); }
	CFinnhubStockPtr GetItem(const string& strStockCode) { return dynamic_pointer_cast<CFinnhubStock>(Get(strStockCode)); }

	void ResetEPSSurprise();
	void ResetPeer();
	void ResetBasicFinancial();
	void ResetDayLine();

	bool LoadProfileDB();
	void UpdateProfileDB();

	void UpdateInsiderTransactionDB();

	static bool ValidateStockSymbol(const CFinnhubStockPtr& pStock);

	bool IsUpdateCompanyNewsDB();
	bool IsUpdateBasicFinancialDB();
	bool IsUpdateInsiderTransactionDB();
	bool IsUpdateInsiderSentimentDB();
	bool IsUpdateEPSSurpriseDB();
	bool IsUpdateSECFilingsDB();
};
