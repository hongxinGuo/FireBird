#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

using std::out_of_range;

bool CProductFinnhub::CheckNoRightToAccess(CWebDataPtr pWebData) {
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Ŀǰfinnhub��������˻�ֻ�ṩ�����������ݡ�Ϊ�˼�����ѯ�ٶȣ������Ƶ�����(�����������������롣
//
// �����г�������������ΪUS����Զ���룬�������������ݷ�����������Ƿ�������롣
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductFinnhub::CheckAndAddInaccessibleExchange() {
	if (IsUSMarket()) return; // �����г���Զ�����ѯ

	gl_finnhubInaccessibleExchange.SetUpdate(true);
	try { // ���ڴ���������
		const auto pExchange = gl_finnhubInaccessibleExchange.GetExchange(m_iProductType);
		if (!pExchange->HaveExchange(m_strInquiringExchange)) {	// �µĽ��������룿
			pExchange->AddExchange(m_strInquiringExchange);
		}
	}
	catch (out_of_range&) {	// �����ڴ��������ͣ���洢�������ͺ͸ý���������
		const auto pNewExchange = make_shared<CInaccessibleExchanges>();
		pNewExchange->SetFunction(m_iProductType);
		pNewExchange->SetFunctionString(gl_FinnhubInquiryType.GetInquiryString(m_iProductType));
		pNewExchange->AddExchange(m_strInquiringExchange);
		gl_finnhubInaccessibleExchange.SetExchange(m_iProductType, pNewExchange);
	}
}
