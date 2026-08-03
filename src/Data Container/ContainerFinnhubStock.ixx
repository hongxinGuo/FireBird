module;

export module FireBirdLib.Container.Stock.FinnhubStock;

import FireBirdLib.Container.Stock;
import FireBirdLib.Stock.FinnhubStock;

import std;
using std::shared_ptr;
using std::string;

export {
	class CContainerFinnhubStock final : public CContainerVirtualStock {
	public:
		CContainerFinnhubStock();
		// 只能有一个实例,不允许赋值。
		CContainerFinnhubStock(const CContainerFinnhubStock&) = delete;
		CContainerFinnhubStock& operator=(const CContainerFinnhubStock&) = delete;
		CContainerFinnhubStock(const CContainerFinnhubStock&&) noexcept = delete;
		CContainerFinnhubStock& operator=(const CContainerFinnhubStock&&) noexcept = delete;
		~CContainerFinnhubStock() override = default;
		void Reset() override;

		shared_ptr<CFinnhubStock> GetItem(size_t lIndex);
		shared_ptr<CFinnhubStock> GetItem(const string& strStockCode);

		void ResetEPSSurprise();
		void ResetPeer();
		void ResetBasicFinancial();
		void ResetDayLine();

		bool LoadProfileDB();
		void UpdateProfileDB();

		void UpdateInsiderTransactionDB();

		static bool ValidateStockSymbol(const shared_ptr<CFinnhubStock>& pStock);

		bool IsUpdateCompanyNewsDB();
		bool IsUpdateBasicFinancialDB();
		bool IsUpdateInsiderTransactionDB();
		bool IsUpdateInsiderSentimentDB();
		bool IsUpdateEPSSurpriseDB();
		bool IsUpdateSECFilingsDB();
	};

	extern CContainerFinnhubStock gl_dataContainerFinnhubStock;
}