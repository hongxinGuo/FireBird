module;

export module FireBirdLib.Container.Stock.TiingoCryptoSymbol;

import FireBirdLib.Container.Stock;
import FireBirdLib.Stock.TiingoCrypto;

import std;
using std::shared_ptr;
using std::string;

export {
	class CContainerTiingoCryptoSymbol : public CContainerVirtualStock {
	public:
		CContainerTiingoCryptoSymbol();
		CContainerTiingoCryptoSymbol(const CContainerTiingoCryptoSymbol& other) = delete;
		CContainerTiingoCryptoSymbol(CContainerTiingoCryptoSymbol&& other) noexcept = delete;
		CContainerTiingoCryptoSymbol& operator=(const CContainerTiingoCryptoSymbol& other) = delete;
		CContainerTiingoCryptoSymbol& operator=(CContainerTiingoCryptoSymbol&& other) noexcept = delete;
		~CContainerTiingoCryptoSymbol() override = default;
		void Reset() override;

		shared_ptr<CTiingoCrypto> GetCrypto(size_t lIndex);
		shared_ptr<CTiingoCrypto> GetCrypto(const string& strCryptoCode);

		void UpdateDB();
		bool LoadDB();
	};

	extern CContainerTiingoCryptoSymbol gl_dataContainerTiingoCryptoSymbol;
}