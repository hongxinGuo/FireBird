#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubCompanyPeer.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyPeer, CProductWebSourceData)

CProductFinnhubCompanyPeer::CProductFinnhubCompanyPeer() {
	m_strClassName = _T("Finnhub company peer");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyPeer::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetPeerUpdated(true);

	return strMessage;
}

bool CProductFinnhubCompanyPeer::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strPeer = ParseFinnhubStockPeer(pWebData);
	if (strPeer.GetLength() > 0) {
		pStock->SetPeer(strPeer);
		pStock->SetPeerUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
	}

	return true;
}

CString CProductFinnhubCompanyPeer::ParseFinnhubStockPeer(CWebDataPtr pWebData) {
	CString strPeer = _T("");
	char buffer[1000]{};
	int i = 0;
	ptree pt;
	string sError;

	if (pWebData->GetBufferLength() <= 3) {
		return strPeer; // û����Ч��ͬҵ��������
	}
	if (!ConvertToJSON(pt, pWebData)) return strPeer;

	ASSERT(pWebData->GetBufferLength() < 1000);
	for (i = 0; i < pWebData->GetBufferLength(); i++) {
		buffer[i] = pWebData->GetData(i);
	}
	if (i > 200) {
		buffer[200] = 0x000;
	}
	else {
		buffer[pWebData->GetBufferLength()] = 0x000;
	}
	strPeer = buffer;

	return strPeer;
}