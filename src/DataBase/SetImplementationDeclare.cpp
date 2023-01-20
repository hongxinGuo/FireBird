//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从虚数据库类继承的，不需要实现（动作皆由虚类完成），只需要声明一下IMPLEMENT_DYNAMIC即可（MFC数据库类的特点）。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "SetFinnhubStockExchange.h"
#include "SetFinnhubCryptoExchange.h"
#include "SetFinnhubForexExchange.h"

#include "SetWorldChosenStock.h"
#include "SetWorldChosenCrypto.h"
#include "SetWorldChosenForex.h"
#include "SetChinaChosenStock.h"

#include "SetDayLineBasicInfo.h"
#include "SetWeekLineBasicInfo.h"
#include "SetWorldStockDayLine.h"
#include "SetCryptoDayLine.h"
#include "SetForexDayLine.h"

#include "SetFinnhubCryptoSymbol.h"
#include "SetFinnhubForexSymbol.h"
#include "SetChinaStockSymbol.h"

#include "SetDayLineExtendInfo.h"
#include "SetWeekLineExtendInfo.h"

#include "SetDayLineTodaySaved.h"
#include "SetCurrentWeekLine.h"

IMPLEMENT_DYNAMIC(CSetFinnhubStockExchange, CVirtualSetExchange)
IMPLEMENT_DYNAMIC(CSetFinnhubCryptoExchange, CVirtualSetExchange)
IMPLEMENT_DYNAMIC(CSetFinnhubForexExchange, CVirtualSetExchange)

IMPLEMENT_DYNAMIC(CSetWorldChosenStock, CVirtualSetChosenStake)
IMPLEMENT_DYNAMIC(CSetWorldChosenCrypto, CVirtualSetChosenStake)
IMPLEMENT_DYNAMIC(CSetWorldChosenForex, CVirtualSetChosenStake)

IMPLEMENT_DYNAMIC(CSetChinaChosenStock, CVirtualSetChosenStake)

IMPLEMENT_DYNAMIC(CSetDayLineBasicInfo, CVirtualSetHistoryCandleBasic)
IMPLEMENT_DYNAMIC(CSetWeekLineBasicInfo, CVirtualSetHistoryCandleBasic)
IMPLEMENT_DYNAMIC(CSetWorldStockDayLine, CVirtualSetHistoryCandleBasic)
IMPLEMENT_DYNAMIC(CSetForexDayLine, CVirtualSetHistoryCandleBasic)
IMPLEMENT_DYNAMIC(CSetCryptoDayLine, CVirtualSetHistoryCandleBasic)

IMPLEMENT_DYNAMIC(CSetChinaStockSymbol, CVirtualSetStockSymbol)
IMPLEMENT_DYNAMIC(CSetFinnhubCryptoSymbol, CVirtualSetStockSymbol)
IMPLEMENT_DYNAMIC(CSetFinnhubForexSymbol, CVirtualSetStockSymbol)

IMPLEMENT_DYNAMIC(CSetDayLineExtendInfo, CVirtualSetHistoryCandleExtend)
IMPLEMENT_DYNAMIC(CSetWeekLineExtendInfo, CVirtualSetHistoryCandleExtend)

IMPLEMENT_DYNAMIC(CSetDayLineTodaySaved, CVirtualSetHistoryCandle)
IMPLEMENT_DYNAMIC(CSetCurrentWeekLine, CVirtualSetHistoryCandle)
