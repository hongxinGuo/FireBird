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

	vector<CEPSSurprisePtr> vEPSSurprise;

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (((CWorldMarket*)m_pMarket)->ParseFinnhubEPSSurprise(pWebData, vEPSSurprise)) {
		if (vEPSSurprise.size() > 0) {
			pStock->UpdateEPSSurprise(vEPSSurprise);
		}
		else {
			pStock->SetLastEPSSurpriseUpdateDate(19700101); // 将日期设置为更早。
			pStock->SetUpdateProfileDB(true);
		}
		pStock->m_fEPSSurpriseUpdated = true;
		pStock->m_fEPSSurpriseNeedSave = true;
	}

	return true;
}