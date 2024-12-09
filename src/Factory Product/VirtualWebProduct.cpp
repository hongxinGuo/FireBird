#include"pch.h"

#include"VirtualWebProduct.h"
import FireBird.HistoryCandle.DayLine;

[[nodiscard]] bool CompareDayLineDate(const CDayLinePtr& p1, const CDayLinePtr& p2) {
	return p1->GetMarketDate() < p2->GetMarketDate();
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
bool CVirtualWebProduct::CheckInaccessible() {
	static int s_iCounter = 0; // ��������Ϊ����ʱ��ʹ�ô˼��������ж��Ƿ�������������ӽ���������

	if (!IsNoRightToAccess()) {
		s_iCounter = 0;
		return false;
	}
	if (IsUSMarket()) {
		if (s_iCounter++ < 10) { // �������г���������10����Ȩ��������ʱ�����ٲ�ѯ
			return false;
		}
	}
	AddInaccessibleSymbol();
	s_iCounter = 0;

	return true;
}

bool CVirtualWebProduct::IsUSMarket() const {
	if (m_strInquiringExchange.Compare(_T("US")) == 0) return true;
	return false;
}
