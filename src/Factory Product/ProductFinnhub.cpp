#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"

using std::out_of_range;

bool CProductFinnhub::CheckAccessRight(CWebDataPtr pWebData) {
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Ŀǰfinnhub��������˻�ֻ�ṩ�����������ݡ�Ϊ�˼�����ѯ�ٶȣ������Ƶ�����(�����������������롣
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductFinnhub::AddInaccessibleExchange() {
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

bool CProductFinnhub::IsValidData(const CWebDataPtr& pWebData) {
	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return false;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return false;
	}
	return true;
}
