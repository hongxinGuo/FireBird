#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "FinnhubStockDayLine.h"

IMPLEMENT_DYNCREATE(CFinnhubStockDayLine, CWebSourceDataProduct)

CFinnhubStockDayLine::CFinnhubStockDayLine() {
	m_strClassName = _T("Finnhub company profile concise");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/candle?symbol=");
	m_lIndex = -1;
}

CString CFinnhubStockDayLine::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMiddle = pStock->GetFinnhubDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetUTCTime());
	pStock->SetDayLineNeedUpdate(false);

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubStockDayLine::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (((CWorldMarket*)m_pMarket)->ParseFinnhubStockCandle(pWebData, pStock)) {
		if (pStock->GetDayLineSize() == 0) { // 没有日线数据？
			if (pStock->IsNotChecked()) { // 尚未确定代码有效性？
				pStock->SetIPOStatus(__STOCK_NULL__);
			}
		}
		else if (IsEarlyThen(pStock->GetDayLine(pStock->GetDayLineSize() - 1)->GetMarketDate(), ((CWorldMarket*)m_pMarket)->GetMarketDate(), 100)) {
			pStock->SetIPOStatus(__STOCK_DELISTED__);
		}
		else {
			pStock->SetIPOStatus(__STOCK_IPOED__);
		}
		//TRACE("处理%s日线数据\n", pStock->GetSymbol().GetBuffer());
	}
	return true;
}