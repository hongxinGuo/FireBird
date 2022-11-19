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

	return m_strInquiry; // ����ʵʱ���ݵ������ַ�����CNeteaseDaylineWebInquiry����ɣ���Product���趯����
}

bool CProductNeteaseDayline::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseNeteaseDayLine(pWebData);
	return true;
}

bool CProductNeteaseDayline::ParseNeteaseDayline(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}