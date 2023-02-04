#pragma once

#include"FinnhubDataSource.h"

namespace testing {
	class CMockFinnhubDataSource final : public CFinnhubDataSource {
	public:
		MOCK_METHOD(void, StartReadingThread, (), (override));
		MOCK_METHOD(bool, ReadingWebData, (), (override));
		MOCK_METHOD(bool, InquireCountryList, (), (override));
		MOCK_METHOD(bool, InquireCompanySymbol, (), (override));
		MOCK_METHOD(bool, InquireCompanyProfileConcise, (), (override));
		MOCK_METHOD(bool, InquireCompanyNews, (), (override));
		MOCK_METHOD(bool, InquireCompanyBasicFinancial, (), (override));
		MOCK_METHOD(bool, InquireStockDayLine, (), (override));
		MOCK_METHOD(bool, InquireRTQuote, (), (override));
		MOCK_METHOD(bool, InquirePeer, (), (override));
		MOCK_METHOD(bool, InquireInsiderTransaction, (), (override));
		MOCK_METHOD(bool, InquireInsiderSentiment, (), (override));
		MOCK_METHOD(bool, InquireEconomicCalendar, (), (override));
		MOCK_METHOD(bool, InquireEPSSurprise, (), (override));
		MOCK_METHOD(bool, InquireForexExchange, (), (override));
		MOCK_METHOD(bool, InquireForexSymbol, (), (override));
		MOCK_METHOD(bool, InquireForexDayLine, (), (override));
		MOCK_METHOD(bool, InquireCryptoExchange, (), (override));
		MOCK_METHOD(bool, InquireCryptoSymbol, (), (override));
		MOCK_METHOD(bool, InquireCryptoDayLine, (), (override));
	};

	using CMockFinnhubDataSourcePtr = shared_ptr<CMockFinnhubDataSource>;
}
