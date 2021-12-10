#include "pch.h"
#include "TiingoStockPriceCandle.h"

#include"globedef.h"
#include"WorldStock.h"
#include"WorldMarket.h"

CTiingoStockPriceCandle::CTiingoStockPriceCandle() : CWebSourceDataProduct() {
	m_strClassName = _T("Tiingo stock price candle");
	m_strInquiringStr = _T("https://api.tiingo.com/tiingo/daily/");
	m_lIndex = -1;
}

CString CTiingoStockPriceCandle::CreatMessage(void) {
	CWorldStockPtr pStock = nullptr;
	CString strMiddle;

	pStock = gl_pWorldMarket->GetStock(GetIndex());
	strMiddle = pStock->GetTiingoDayLineInquiryString(gl_pWorldMarket->GetMarketDate());
	pStock->SetDayLineNeedUpdate(false);

	return m_strInquiringStr + strMiddle;
}

bool CTiingoStockPriceCandle::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_lIndex >= 0);

	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	gl_pWorldMarket->ParseTiingoStockDayLine(pWebData, pStock);
	TRACE("处理Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());

	return true;
}