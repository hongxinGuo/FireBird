#include"pch.h"

#include"TengxunRTDataSource.h"
#include"ProductTengxunRT.h"

#include"ChinaMarket.h"

CTengxunRTDataSource::CTengxunRTDataSource() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("TengxunRT");
	m_lInquiringNumber = 900; // ��Ѷʵʱ���ݲ�ѯĬ��ֵ

	ConfigureSession();

	Reset();
}

bool CTengxunRTDataSource::Reset(void) {
	return true;
}

bool CTengxunRTDataSource::UpdateStatus(void) {
	return true;
}

bool CTengxunRTDataSource::Inquire(const long lCurrentTime) {
	const long long llTickCount = GetTickCount64();
	static long long sllLastTimeTickCount = 0;

	if (gl_systemStatus.IsWebBusy()) return false; // �����������ʱ����������Ѷʵʱ���ݡ�
	if (gl_pChinaMarket->IsSystemReady() && (llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime() * 5))) {
		if (IsWebError()) {
			sllLastTimeTickCount = llTickCount + 10000; //������ִ���ʱ���ӳ�ʮ���ٲ�ѯ
		}
		else {
			if (!gl_pChinaMarket->IsFastReceivingRTData() && gl_pChinaMarket->IsSystemReady()) {
				sllLastTimeTickCount = llTickCount + 60000; // ��ȫ��ѯһ��󣬷ǽ���ʱ��һ�������Ҹ���һ�μ���
			}
			else {
				sllLastTimeTickCount = llTickCount;
			}
		}
		if (!IsInquiring()) {
			InquireRTData(lCurrentTime);
		}
	}
	return true;
}

bool CTengxunRTDataSource::InquireRTData(const long lCurrentTime) {
	if (!IsInquiring()) {
		const auto product = make_shared<CProductTengxunRT>();
		StoreInquiry(product);
		SetInquiring(true);
		return true;
	}
	return false;
}

bool CTengxunRTDataSource::PrepareNextInquiringString(void) {
	CString strMiddle = _T("");
	ASSERT(gl_pChinaMarket->IsSystemReady());
	// ������һ���ι�Ʊʵʱ���ݡ�
	// ������Ѷʵʱ����ʱ��������������ڵĹ�Ʊ���룬�������᷵��v_pv_none_match="1";������ϵͳ���ϣ�
	// �ʶ�����ֻʹ����Ч��Ʊ���롣
	strMiddle = GetNextInquiringMiddleString(m_lInquiringNumber, false); // ʹ�û�Ծ��Ʊ��
	CreateTotalInquiringString(strMiddle);
	return true;
}

CString CTengxunRTDataSource::GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) {
	ASSERT(gl_pChinaMarket->IsSystemReady());
	return gl_pChinaMarket->GetNextTengxunStockInquiringMiddleStr(lTotalNumber);
}

void CTengxunRTDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 4000); // ���������Tengxunʵʱ���ݽ���ʱ�����Ϊ300���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 4000); // ���ý��ճ�ʱʱ��Ϊ4000����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // ���÷��ͳ�ʱʱ��Ϊ500����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}

bool CTengxunRTDataSource::ReportStatus(long lNumberOfData) const {
	TRACE("����%d����Ѷʵʱ����\n", lNumberOfData);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTDataSource::IsTengxunRTDataInvalid(CWebData& WebDataReceived) {
	char buffer[50]{};
	char* pBuffer = buffer;
	CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // ��Ϊ��Ч��Ʊ��ѯ�������ݸ�ʽ����21���ַ�

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	CString str1 = buffer;

	if (str1.Compare(strInvalidStock) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}
