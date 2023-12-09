#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

#include"JsonParse.h"

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
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode.data()));
	return m_strInquiryFunction + strStocks;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// todo ����ʵʱ�����Ǳ�ϵͳ�����ʱ�����񣬿��ǲ��ù����̵߳�ģʽ��д֮�������������ö��CPU�������ĸ�����������ִ�С�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseSinaRTData(pWebData);

	gl_ProcessChinaMarketRTData.acquire();
	for (const auto& pRTData : *pvWebRTData) {
		GetMarket()->PushSinaRT(pRTData);// ����ʵʱ����ָ�����ʵʱ���ݶ���
	}
	gl_ProcessChinaMarketRTData.release();

	return true;
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
