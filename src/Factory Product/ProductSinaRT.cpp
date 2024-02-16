#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

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
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode));
	return m_strInquiryFunction + strStocks;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// ʹ�ù����߳�ģʽ��д��ʵ��ִ��ʱ��ȴ�䳤�ˣ���ʮ�����ϣ����߳��л�ʱ��̫����Ե�ʡ������ù����߳�ģʽ��
// ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ�������ɼ��߳��л�����̫��ʱ�䡣
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseSinaRTData(pWebData);
	//ParseSinaRTDataUsingWorkingThread(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ�������ɼ��߳��л�����̫��ʱ��
}
