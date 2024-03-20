#include"pch.h"

#include"VirtualWebProduct.h"
#include"DayLine.h"

[[nodiscard]] bool CompareDayLineDate(const CDayLinePtr& p1, const CDayLinePtr& p2) {
	return p1->GetMarketDate() < p2->GetMarketDate();
}

CVirtualWebProduct::CVirtualWebProduct() {
	m_lIndex = -1; // Ԥ������ΪԽ��

	m_iReceivedDataStatus = 0;
	m_iInquireType = 0;

	m_strInquiryFunction = _T("");
	m_strInquiry = _T("");
	m_strInquiringExchange = _T("ALL"); // Ĭ�����������еĽ��������ݡ�
}

bool CVirtualWebProduct::IsVoidJson(const CWebDataPtr& pWebData) {
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����վ�Ƿ���������������ݣ�������Ļ����������������������
// ���������������������Ļ�����Ҫ����10�β������Ž�������������ӽ�����������
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualWebProduct::CheckInaccessible(const CWebDataPtr& pWebData) {
	static int s_iCounter = 0; // ��������Ϊ����ʱ��ʹ�ô˼��������ж��Ƿ�������������ӽ���������

	CheckAccessRight(pWebData);
	if (!IsNoRightToAccess()) {
		s_iCounter = 0;
		return false;
	}
	if (IsUSMarket()) {
		if (s_iCounter++ < 10) { // �������г���������10����Ȩ��������ʱ�����ٲ�ѯ
			return false;
		}
	}
	AddInaccessibleExchange();
	s_iCounter = 0;

	return true;
}

bool CVirtualWebProduct::IsUSMarket() const {
	if (m_strInquiringExchange.Compare(_T("US")) == 0) return true;
	return false;
}
