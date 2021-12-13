#include "pch.h"
#include "TiingoStockPriceCandle.h"

#include"globedef.h"
#include"WorldStock.h"
#include"WorldMarket.h"

IMPLEMENT_DYNCREATE(CTiingoStockPriceCandle, CWebSourceDataProduct)

CTiingoStockPriceCandle::CTiingoStockPriceCandle() : CWebSourceDataProduct() {
	m_strClassName = _T("Tiingo stock price candle");
	m_strInquiringStr = _T("https://api.tiingo.com/tiingo/daily/");
	m_lIndex = -1;
}

CString CTiingoStockPriceCandle::CreatMessage(void) {
	CWorldStockPtr pStock = nullptr;
	CString strMiddle;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pStock = ((CWorldMarket*)m_pMarket)->GetStock(GetIndex());
	strMiddle = pStock->GetTiingoDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetDayLineNeedUpdate(false);

	return m_strInquiringStr + strMiddle;
}

bool CTiingoStockPriceCandle::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_lIndex >= 0);
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	((CWorldMarket*)m_pMarket)->ParseTiingoStockDayLine(pWebData, pStock);
	TRACE("处理Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());

	return true;
}