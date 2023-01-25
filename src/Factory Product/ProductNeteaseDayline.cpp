#include"pch.h"

#include"ProductNeteaseDayLine.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

CProductNeteaseDayLine::CProductNeteaseDayLine() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://quotes.money.163.com/service/chddata.html?code=");
}

CString CProductNeteaseDayLine::CreateMessage(void) {
	return m_strInquiry; // 新浪实时数据的申请字符串由CNeteaseDayLineWebInquiry类完成，本Product无需动作。
}

bool CProductNeteaseDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pDayLineWebData = ParseNeteaseDayLine(pWebData);

	gl_pChinaMarket->PushDayLine(pDayLineWebData);

	return true;
}
