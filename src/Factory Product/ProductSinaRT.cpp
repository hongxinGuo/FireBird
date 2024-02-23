#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "HighPerformanceCounter.h"

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
// ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽҪ��15%��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseSinaRTData(pWebData);
	//ParseSinaRTDataUsingWorkingThread(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��40%��

	/*
	CWebRTDataPtr p;
	CHighPerformanceCounter counter1, counter2;
	ParseSinaRTDataUsingWorkingThread(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��40%��
	int total2 = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total2++;
	int i = total2;
	ParseSinaRTData(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��40%��
	int total = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total++;
	i = total;
	*/
}
