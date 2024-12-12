#include"pch.h"
module;
module FireBird.WebProduct.SinaRT;

import FireBird.Market.China;
import FireBird.DataSource.SinaRT;
import FireBird.Accessory.ChinaStockCodeConverter;
import FireBird.System.Message;
import FireBird.Accessory.JsonParse;

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
	ParseSinaRTData(pWebData); // ʹ��thread pool + coroutineЭ�̲��н������ٶȱȵ��߳�ģʽ��һ�����ϡ�
}
