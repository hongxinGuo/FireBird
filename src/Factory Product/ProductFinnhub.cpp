#include"pch.h"

#include"ProductFinnhub.h"

#include"FinnhubInaccessibleExchange.h"
#include "WebData.h"

void CProductFinnhub::CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) {
	for (auto pWebData : *pvWebData) {
		if (pWebData != nullptr) {
			gl_FinnhubTotalData += pWebData->GetBufferLength();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Ŀǰfinnhub��������˻�ֻ�ṩ�����������ݡ�Ϊ�˼�����ѯ�ٶȣ������Ƶ�����(�����������������롣
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CProductFinnhub::AddInaccessibleSymbol() {
	gl_finnhubInaccessibleExchange.SetUpdateDB(true);
	try { // ���ڴ���������
		const auto pExchange = gl_finnhubInaccessibleExchange.GetExchange(m_iInquireType);
		if (!pExchange->HaveSymbol(m_strInquiringExchange)) {	// �µĽ��������룿
			pExchange->AddSymbol(m_strInquiringExchange);
		}
	} catch (out_of_range&) {	// �����ڴ��������ͣ���洢�������ͺ͸ý���������
		const auto pNewExchange = make_shared<CInaccessible>();
		pNewExchange->SetFunction(m_iInquireType);
		pNewExchange->SetFunctionString(gl_FinnhubInquiryType.GetInquiryString(m_iInquireType));
		pNewExchange->AddSymbol(m_strInquiringExchange);
		gl_finnhubInaccessibleExchange.SetExchange(m_iInquireType, pNewExchange);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// Ŀǰ�����ڲ����С�׼��ɾ��
//
////////////////////////////////////////////////////////////////////////////////////////////
bool CProductFinnhub::__Test_checkAccessRight(CWebDataPtr pWebData) {
	const string s(pWebData->GetStringView(0, pWebData->GetBufferLength()));
	if (s == _T("{\"error\":\"You don't have access to this resource.\"}")) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return false;
	}
	if (IsNoRightToAccess()) m_iReceivedDataStatus = GOOD_DATA__;
	return true;
}
