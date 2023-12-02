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

	CNeteaseRTDataSource::ConfigureSession();
	CNeteaseRTDataSource::Reset();
}

bool CNeteaseRTDataSource::Reset() {
	return true;
}

bool CNeteaseRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const ULONGLONG llTickCount = GetTickCount();
	if (llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime())) {
		// ���ж��´ε�����ʱ�䡣���������ֻ��˳�������������ݺ󷽲����á�
		if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // ϵͳ����Ϊ������ϵͳʱ����������ѯ�ٶ�
			m_llLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
		}
		else {
			m_llLastTimeTickCount = llTickCount;
		}
		// ��������������
		if (!IsInquiring()) {
			ASSERT(!HaveInquiry());
			InquireRTData(lCurrentTime);
			return true;
		}
	}
	return false;
}

void CNeteaseRTDataSource::InquireRTData(const long lCurrentTime) {
	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		const auto product = make_shared<CProductNeteaseRT>();
		const CString strMessage = _T("http://api.money.126.net/data/feed/");
		const CString strStocks = gl_pChinaMarket->GetNeteaseStockInquiringMiddleStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock()); // Ŀǰ����ʹ��ȫ����Ʊ��
		const CString strNeteaseStockCode = strStocks.Left(7); //ֻ��ȡ��һ����Ʊ����.���״����ʽΪ��0600000��100001�����߸��ַ�
		gl_systemMessage.SetStockCodeForInquiringRTData(XferNeteaseToStandard(strNeteaseStockCode));
		product->SetInquiryFunction(strMessage + strStocks);
		StoreInquiry(product);
		SetInquiring(true);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ĿǰneteaseRTDataʹ��nlohmann json�⺯��������������ٶȴ�����property tree��������
// ����900���������ݣ�DEBUGģʽ�´���ʱ��Ҫ300-400���룻Releaseģʽ��Ҳ��50���롣
//
// �ڴ˽������ݵĻ���ϵͳ����������ԭ�����������ȡ�����йأ���ȷ��ԭ�������ʶ�������������������DataSource�Ĺ����߳��С�
// ����ʹ��ȫ���Ż�������JsonParse.cpp�ļ������ٶ���DEBUGģʽ��Ҳ�ܹ��ﵽҪ�󣬹ʶ������ر����ˡ�
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseRTDataSource::ParseData(CWebDataPtr pWebData) {
	return false;
}

void CNeteaseRTDataSource::GenerateCurrentInquiryMessage() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// ����ʵʱ���ݵĽ��գ���ʱ���ִ������ݡ�Ŀǰ�޷�ȷ���Ƿ���������Ĵ����ǽ����г��ֵĴ����������������ͷ��ʹ��ˣ����޷������ˡ�
/// �޸�session�������������ܷ������⡣
/// </summary>
void CNeteaseRTDataSource::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 2000); // �������ӳ�ʱʱ��Ϊ2000���롣 �������������ʵʱ���ݽ���ʱ�䲻����200���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 2000); // ���ý��ճ�ʱʱ��Ϊ2000����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // ���÷��ͳ�ʱʱ��Ϊ500����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // ����һ�Ρ�
}
