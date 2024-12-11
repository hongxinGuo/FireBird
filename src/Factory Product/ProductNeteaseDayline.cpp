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
	return m_strInquiryFunction; // 新浪实时数据的申请字符串由CNeteaseDayLineWebInquiry类完成，本Product无需动作。
}

void CProductNeteaseDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pDayLineWebData = ParseNeteaseDayLine(pWebData);

	gl_qDayLine.enqueue(pDayLineWebData);
}
