#include"pch.h"

#include"StockCodeConverter.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include"SinaRTDataSource.h"

#include "SinaRTWebInquiry.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

/// <summary>
/// ����ʵʱ���ݷ�����Ҫ���ṩ��ͷ��֤���ݡ�
///
/// </summary>
CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
	// 2022��1��20�պ�����ʵʱ���ݷ�������Ҫ��ӱ�ͷ��֤���ݣ���ʽΪ�� Referer:https://finance.sina.com.cn
	// User-Agent����ֻ����˵����ʽ,����ͷ����\r\n��CRLF)����
	m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");

	m_strInquiryFunction = _T("https://hq.sinajs.cn/list="); // ����ʵʱ���ݷ�������ʹ��https��ʽ
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("SinaRT");
	m_lInquiringNumber = 850; // ����ʵʱ���ݲ�ѯ����Ĭ��ֵ

	ConfigerateSession();
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::PrepareNextInquiringString(void) {
	CString strMiddle = _T("");
	CString strSinaStockCode;

	// ������һ���ι�Ʊʵʱ����
	// �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
	// ����ʱʹ�ý��ջ�Ծ��Ʊ��
	strMiddle = GetNextInquiringMiddleString(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock());
	strSinaStockCode = strMiddle.Left(8); // ֻ��ȡ��һ����Ʊ���롣���˴����ʽΪ��sh000001�����˸��ַ���
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandred(strSinaStockCode));
	CreateTotalInquiringString(strMiddle);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������ʵʱ������ȡ������
// Ŀǰֻ��ȡǰ12000����Ʊ��ʵʱ���ݡ�
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSinaRTWebInquiry::GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetSinaStockInquiringStr(lTotalNumber, fUsingTotalStockSet);
}

/// <summary>
/// ����ʵʱ���ݽ���ʱ����ʱ����ֽ���ʧ�ܵ�������о�ԭ������������ݷ��������ʹ��󣬵���Ҫ�����۲졣
/// ����Session��ѡ����ܻ���ƽ��������
/// </summary>
/// <param name=""></param>
void CSinaRTWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 150); // ���������sinaʵʱ���ݽ���ʱ�䲻����50���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 150); // ���ý��ճ�ʱʱ��Ϊ150����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 50); // ���÷��ͳ�ʱʱ��Ϊ50����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d������ʵʱ����\n", lNumberOfData);
	return true;
}

void CSinaRTWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_pSinaRTDataSource->GetReceivedDataSize() > 0) gl_pSinaRTDataSource->GetReceivedData();
	gl_pSinaRTDataSource->SetInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Quandl���������־��
}

/// <summary>
/// Finnhub.io��˳����ȡ�󣬻ᱨ�浱ǰϵͳ״̬����������״̬��Ϊ429ʱ��˵����ȡƵ�ʳ��٣�ϵͳ�ܾ��ش�
/// </summary>
/// <param name=""></param>
void CSinaRTWebInquiry::UpdateStatusAfterReadingWebData(void) {
	gl_pSinaRTDataSource->SetDataReceived(true); // �������������ݺ����״̬��
}

void CSinaRTWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushSinaRTData(pWebDataBeStored);
	gl_pSinaRTDataSource->StoreReceivedData(pWebDataBeStored);
}