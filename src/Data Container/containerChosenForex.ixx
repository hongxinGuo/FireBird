module;

export module FireBirdLib.Container.Stock.ChosenForex;

export import FireBirdLib.Container.Stock;

import FireBirdLib.Stock.FinnhubForex;

import std;
using std::shared_ptr;
using std::string;

export {
	class CContainerChosenForex : public CContainerVirtualStock {
	public:
		CContainerChosenForex();
		CContainerChosenForex(const CContainerChosenForex& other) = delete;
		CContainerChosenForex(CContainerChosenForex&& other) noexcept = delete;
		CContainerChosenForex& operator=(const CContainerChosenForex& other) = delete;
		CContainerChosenForex& operator=(CContainerChosenForex&& other) noexcept = delete;
		~CContainerChosenForex() override = default;
		void Reset() override;

		bool LoadDB();
		//bool UpdateDB();

		shared_ptr<CFinnhubForex> GetForexSymbol(const size_t lIndex);
		shared_ptr<CFinnhubForex> GetForexSymbol(const string& strStockCode);

	protected:
	};

	extern CContainerChosenForex gl_dataContainerChosenWorldForex;
}