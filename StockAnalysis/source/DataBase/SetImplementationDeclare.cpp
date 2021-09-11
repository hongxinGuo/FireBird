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

#include "SetWorldChoicedStock.h"
#include "SetWorldChoicedCrypto.h"
#include "SetWorldChoicedForex.h"
#include "SetChinaChoicedStock.h"

#include "SetDayLineBasicInfo.h"
#include "SetWeekLineBasicInfo.h"
#include "SetWorldStockDayLine.h"
#include "SetCryptoDayLine.h"
#include "SetForexDayLine.h"

IMPLEMENT_DYNAMIC(CSetFinnhubStockExchange, CVirtualSetExchange)
IMPLEMENT_DYNAMIC(CSetFinnhubCryptoExchange, CVirtualSetExchange)
IMPLEMENT_DYNAMIC(CSetFinnhubForexExchange, CVirtualSetExchange)

IMPLEMENT_DYNAMIC(CSetWorldChoicedStock, CVirtualSetChoicedStake)
IMPLEMENT_DYNAMIC(CSetWorldChoicedCrypto, CVirtualSetChoicedStake)
IMPLEMENT_DYNAMIC(CSetWorldChoicedForex, CVirtualSetChoicedStake)

IMPLEMENT_DYNAMIC(CSetChinaChoicedStock, CVirtualSetChoicedStake)

IMPLEMENT_DYNAMIC(CSetDayLineBasicInfo, CVirtualSetHistoryCandle)
IMPLEMENT_DYNAMIC(CSetWeekLineBasicInfo, CVirtualSetHistoryCandle)
IMPLEMENT_DYNAMIC(CSetWorldStockDayLine, CVirtualSetHistoryCandle)
IMPLEMENT_DYNAMIC(CSetForexDayLine, CVirtualSetHistoryCandle)
IMPLEMENT_DYNAMIC(CSetCryptoDayLine, CVirtualSetHistoryCandle)