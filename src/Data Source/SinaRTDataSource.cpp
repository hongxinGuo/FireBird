#include"pch.h"

#include"SinaRTDataSource.h"
#include"ProductSinaRT.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

/// <summary>
/// ����ʵʱ���ݷ�����Ҫ���ṩ��ͷ��֤���ݡ�
///
/// </summary>
CSinaRTDataSource::CSinaRTDataSource() {
	// 2022��1��20�պ�����ʵʱ���ݷ�������Ҫ��ӱ�ͷ��֤���ݣ���ʽΪ�� Referer:https://finance.sina.com.cn
	// User-Agent����ֻ����˵����ʽ,����ͷ����\r\n��CRLF)����
	//m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");
	m_strHeaders = _T("Referer:https://finance.sina.com.cn\r\n");

	m_strInquiryFunction = _T("https://hq.sinajs.cn/list="); // ����ʵʱ���ݷ�������ʹ��https��ʽ
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 850; // ����ʵʱ���ݲ�ѯ����Ĭ��ֵ

	CSinaRTDataSource::ConfigureSession();

	CSinaRTDataSource::Reset();
}

bool CSinaRTDataSource::Reset(void) {
	return true;
}

bool CSinaRTDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount64();
	if (static long long sllLastTimeTickCount = 0; llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime())) {
		// ���ж��´ε�����ʱ�䡣���������ֻ��˳�������������ݺ󷽲����á�
		if (IsWebError()) {
			sllLastTimeTickCount = llTickCount + 10000; //������ִ���ʱ���ӳ�ʮ���ٲ�ѯ
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // ϵͳ����Ϊ����ϵͳʱ����������ѯ�ٶ�
				sllLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
			}
			else {
				//m_pWebInquiry->SetCurrentInquiryTime(llTickCount - sllLastTimeTickCount);
				sllLastTimeTickCount = llTickCount;
			}
		}
		// ��������������
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
	}
	return true;
}

bool CSinaRTDataSource::InquireRTData(const long) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductSinaRT>();
		const CString strMessage = _T("https://hq.sinajs.cn/list=");
		// ������һ���ι�Ʊʵʱ����
		// �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
		// ����ʱʹ�ý��ջ�Ծ��Ʊ��
		const CString strStocks = gl_pChinaMarket->GetSinaStockInquiringStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock());
		const CString strSinaStockCode = strStocks.Left(8); // ֻ��ȡ��һ����Ʊ���롣���˴����ʽΪ��sh000001�����˸��ַ���
		gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode));
		product->SetInquiryFunction(strMessage + strStocks);
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������ʵʱ������ȡ������
// Ŀǰֻ��ȡǰ12000����Ʊ��ʵʱ���ݡ�
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSinaRTDataSource::CreateInquiryMessageFromCurrentProduct(void) {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

/// <summary>
/// ����ʵʱ���ݽ���ʱ����ʱ����ֽ���ʧ�ܵ�������о�ԭ������������ݷ��������ʹ��󣬵���Ҫ�����۲졣
/// ����Session��ѡ����ܻ���ƽ��������
/// </summary>
/// <param name=""></param>
void CSinaRTDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 3000); // ���������sinaʵʱ���ݽ���ʱ�䲻����50���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 3000); // ���ý��ճ�ʱʱ��Ϊ4000����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // ���÷��ͳ�ʱʱ��Ϊ500����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}
