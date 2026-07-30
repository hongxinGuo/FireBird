module;

export module Container.Stock.FinnhubCrypto;

import Container.Stock;

import Stock.FinnhubCrypto;

import std;
using std::string;
using std::shared_ptr;

export {
	class CContainerFinnhubCrypto : public CContainerVirtualStock {
	public:
		CContainerFinnhubCrypto();
		CContainerFinnhubCrypto(const CContainerFinnhubCrypto& other) = delete;
		CContainerFinnhubCrypto(CContainerFinnhubCrypto&& other) noexcept = delete;
		CContainerFinnhubCrypto& operator=(const CContainerFinnhubCrypto& other) = delete;
		CContainerFinnhubCrypto& operator=(CContainerFinnhubCrypto&& other) noexcept = delete;
		~CContainerFinnhubCrypto() override = default;
		void Reset() override;

		bool LoadProfileDB();
		void UpdateProfileDB();

		shared_ptr<CFinnhubCrypto> GetItem(size_t lIndex);
		shared_ptr<CFinnhubCrypto> GetItem(const string& strStockCode);

	protected:
		size_t m_llLastTotalSymbol;
	};

	extern CContainerFinnhubCrypto gl_dataFinnhubCryptoSymbol;
}