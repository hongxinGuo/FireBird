#pragma once

#include"stdafx.h"
#include"pch.h"

#include"FinnhubForexSymbol.h"

#include"WorldStock.h"
#include"WorldMarket.h"

namespace testing {
  class CMockWorldMarket : public CWorldMarket {
  public:
    MOCK_METHOD(bool, RunningthreadUpdateDayLneStartEndDate, (CWorldMarket* pMarket), (override));
    MOCK_METHOD(bool, RunningThreadUpdateDayLineDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateStockDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateForexDayLineDB, (CFinnhubForexSymbol* pSymbol), (override));
    MOCK_METHOD(bool, RunningThreadUpdateForexSymbolDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateCountryListDB, (), (override));
    MOCK_METHOD(bool, RunningThreadUpdateEPSSurpriseDB, (CWorldStock* pStock), (override));

    MOCK_METHOD(bool, UpdateCountryListDB, (), (override));
    MOCK_METHOD(bool, UpdateStockDB, (), (override));
    MOCK_METHOD(bool, UpdateForexSymbolDB, (), (override));
    MOCK_METHOD(bool, UpdateDayLineStartEndDate, (), (override));
  };
  typedef shared_ptr<CMockWorldMarket> CMockWorldMarketPtr;
}
