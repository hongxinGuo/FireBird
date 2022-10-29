#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

IMPLEMENT_DYNCREATE(CProductFinnhub, CVirtualProductWebData)

bool CProductFinnhub::AddInaccessibleExchange(void) {
	gl_finnhubInaccessibleExchange.m_fUpdate = true;
	try {
		CInaccessibleExchangesPtr pExchange = gl_finnhubInaccessibleExchange.m_mapInaccessibleExchange.at(m_iProductType);
		pExchange->AddExchange(m_strInquiringExchange);

		return true;
	}
	catch (out_of_range&) {
		CInaccessibleExchangesPtr pNewExchange = make_shared<CInaccessibleExchanges>();
		pNewExchange->m_iFunction = m_iProductType;
		pNewExchange->m_sFunction = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(m_iProductType);
		pNewExchange->m_vExchange.push_back(m_strInquiringExchange);
		pNewExchange->m_setExchange.insert(m_strInquiringExchange);
		gl_finnhubInaccessibleExchange.m_mapInaccessibleExchange[m_iProductType] = pNewExchange;
		return true;
	}
}