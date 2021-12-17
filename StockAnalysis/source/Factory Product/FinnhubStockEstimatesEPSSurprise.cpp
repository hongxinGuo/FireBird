#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "FinnhubStockEstimatesEPSSurprise.h"

IMPLEMENT_DYNCREATE(CFinnhubStockEstimatesEPSSurprise, CWebSourceDataProduct)

CFinnhubStockEstimatesEPSSurprise::CFinnhubStockEstimatesEPSSurprise() {
	m_strClassName = _T("Finnhub estimates EPS surprise");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
	m_lIndex = -1;
}

CString CFinnhubStockEstimatesEPSSurprise::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMiddle = pStock->GetSymbol();

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubStockEstimatesEPSSurprise::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CEPSSurpriseVectorPtr pvEPSSurprise;

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pvEPSSurprise = ((CWorldMarket*)m_pMarket)->ParseFinnhubEPSSurprise(pWebData);
	if (pvEPSSurprise->size() > 0) {
		pStock->UpdateEPSSurprise(*pvEPSSurprise);
	}
	else {
		pStock->SetLastEPSSurpriseUpdateDate(19700101); // ����������Ϊ���硣
		pStock->SetUpdateProfileDB(true);
	}
	pStock->m_fEPSSurpriseUpdated = true;
	pStock->m_fEPSSurpriseNeedSave = true;

	return true;
}