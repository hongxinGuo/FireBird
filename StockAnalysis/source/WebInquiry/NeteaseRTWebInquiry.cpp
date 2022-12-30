/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// ����ʵʱ����ȱ�ٹؼ��Եĳɽ����һ��ʶ��޷���Ϊ�������ݣ�ֻ����Ϊ�����á�(turnover���ǳɽ�����ȱ�ˣ���
//
// ����ʵʱ���ݷ������ķ���Ƶ��Ϊÿ�������һ�Σ�������ʵʱ���ݵĸ����ٶȣ�ÿ����һ�Σ�Ҫ��һ�����ʶ���Ϊ��һ��ѡ��
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ChinaStockCodeConverter.h"
#include"ChinaMarket.h"

#include "NeteaseRTWebInquiry.h"
#include"NeteaseRTDataSource.h"

CNeteaseRTWebInquiry::CNeteaseRTWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("NeteaseRT");
	m_fReportStatus = false;
	m_lInquiringNumber = 900; // ����ʵʱ���ݲ�ѯĬ��ֵ

	ConfigureSession();
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
bool CNeteaseRTWebInquiry::ParseData(CWebDataPtr pWebData) {
	return false;
}

bool CNeteaseRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d������ʵʱ����\n", lNumberOfData);
	return true;
}

bool CNeteaseRTWebInquiry::PrepareNextInquiringString(void) {
	// ������һ���ι�Ʊʵʱ����
	const CString strMiddle = GetNextInquiringMiddleString(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock()); // Ŀǰ����ʹ��ȫ����Ʊ��
	const CString strNeteaseStockCode = strMiddle.Left(7); //ֻ��ȡ��һ����Ʊ����.���״����ʽΪ��0600000��100001�����߸��ַ�
	gl_systemMessage.SetStockCodeForInquiringRTData(XferNeteaseToStandard(strNeteaseStockCode));
	CreateTotalInquiringString(strMiddle);

	return true;
}

CString CNeteaseRTWebInquiry::GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetNeteaseStockInquiringMiddleStr(lTotalNumber, fUsingTotalStockSet);
}

/// <summary>
/// ����ʵʱ���ݵĽ��գ���ʱ���ִ������ݡ�Ŀǰ�޷�ȷ���Ƿ���������Ĵ����ǽ����г��ֵĴ����������������ͷ��ʹ��ˣ����޷������ˡ�
/// �޸�session�������������ܷ������⡣
/// </summary>
/// <param name=""></param>
void CNeteaseRTWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000); // �������ӳ�ʱʱ��Ϊ4000���롣 �������������ʵʱ���ݽ���ʱ�䲻����200���롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 1000); // ���ý��ճ�ʱʱ��Ϊ4000����
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 200); // ���÷��ͳ�ʱʱ��Ϊ500����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2); // ����һ�Ρ�
}
