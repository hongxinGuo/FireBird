#pragma once
#include"FinnhubForexSymbol.h"

#include"WorldStock.h"
#include"WorldMarket.h"

namespace testing {
  class CMockWorldMarket : public CWorldMarket {
  public:
    MOCK_METHOD(bool, RunningthreadUpdateDayLneStartEndDate, (CWorldMarket* pMarket), (override));
    MOCK_METHOD(bool, RunningThreadUpdateDayLineDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateStockProfileDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateForexDayLineDB, (CFinnhubForexSymbol* pSymbol), (override));
    MOCK_METHOD(bool, RunningThreadUpdateForexSymbolDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateCountryListDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateEPSSurpriseDB, (CWorldStock* pStock), (override));

    MOCK_METHOD(bool, TaskInquiryFinnhubCountryList, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubCompanySymbol, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubCompanyProfileConcise, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubDayLine, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubRTQuote, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubPeer, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubEconomicCalendar, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubEPSSurprise, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubForexExchange, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubForexSymbol, (), (override));
    MOCK_METHOD(bool, TaskInquiryFinnhubForexDayLine, (), (override));

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
    MOCK_METHOD(bool, ProcessFinnhubCountryList, (CWebDataPtr pWebData, vector<CCountryPtr>& vCountry), (override));
    MOCK_METHOD(bool, ProcessFinnhubStockPeer, (CWebDataPtr pWebData, CWorldStockPtr& pStock), (override));
    MOCK_METHOD(bool, ProcessFinnhubEconomicCalendar, (CWebDataPtr pWebData, vector<CEconomicCalendarPtr>& m_vEconomicCalendar), (override));
    MOCK_METHOD(bool, ProcessFinnhubEPSSurprise, (CWebDataPtr pWebData, vector<CEPSSurprisePtr>& vEPSSurprise), (override));

    MOCK_METHOD(bool, ProcessTiingoStockSymbol, (CWebDataPtr pWebData, vector<CWorldStockPtr>& vStock), (override));
    MOCK_METHOD(bool, ProcessTiingoStockDayLine, (CWebDataPtr pWebData, CWorldStockPtr& pStock), (override));

    MOCK_METHOD(bool, UpdateCountryListDB, (), (override));
    MOCK_METHOD(bool, UpdateStockProfileDB, (), (override));
    MOCK_METHOD(bool, UpdateForexSymbolDB, (), (override));
    MOCK_METHOD(bool, UpdateStockDayLineStartEndDate, (), (override));
  };
  typedef shared_ptr<CMockWorldMarket> CMockWorldMarketPtr;
}