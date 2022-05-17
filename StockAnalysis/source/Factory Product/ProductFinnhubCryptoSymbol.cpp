#include "pch.h"

#include"globedef.h"
#include"accessory.h"

#include "ProductFinnhubCryptoSymbol.h"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

IMPLEMENT_DYNCREATE(CProductFinnhubCryptoSymbol, CProductWebSourceData)

CProductFinnhubCryptoSymbol::CProductFinnhubCryptoSymbol() {
	m_strClassName = _T("Finnhub crypto symbols");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/crypto/symbol?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoSymbol::CreatMessage(void) {
	CString strMiddle = ((CWorldMarket*)m_pMarket)->GetCryptoExchange(m_lIndex);

	return m_strInquiringStr + strMiddle;
}

bool CProductFinnhubCryptoSymbol::ProcessWebData(CWebDataPtr pWebData) {
	CFinnhubCryptoSymbolVectorPtr pvCryptoSymbol;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvCryptoSymbol = ParseFinnhubCryptoSymbol(pWebData);
	for (auto& pSymbol : *pvCryptoSymbol) {
		if (!((CWorldMarket*)m_pMarket)->IsFinnhubCryptoSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(((CWorldMarket*)m_pMarket)->GetCryptoExchange(m_lIndex));
			((CWorldMarket*)m_pMarket)->AddFinnhubCryptoSymbol(pSymbol);
		}
	}

	return true;
}

CFinnhubCryptoSymbolVectorPtr CProductFinnhubCryptoSymbol::ParseFinnhubCryptoSymbol(CWebDataPtr pWebData) {
	CFinnhubCryptoSymbolVectorPtr pvCryptoSymbol = make_shared<vector<CFinnhubCryptoSymbolPtr>>();
	CFinnhubCryptoSymbolPtr pSymbol = nullptr;
	ptree pt2;
	string s;
	string sError;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsParsed()) {
		if (pWebData->IsVoidJSon()) return pvCryptoSymbol;
		ppt = pWebData->GetPTree();
		try {
			for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
				pSymbol = make_shared<CFinnhubCryptoSymbol>();
				pt2 = it->second;
				s = pt2.get<string>(_T("description"));
				if (s.size() > 0) pSymbol->SetDescription(s.c_str());
				s = pt2.get<string>(_T("displaySymbol"));
				pSymbol->SetDisplaySymbol(s.c_str());
				s = pt2.get<string>(_T("symbol"));
				pSymbol->SetSymbol(s.c_str());
				pvCryptoSymbol->push_back(pSymbol);
			}
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Symbol "), e);
		}
	}
	return pvCryptoSymbol;
}