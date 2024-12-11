#include"pch.h"
module;
#include"ConcurrentQueueHeader.h"
module FireBird.WebProduct.NeteaseDayLine;

import FireBird.System.Data;
import FireBird.Accessory.JsonParse;

CProductNeteaseDayLine::CProductNeteaseDayLine() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("http://quotes.money.163.com/service/chddata.html?code=");
}

CString CProductNeteaseDayLine::CreateMessage() {
	return m_strInquiryFunction; // ����ʵʱ���ݵ������ַ�����CNeteaseDayLineWebInquiry����ɣ���Product���趯����
}

void CProductNeteaseDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pDayLineWebData = ParseNeteaseDayLine(pWebData);

	gl_qDayLine.enqueue(pDayLineWebData);
}
