#include"pch.h"

#include "SetFinnhubStockExchange.h"
#include "SetFinnhubCryptoExchange.h"
#include "SetFinnhubForexExchange.h"

#include "SetWorldChoicedStock.h"
#include "SetWorldChoicedCrypto.h"
#include "SetWorldChoicedForex.h"

IMPLEMENT_DYNAMIC(CSetFinnhubStockExchange, CVirtualSetExchange)
IMPLEMENT_DYNAMIC(CSetFinnhubCryptoExchange, CVirtualSetExchange)
IMPLEMENT_DYNAMIC(CSetFinnhubForexExchange, CVirtualSetExchange)

IMPLEMENT_DYNAMIC(CSetWorldChoicedStock, CVirtualSetChoicedStake)
IMPLEMENT_DYNAMIC(CSetWorldChoicedCrypto, CVirtualSetChoicedStake)
IMPLEMENT_DYNAMIC(CSetWorldChoicedForex, CVirtualSetChoicedStake)