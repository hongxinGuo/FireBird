#include"pch.h"
module;
#include"nlohmannJsonHeader.h"
export module FireBird.Container.Chosen.Crypto;

import FireBird.Container.VirtualStock;
import FireBird.Stock.FinnhubCrypto;

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

		CFinnhubCryptoPtr GetCryptoSymbol(size_t lIndex) { return dynamic_pointer_cast<CFinnhubCrypto>(Get(lIndex)); }
		CFinnhubCryptoPtr GetCryptoSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubCrypto>(Get(strStockCode)); }

	protected:
		size_t m_lChosenCryptoPos;
	};
}
