//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ݿ���̳еģ�����Ҫʵ�֣���������������ɣ���ֻ��Ҫ����һ��IMPLEMENT_DYNAMIC���ɣ�MFC���ݿ�����ص㣩��
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

#include "SetFinnhubCryptoSymbol.h"
#include "SetFinnhubForexSymbol.h"
#include "SetChinaStockSymbol.h"

#include "SetDayLineExtendInfo.h"
#include "SetWeekLineExtendInfo.h"

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

IMPLEMENT_DYNAMIC(CSetChinaStockSymbol, CVirtualSetStockSymbol)
IMPLEMENT_DYNAMIC(CSetFinnhubCryptoSymbol, CVirtualSetStockSymbol)
IMPLEMENT_DYNAMIC(CSetFinnhubForexSymbol, CVirtualSetStockSymbol)

IMPLEMENT_DYNAMIC(CSetDayLineExtendInfo, CVirtualSetHistoryCandleExtend)
IMPLEMENT_DYNAMIC(CSetWeekLineExtendInfo, CVirtualSetHistoryCandleExtend)