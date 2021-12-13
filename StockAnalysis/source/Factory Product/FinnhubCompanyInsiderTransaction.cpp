#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "FinnhubCompanyInsiderTransaction.h"

IMPLEMENT_DYNCREATE(CFinnhubCompanyInsiderTransaction, CWebSourceDataProduct)

CFinnhubCompanyInsiderTransaction::CFinnhubCompanyInsiderTransaction() {
	m_strClassName = _T("Finnhub company insider transaction");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol=");
	m_lIndex = -1;
}

CString CFinnhubCompanyInsiderTransaction::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetInsiderTransactionNeedUpdate(false);

	return strMessage;
}

bool CFinnhubCompanyInsiderTransaction::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	vector<CInsiderTransactionPtr> vInsiderTransaction;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (((CWorldMarket*)m_pMarket)->ParseFinnhubStockInsiderTransaction(pWebData, vInsiderTransaction)) {
		pStock->SetInsiderTransactionUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
		if (vInsiderTransaction.size() > 0) {
			pStock->UpdateInsiderTransaction(vInsiderTransaction);
			pStock->SetInsiderTransactionNeedSave(true);
		}
	}

	return true;
}