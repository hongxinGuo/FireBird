#pragma once
#include"FinnhubForexSymbol.h"

#include"WorldStock.h"
#include"WorldMarket.h"

namespace testing {
	class CMockWorldMarket : public CWorldMarket {
	public:
		MOCK_METHOD(bool, RunningthreadUpdateDayLneStartEndDate, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateDayLineDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateStockProfileDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateForexDayLineDB, (CFinnhubForexSymbol* pSymbol), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateForexSymbolDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateForexExchangeDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateCryptoDayLineDB, (CFinnhubCryptoSymbol* pSymbol), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateCryptoSymbolDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateCryptoExchangeDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateCountryListDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateEPSSurpriseDB, (CWorldStock* pStock), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateTiingoIndustry, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateSICIndustry, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateNaicsIndustry, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateTiingoStockDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateInsiderTransactionDB, (), (override));
		MOCK_METHOD(bool, CreatingThreadUpdateEconomicCalendarDB, (), (override));

		MOCK_METHOD(bool, TaskInquiryFinnhubCountryList, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubCompanySymbol, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubCompanyProfileConcise, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubDayLine, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubRTQuote, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubPeer, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubInsiderTransaction, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubEconomicCalendar, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubEPSSurprise, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubForexExchange, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubForexSymbol, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubForexDayLine, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubCryptoExchange, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubCryptoSymbol, (), (override));
		MOCK_METHOD(bool, TaskInquiryFinnhubCryptoDayLine, (), (override));

		MOCK_METHOD(bool, TaskInquiryTiingoCompanySymbol, (), (override));
		MOCK_METHOD(bool, TaskInquiryTiingoDayLine, (), (override));

		MOCK_METHOD(bool, ProcessFinnhubStockProfile, (CWebDataPtr pWebData, CWorldStockPtr& pStock), (override));
		MOCK_METHOD(bool, ProcessFinnhubStockProfileConcise, (CWebDataPtr pWebData, CWorldStockPtr& pStock), (override));
		MOCK_METHOD(bool, ProcessFinnhubStockSymbol, (CWebDataPtr pWebData, vector<CWorldStockPtr>& vStock), (override));
		MOCK_METHOD(bool, ProcessFinnhubStockCandle, (CWebDataPtr pWebData, CWorldStockPtr& pStock), (override));
		MOCK_METHOD(bool, ProcessFinnhubStockQuote, (CWebDataPtr pWebData, CWorldStockPtr& pStock), (override));
		MOCK_METHOD(bool, ProcessFinnhubForexExchange, (CWebDataPtr pWebData, vector<CString>& vExchange), (override));
		MOCK_METHOD(bool, ProcessFinnhubForexSymbol, (CWebDataPtr pWebData, vector<CForexSymbolPtr>& vForexSymbol), (override));
		MOCK_METHOD(bool, ProcessFinnhubForexCandle, (CWebDataPtr pWebData, CForexSymbolPtr& pForexSymbol), (override));
		MOCK_METHOD(bool, ProcessFinnhubCryptoExchange, (CWebDataPtr pWebData, vector<CString>& vExchange), (override));
		MOCK_METHOD(bool, ProcessFinnhubCryptoSymbol, (CWebDataPtr pWebData, vector<CCryptoSymbolPtr>& vCryptoSymbol), (override));
		MOCK_METHOD(bool, ProcessFinnhubCryptoCandle, (CWebDataPtr pWebData, CCryptoSymbolPtr& pCryptoSymbol), (override));
		MOCK_METHOD(bool, ProcessFinnhubCountryList, (CWebDataPtr pWebData, vector<CCountryPtr>& vCountry), (override));
		MOCK_METHOD(bool, ProcessFinnhubStockPeer, (CWebDataPtr pWebData, CWorldStockPtr& pStock), (override));
		MOCK_METHOD(bool, ProcessFinnhubStockInsiderTransaction, (CWebDataPtr pWebData, vector<CInsiderTransactionPtr>& vInsiderTransaction), (override));
		MOCK_METHOD(bool, ProcessFinnhubEconomicCalendar, (CWebDataPtr pWebData, vector<CEconomicCalendarPtr>& m_vEconomicCalendar), (override));
		MOCK_METHOD(bool, ProcessFinnhubEPSSurprise, (CWebDataPtr pWebData, vector<CEPSSurprisePtr>& vEPSSurprise), (override));

		MOCK_METHOD(bool, ProcessTiingoStockSymbol, (CWebDataPtr pWebData, vector<CTiingoStockPtr>& vStock), (override));
		MOCK_METHOD(bool, ProcessTiingoStockDayLine, (CWebDataPtr pWebData, CWorldStockPtr& pStock), (override));

		MOCK_METHOD(bool, UpdateCountryListDB, (), (override));
		MOCK_METHOD(bool, UpdateStockProfileDB, (), (override));
		MOCK_METHOD(bool, UpdateStockDayLineDB, (), (override));
		MOCK_METHOD(bool, UpdateForexSymbolDB, (), (override));
		MOCK_METHOD(bool, UpdateForexExchangeDB, (), (override));
		MOCK_METHOD(bool, UpdateCryptoSymbolDB, (), (override));
		MOCK_METHOD(bool, UpdateCryptoExchangeDB, (), (override));
		MOCK_METHOD(bool, UpdateInsiderTransactionDB, (), (override));
		MOCK_METHOD(bool, UpdateStockDayLineStartEndDate, (), (override));
		MOCK_METHOD(bool, UpdateTiingoStockDB, (), (override));
		MOCK_METHOD(bool, UpdateTiingoIndustry, (), (override));
		MOCK_METHOD(bool, UpdateSICIndustry, (), (override));
		MOCK_METHOD(bool, UpdateNaicsIndustry, (), (override));
		MOCK_METHOD(bool, UpdateEconomicCalendarDB, (), (override));
	};
	typedef shared_ptr<CMockWorldMarket> CMockWorldMarketPtr;
}
