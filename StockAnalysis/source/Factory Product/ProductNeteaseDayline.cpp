#include"pch.h"

#include"ProductNeteaseDayLine.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

IMPLEMENT_DYNCREATE(CProductNeteaseDayLine, CVirtualProductWebData)

CProductNeteaseDayLine::CProductNeteaseDayLine() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://quotes.money.163.com/service/chddata.html?code=");
}

CString CProductNeteaseDayLine::CreateMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // ����ʵʱ���ݵ������ַ�����CNeteaseDayLineWebInquiry����ɣ���Product���趯����
}

bool CProductNeteaseDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	CNeteaseDayLineWebDataPtr pDayLineWebData = nullptr;

	pDayLineWebData = ParseNeteaseDayLine(pWebData);
	gl_pChinaMarket->PushNeteaseDayLine(pDayLineWebData);

	return true;
}