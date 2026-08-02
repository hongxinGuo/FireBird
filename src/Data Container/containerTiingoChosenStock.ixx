module;

export module FireBirdLib.Container.Stock.TiingoChosenStock;

import FireBirdLib.Container.Stock;
import FireBirdLib.Container.Stock;

import FireBirdLib.Stock.TiingoStock;

import std;
using std::string;
using std::shared_ptr;

export {
	class CContainerTiingoChosenStock : public CContainerVirtualStock {
	public:
		CContainerTiingoChosenStock();
		CContainerTiingoChosenStock(const CContainerTiingoChosenStock& other) = delete;
		CContainerTiingoChosenStock(CContainerTiingoChosenStock&& other) noexcept = delete;
		CContainerTiingoChosenStock& operator=(const CContainerTiingoChosenStock& other) = delete;
		CContainerTiingoChosenStock& operator=(CContainerTiingoChosenStock&& other) noexcept = delete;
		~CContainerTiingoChosenStock() override = default;
		void Reset() override;

		bool LoadDB();
		void UpdateDB() const;

		shared_ptr<CTiingoStock> GetStock(size_t lIndex);
		shared_ptr<CTiingoStock> GetStock(const string& strStockCode);

	protected:
	};

	extern CContainerTiingoChosenStock gl_dataContainerTiingoChosenStock;
}