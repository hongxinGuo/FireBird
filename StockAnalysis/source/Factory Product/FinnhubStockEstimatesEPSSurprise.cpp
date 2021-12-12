#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "FinnhubStockEstimatesEPSSurprise.h"

CFinnhubStockEstimatesEPSSurprise::CFinnhubStockEstimatesEPSSurprise() {
	m_strClassName = _T("Finnhub estimates EPS surprise");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
	m_lIndex = -1;
}

CString CFinnhubStockEstimatesEPSSurprise::CreatMessage(void) {
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	CString strMiddle = pStock->GetSymbol();

	return m_strInquiringStr + strMiddle;
}

bool CFinnhubStockEstimatesEPSSurprise::ProcessWebData(CWebDataPtr pWebData)
{
	vector<CEPSSurprisePtr> vEPSSurprise;

	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	if (gl_pWorldMarket->ParseFinnhubEPSSurprise(pWebData, vEPSSurprise)) {
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