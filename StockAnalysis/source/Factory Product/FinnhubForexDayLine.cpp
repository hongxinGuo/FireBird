#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"FinnhubForexSymbol.h"

#include "FinnhubForexDayLine.h"

IMPLEMENT_DYNCREATE(CFinnhubForexDayLine, CWebSourceDataProduct)

CFinnhubForexDayLine::CFinnhubForexDayLine() {
	m_strClassName = _T("Finnhub forex dayline");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
	m_lIndex = -1;
}

CString CFinnhubForexDayLine::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CForexSymbolPtr pForexSymbol = ((CWorldMarket*)m_pMarket)->GetForexSymbol(m_lIndex);
	CString strMessage = m_strInquiringStr + pForexSymbol->GetFinnhubDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetUTCTime());
	pForexSymbol->SetDayLineNeedUpdate(false);

	return strMessage;
}

bool CFinnhubForexDayLine::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CDayLineVectorPtr pvDayLine = nullptr;
	long lTemp = 0;

	CForexSymbolPtr pForexSymbol = ((CWorldMarket*)m_pMarket)->GetForexSymbol(m_lIndex);
	pvDayLine = ((CWorldMarket*)m_pMarket)->ParseFinnhubForexCandle(pWebData);
	if (pvDayLine->size() > 0) {
		for (auto pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pForexSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pForexSymbol->GetSymbol());
			lTemp = TransferToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pForexSymbol->SetIPOStatus(__STOCK_IPOED__);
		pForexSymbol->UpdateDayLine(*pvDayLine);
		pForexSymbol->SetDayLineNeedUpdate(false);
		pForexSymbol->SetDayLineNeedSaving(true);
		pForexSymbol->SetUpdateProfileDB(true);
		TRACE("处理%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
	}
	return true;
}