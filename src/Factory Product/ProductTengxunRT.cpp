#include"pch.h"

#include"ProductTengxunRT.h"

#include <simdjson.h>

#include "HighPerformanceCounter.h"
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
// ʹ�ò��й����߳�ģʽ��д���ٶ�Ϊ����ģʽ��2�����ϡ�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	//ParseTengxunRTData(pWebData);
	ParseTengxunRTDataUsingWorkingThread(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��һ�����ϡ�

	/*
	CWebRTDataPtr p;
	CHighPerformanceCounter counter1, counter2;
	counter1.start();
	ParseTengxunRTDataUsingWorkingThread(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��40%��
	counter1.stop();
	int total2 = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total2++;
	counter2.start();
	ParseTengxunRTData(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��40%��
	counter2.stop();
	int total = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total++;
	ASSERT(total2 == total);
	double f = (double)counter1.GetElapseTick() / counter2.GetElapseTick();
	*/
}
