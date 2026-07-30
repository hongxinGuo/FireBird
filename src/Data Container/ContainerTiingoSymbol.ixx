module;

export module Container.Stock.TiingoSymbol;

import Container.Stock;

import Stock.TiingoStock;

import std;
using std::string;
using std::shared_ptr;

export {
	class CContainerTiingoSymbol : public CContainerVirtualStock {
	public:
		CContainerTiingoSymbol();
		CContainerTiingoSymbol(const CContainerTiingoSymbol& other) = delete;
		CContainerTiingoSymbol(CContainerTiingoSymbol&& other) noexcept = delete;
		CContainerTiingoSymbol& operator=(const CContainerTiingoSymbol& other) = delete;
		CContainerTiingoSymbol& operator=(CContainerTiingoSymbol&& other) noexcept = delete;
		~CContainerTiingoSymbol() override = default;
		void Reset() override;

		shared_ptr<CTiingoStock> GetStock(size_t lIndex);
		shared_ptr<CTiingoStock> GetStock(const string& strStockCode);
	};

	extern CContainerTiingoSymbol gl_dataContainerTiingoNewSymbol;
	extern CContainerTiingoSymbol gl_dataContainerTiingoDelistedSymbol;
}