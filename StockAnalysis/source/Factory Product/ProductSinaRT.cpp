#include"pch.h"

#include"ProductSinaRT.h"

#include"WebInquirer.h"

IMPLEMENT_DYNCREATE(CProductSinaRT, CVirtualProductWebData)

CProductSinaRT::CProductSinaRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("https://hq.sinajs.cn/list=");
}

CString CProductSinaRT::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // ����ʵʱ���ݵ������ַ�����CSinaRTWebInquiry����ɣ���Product���趯����
}

bool CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	gl_WebInquirer.PushSinaRTData(pWebData); // Ŀǰֻ�ǽ����ݴ洢���ݴ�����С�������ݵĽ���������������˴���
	return true;
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}