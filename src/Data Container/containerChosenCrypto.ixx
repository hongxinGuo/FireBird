module;

export module FireBirdLib.Container.Stock.ChosenCrypto;

export import FireBirdLib.Container.Stock;
import FireBirdLib.Stock.FinnhubCrypto;

import std;
using std::shared_ptr;
using std::string;

export {
	class CContainerChosenCrypto final : public CContainerVirtualStock {
	public:
		CContainerChosenCrypto();
		CContainerChosenCrypto(const CContainerChosenCrypto& other) = delete;
		CContainerChosenCrypto(CContainerChosenCrypto&& other) noexcept = delete;
		CContainerChosenCrypto& operator=(const CContainerChosenCrypto& other) = delete;
		CContainerChosenCrypto& operator=(CContainerChosenCrypto&& other) noexcept = delete;
		~CContainerChosenCrypto() override = default;
		void Reset() override;

		bool LoadDB();
		//bool UpdateDB();

		shared_ptr<CFinnhubCrypto> GetCryptoSymbol(size_t lIndex);
		shared_ptr<CFinnhubCrypto> GetCryptoSymbol(const string& strStockCode);

	protected:
	};

	extern CContainerChosenCrypto gl_dataContainerChosenWorldCrypto;
}