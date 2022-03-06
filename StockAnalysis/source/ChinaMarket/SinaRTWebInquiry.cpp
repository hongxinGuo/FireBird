#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "SinaRTWebInquiry.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
	// 2022��1��20�պ�����ʵʱ���ݷ�������Ҫ��ӱ�ͷ��֤���ݣ���ʽΪReferer:https://finance.sina.com.cn
	// User - Agent����ֻ����˵����ʽ,����ͷ����\r\n��CRLF)����
	m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");

	m_strWebDataInquirePrefix = _T("https://hq.sinajs.cn/list="); // ����ʵʱ���ݷ�������ʹ��https��ʽ
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("SinaRT");
	m_lShortestInquiringInterval = 400; // ����ʵʱ����ÿ3�����������һ�Σ��ʶ�ÿ400�����ѯһ�μ�����3���������ѯһ�ε�����
	m_lInquiringNumber = 850; // ����ʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::TransferData(CWebDataPtr pWebData) {
	auto byteReaded = GetByteReaded();
	if (m_lContentLength > 0) {
		if (m_lContentLength != byteReaded) gl_systemMessage.PushErrorMessage(_T("�������ݳ��Ȳ�����") + m_strInquire.Left(120));
	}
	m_sBuffer.resize(byteReaded);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // ʹ��std::move�Լ���ִ���ٶ�
	m_sBuffer.resize(1024 * 1024); // ���·����ڴ�
	pWebData->SetBufferLength(byteReaded);
	pWebData->ResetCurrentPos();
	return true;
}

bool CSinaRTWebInquiry::ProcessData(CWebDataPtr pWebData) {
	INT64 llTotal = 0;
	bool fSucceed = true;
	pWebData->ResetCurrentPos();
	while (!pWebData->IsProcessedAllTheData()) {
		if (gl_fExitingSystem) return fSucceed;
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		if (pRTData->ReadSinaData(pWebData)) {
			llTotal++;
			gl_WebRTDataContainer.PushSinaData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
		}
		else {
			fSucceed = false;
			gl_systemMessage.PushErrorMessage(_T("����ʵʱ���ݽ�������ʧ����Ϣ"));
			break;  // ��������ݳ����⣬�׵����á�
		}
	}
	gl_pChinaMarket->IncreaseRTDataReceived(llTotal);

	return fSucceed;
}

bool CSinaRTWebInquiry::PrepareNextInquiringStr(void) {
	CString strMiddle = _T("");
	CString strSinaStockCode;

	// ������һ���ι�Ʊʵʱ����
	// �������Ѱ�ҽ��ջ�Ծ��Ʊ�ڼ䣨9:10--9:29, 11:31--12:59),��ʹ��ȫ�ֹ�Ʊ��
	// ����ʱʹ�ý��ջ�Ծ��Ʊ��
	strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock());
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
CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetSinaStockInquiringStr(lTotalNumber, fUsingTotalStockSet);
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d������ʵʱ����\n", lNumberOfData);
	return true;
}