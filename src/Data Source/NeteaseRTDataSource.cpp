#include"pch.h"

#include"NeteaseRTDataSource.h"
#include"ProductNeteaseRT.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

CNeteaseRTDataSource::CNeteaseRTDataSource() {
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("NeteaseRT");
	m_fReportStatus = false;
	m_lInquiringNumber = 900; // ����ʵʱ���ݲ�ѯĬ��ֵ

	ConfigureSession();

	Reset();
}

bool CNeteaseRTDataSource::Reset(void) {
	return true;
}

bool CNeteaseRTDataSource::UpdateStatus(void) { return true; }

bool CNeteaseRTDataSource::Inquire(const long lCurrentTime) {
	const ULONGLONG llTickCount = GetTickCount64();
	if (static ULONGLONG sllLastTimeTickCount = 0; llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime())) {
		// ���ж��´ε�����ʱ�䡣���������ֻ��˳�������������ݺ󷽲����á�
		if (IsWebError()) {
			sllLastTimeTickCount = llTickCount + 10000; //������ִ���ʱ���ӳ�ʮ���ٲ�ѯ
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady() && !gl_systemConfiguration.IsDebugMode()) { // ϵͳ����Ϊ������ϵͳʱ����������ѯ�ٶ�
				sllLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
			}
			else {
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

bool CNeteaseRTDataSource::InquireRTData(const long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductNeteaseRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
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

bool CNeteaseRTDataSource::ReportStatus(long lNumberOfData) const {
	TRACE("����%d������ʵʱ����\n", lNumberOfData);
	return true;
}

bool CNeteaseRTDataSource::PrepareNextInquiringString(void) {
	// ������һ���ι�Ʊʵʱ����
	const CString strMiddle = GetNextInquiringMiddleString(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock()); // Ŀǰ����ʹ��ȫ����Ʊ��
	const CString strNeteaseStockCode = strMiddle.Left(7); //ֻ��ȡ��һ����Ʊ����.���״����ʽΪ��0600000��100001�����߸��ַ�
	gl_systemMessage.SetStockCodeForInquiringRTData(XferNeteaseToStandard(strNeteaseStockCode));
	CreateTotalInquiringString(strMiddle);

	return true;
}

CString CNeteaseRTDataSource::GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetNeteaseStockInquiringMiddleStr(lTotalNumber, fUsingTotalStockSet);
}

/// <summary>
/// ����ʵʱ���ݵĽ��գ���ʱ���ִ������ݡ�Ŀǰ�޷�ȷ���Ƿ���������Ĵ����ǽ����г��ֵĴ����������������ͷ��ʹ��ˣ����޷������ˡ�
/// �޸�session�������������ܷ������⡣
/// </summary>
void CNeteaseRTDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 3000); // �������ӳ�ʱʱ��Ϊ4000���롣 �������������ʵʱ���ݽ���ʱ�䲻����200���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 3000); // ���ý��ճ�ʱʱ��Ϊ4000����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 200); // ���÷��ͳ�ʱʱ��Ϊ500����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // ����һ�Ρ�
}
