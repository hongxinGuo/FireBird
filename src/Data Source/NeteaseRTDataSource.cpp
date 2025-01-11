#include"pch.h"

#include"NeteaseRTDataSource.h"
#include"ProductNeteaseRT.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

CNeteaseRTDataSource::CNeteaseRTDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 900; // ����ʵʱ���ݲ�ѯĬ��ֵ

	CNeteaseRTDataSource::ConfigureInternetOption();
	CNeteaseRTDataSource::Reset();
}

bool CNeteaseRTDataSource::Reset() {
	return true;
}

bool CNeteaseRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const auto llTickCount = GetTickCount();
	if (llTickCount > m_PrevTimePoint + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()) {
		// ���ж��´�����ʱ�䡣�����������ʱ����֮�������´����롣
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // ϵͳ����Ϊ������ϵͳʱ����������ѯ�ٶ�
			m_PrevTimePoint = llTickCount + 60000ms; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
		}
		else {
			if (!IsInquiring()) m_PrevTimePoint = llTickCount; // ֻ�е���һ����������󷽵�����ʱ���㣬���������һ�����볬ʱ�����󣬱�֤���������һ�����롣
		}
		// ��������������
		if (!IsInquiring()) {
			ASSERT(!HaveInquiry());
			Inquire(lCurrentTime);
			return true;
		}
	}
	return false;
}

void CNeteaseRTDataSource::Inquire(const long lCurrentTime) {
	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		const CString strMessage = _T("http://api.money.126.net/data/feed/");
		const CString strStocks = gl_pChinaMarket->GetNeteaseStockInquiringMiddleStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock()); // Ŀǰ����ʹ��ȫ����Ʊ��
		const CString strNeteaseStockCode = strStocks.Left(7); //ֻ��ȡ��һ����Ʊ����.���״����ʽΪ��0600000��100001�����߸��ַ�
		gl_systemMessage.SetStockCodeForInquiringRTData(XferNeteaseToStandard(strNeteaseStockCode));
		const auto product = make_shared<CProductNeteaseRT>();
		product->SetMarket(gl_pChinaMarket);
		product->SetInquiryFunction(strMessage + strStocks);
		StoreInquiry(product);
		SetInquiring(true);
	}
}

void CNeteaseRTDataSource::CreateCurrentInquireString() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// ����ʵʱ���ݵĽ��գ���ʱ���ִ������ݡ�Ŀǰ�޷�ȷ���Ƿ���������Ĵ����ǽ����г��ֵĴ����������������ͷ��ʹ��ˣ����޷������ˡ�
/// �޸�session�������������ܷ������⡣
/// </summary>
void CNeteaseRTDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 2000;
	m_internetOption.option_receive_timeout = 2000;
	m_internetOption.option_data_receive_timeout = 2000;
	m_internetOption.option_send_timeout = 500;
	m_internetOption.option_connect_retries = 1;
}
