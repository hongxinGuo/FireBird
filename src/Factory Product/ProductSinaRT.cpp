#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "HighPerformanceCounter.h"

#include <simdjson.h>

#include"JsonParse.h"
#include "SinaRTDataSource.h"

CProductSinaRT::CProductSinaRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("https://hq.sinajs.cn/list=");
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ������һ���ι�Ʊʵʱ����
// �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
// ����ʱʹ�ý��ջ�Ծ��Ʊ��
//
/////////////////////////////////////////////////////////////////////////////////////
CString CProductSinaRT::CreateMessage() {
	const CString strStocks = GetMarket()->GetSinaStockInquiringStr(gl_pSinaRTDataSource->GetInquiringNumber(), GetMarket()->IsCheckingActiveStock());
	const string_view strSinaStockCode = string_view(strStocks, 8); // ֻ��ȡ��һ����Ʊ���롣���˴����ʽΪ��sh000001�����˸��ַ���
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode).c_str());
	return m_strInquiryFunction + strStocks;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽҪ��һ�����ϡ�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	//ParseSinaRTData(pWebData);
	ParseSinaRTDataUsingWorkingThread(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��һ�����ϡ�

	/*
	CWebRTDataPtr p;
	CHighPerformanceCounter counter1, counter2;
	counter1.start();
	ParseSinaRTDataUsingWorkingThread(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��40%��
	counter1.stop();
	int total2 = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total2++;
	counter2.start();
	ParseSinaRTData(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��40%��
	counter2.stop();
	int total = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total++;
	ASSERT(total2 == total); // Note ���ڼ���ʵʱ���ݵ��̲߳���ִ�У�����ż���᲻��ȶ������жϴ��󡣺���֮��
	double f = static_cast<double>(counter1.GetElapseTick()) / counter2.GetElapseTick();
	double f2 = f;
	*/
}
