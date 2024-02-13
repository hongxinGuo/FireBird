#include"pch.h"

#include"ProductTengxunRT.h"
#include"JsonParse.h"
#include "TengxunRTDataSource.h"

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
}

CString CProductTengxunRT::CreateMessage() {
	// ������һ���ι�Ʊʵʱ���ݡ�
	// ������Ѷʵʱ����ʱ��������������ڵĹ�Ʊ���룬�������᷵��v_pv_none_match="1";������ϵͳ���ϣ�
	// �ʶ�����ֻʹ����Ч��Ʊ���롣
	const CString strStocks = gl_dataContainerChinaStock.GetNextTengxunStockInquiringMiddleStr(gl_pTengxunRTDataSource->GetInquiringNumber()); // ʹ�û�Ծ��Ʊ��
	return m_strInquiryFunction + strStocks;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ʹ�ù����߳�ģʽ��д��ʵ��ִ��ʱ��ȴ�䳤�ˡ�����ʱ�߳��л�ʱ��̫����Ե�ʡ������ù����߳�ģʽ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseTengxunRTData(pWebData);
	//ParseTengxunRTDataUsingWorkingThread(pWebData);
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
