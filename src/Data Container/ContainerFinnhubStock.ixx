#include"pch.h"
module;
#include"FinnhubStock.h"
export module FireBird.Container.FinnhubStock;

import FireBird.Container.VirtualStock;

export {
	class CContainerFinnhubStock final : public CContainerVirtualStock {
	public:
		CContainerFinnhubStock();
		// 只能有一个实例,不允许赋值。
		CContainerFinnhubStock(const CContainerFinnhubStock&) = delete;
		CContainerFinnhubStock& operator=(const CContainerFinnhubStock&) = delete;
		CContainerFinnhubStock(const CContainerFinnhubStock&&) noexcept = delete;
		CContainerFinnhubStock& operator=(const CContainerFinnhubStock&&) noexcept = delete;
		~CContainerFinnhubStock() override;
		void Reset() override;

		CFinnhubStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubStock>(Get(lIndex)); }
		CFinnhubStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubStock>(Get(strStockCode)); }

		void ResetEPSSurprise();
		void ResetPeer();
		void ResetBasicFinancial();
		void ResetDayLine();

		bool LoadDB();
		void UpdateProfileDB();

		bool UpdateBasicFinancialDB();
		static void UpdateBasicFinancialAnnualDB(const vector<CFinnhubStockPtr>& vStock);
		static void UpdateBasicFinancialQuarterDB(const vector<CFinnhubStockPtr>& vStock);
		void UpdateBasicFinancialMetricDB(const vector<CFinnhubStockPtr>& vStock);
		static void ClearUpdateBasicFinancialFlag(const vector<CFinnhubStockPtr>& vStock);

		static bool ValidateStockSymbol(const CFinnhubStockPtr& pStock);

		bool IsUpdateCompanyNewsDB();
		bool IsUpdateBasicFinancialDB();
		bool IsUpdateInsiderTransactionDB();
		bool IsUpdateInsiderSentimentDB();
		bool IsUpdateEPSSurpriseDB();
		bool IsUpdateSECFilingsDB();
	};
}
