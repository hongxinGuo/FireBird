#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

IMPLEMENT_DYNCREATE(CProductFinnhub, CVirtualProductWebData)

bool CProductFinnhub::CheckNoRightToAccess(CWebDataPtr pWebData) {
	if (pWebData->CheckNoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return true; }
	else return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Ŀǰfinnhub��������˻�ֻ�ṩ�����������ݡ�Ϊ�˼�����ѯ�ٶȣ������Ƶ�����(�����������������롣
//
// �����г�������������ΪUS����Զ���룬�������������ݷ�����������Ƿ�������롣
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CProductFinnhub::AddInaccessibleExchangeIfNeeded(void) {
	if (IsUSMarket()) return false; // �����г���Զ�����ѯ

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