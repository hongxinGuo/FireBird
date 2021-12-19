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

	CDayLineVectorPtr pvDayLine = nullptr;
	long lTemp = 0;

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pvDayLine = ((CWorldMarket*)m_pMarket)->ParseFinnhubStockCandle(pWebData);

	for (auto& pDayLine : *pvDayLine) {
		pDayLine->SetExchange(pStock->GetExchangeCode());
		pDayLine->SetStockSymbol(pStock->GetSymbol());
		pDayLine->SetDisplaySymbol(pStock->GetTicker());
		lTemp = TransferToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
		pDayLine->SetDate(lTemp);
	}
	if (pvDayLine->size() > 0) {
		pStock->UpdateDayLine(*pvDayLine);
		pStock->SetDayLineNeedUpdate(false);
		pStock->SetDayLineNeedSaving(true);
		pStock->SetUpdateProfileDB(true);
		if (!IsEarlyThen(pStock->GetDayLine(pStock->GetDayLineSize() - 1)->GetMarketDate(), ((CWorldMarket*)m_pMarket)->GetMarketDate(), 100)) {
			pStock->SetIPOStatus(__STOCK_IPOED__);
		}
	}
	//TRACE("处理%s日线数据\n", pStock->GetSymbol().GetBuffer());

	return true;
}