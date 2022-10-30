#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

IMPLEMENT_DYNCREATE(CProductFinnhub, CVirtualProductWebData)

bool CProductFinnhub::AddInaccessibleExchangeIfNeeded(void) {
	gl_finnhubInaccessibleExchange.SetUpdate(true);
	try {
		CInaccessibleExchangesPtr pExchange = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(m_iProductType);
		if (!pExchange->HaveExchange(m_strInquiringExchange)) { // ������µĽ���������
			pExchange->AddExchange(m_strInquiringExchange);
			return true;
		}
		else {
			return false;
		}
	}
	catch (out_of_range&) { // �µ�����
		CInaccessibleExchangesPtr pNewExchange = make_shared<CInaccessibleExchanges>();
		pNewExchange->SetFunction(m_iProductType);
		pNewExchange->SetFunctionString(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(m_iProductType));
		pNewExchange->AddExchange(m_strInquiringExchange);
		gl_finnhubInaccessibleExchange.SetInaccessibleExchange(m_iProductType, pNewExchange);
		return true;
	}
}