#include"pch.h"

#include"ProductNeteaseDayline.h"

#include"JsonParse.h"

IMPLEMENT_DYNCREATE(CProductNeteaseDayline, CVirtualProductWebData)

CProductNeteaseDayline::CProductNeteaseDayline() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://quotes.money.163.com/service/chddata.html?code=");
}

CString CProductNeteaseDayline::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // 新浪实时数据的申请字符串由CNeteaseDaylineWebInquiry类完成，本Product无需动作。
}

bool CProductNeteaseDayline::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseNeteaseDayLine(pWebData);
	return true;
}

bool CProductNeteaseDayline::ParseNeteaseDayline(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}