module;

export module FireBirdLib.Container.HistoryCandle.CryptoDayLine;

import FireBirdLib.Container.HistoryCandle;

import std;
using std::string;
using std::shared_ptr;

export {
	class CContainerCryptoDayLine final : public CVirtualDataHistoryCandle {
	public:
		CContainerCryptoDayLine();
		// 单一实例，不允许赋值
		CContainerCryptoDayLine(const CContainerCryptoDayLine& other) = delete;
		CContainerCryptoDayLine(CContainerCryptoDayLine&& other) = delete;
		CContainerCryptoDayLine& operator=(const CContainerCryptoDayLine& other) = delete;
		CContainerCryptoDayLine& operator=(CContainerCryptoDayLine&& other) = delete;
		~CContainerCryptoDayLine() override = default;

		void SaveDB(const string& strCryptoSymbol) override;
		void LoadDB(const string& strCryptoSymbol) override;

		// 特有函数
	};

	using CContainerCryptoDayLinePtr = shared_ptr<CContainerCryptoDayLine>;
}