#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

bool CProductFinnhub::CheckAccessRight(CWebDataPtr pWebData) {
	const string s(pWebData->GetStringView(0, pWebData->GetBufferLength()));
	if (s == _T("{\"error\":\"You don't have access to this resource.\"}")) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return false;
	}
	if (IsNoRightToAccess()) m_iReceivedDataStatus = GOOD_DATA__;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Ŀǰfinnhub��������˻�ֻ�ṩ�����������ݡ�Ϊ�˼�����ѯ�ٶȣ������Ƶ�����(�����������������롣
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductFinnhub::AddInaccessibleExchange() {
	gl_finnhubInaccessibleExchange.NeedUpdate(true);
	try { // ���ڴ���������
		const auto pExchange = gl_finnhubInaccessibleExchange.GetExchange(m_iInquireType);
		if (!pExchange->HaveExchange(m_strInquiringExchange)) {	// �µĽ��������룿
			pExchange->AddExchange(m_strInquiringExchange);
		}
	} catch (out_of_range&) {	// �����ڴ��������ͣ���洢�������ͺ͸ý���������
		const auto pNewExchange = make_shared<CInaccessibleExchanges>();
		pNewExchange->SetFunction(m_iInquireType);
		pNewExchange->SetFunctionString(gl_FinnhubInquiryType.GetInquiryString(m_iInquireType));
		pNewExchange->AddExchange(m_strInquiringExchange);
		gl_finnhubInaccessibleExchange.SetExchange(m_iInquireType, pNewExchange);
	}
}
