#include"pch.h"
#include"globedef.h"
#include"ChinaMarket.h"

#include"Thread.h"
#include "TengxunRTWebInquiry.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CTengxunRTWebInquiry::CTengxunRTWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T("http://qt.gtimg.cn/q=");
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("TengxunRT");
	m_lShortestInquiringInterval = 400;
	m_lInquiringNumber = 900; // ��Ѷʵʱ���ݲ�ѯĬ��ֵ
}

CTengxunRTWebInquiry::~CTengxunRTWebInquiry() {
}

bool CTengxunRTWebInquiry::TransferData(CWebDataPtr pWebData) {
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

bool CTengxunRTWebInquiry::ProcessData(CWebDataPtr pWebData) {
	bool fSucceed = true;

	pWebData->ResetCurrentPos();
	if (!IsTengxunRTDataInvalid(*pWebData)) { // ������21���ַ����ĺ������Է������Ҳ���Է�����ǰ�档
		while (!pWebData->IsProcessedAllTheData()) {
			if (gl_fExitingSystem) return fSucceed;
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
			if (pRTData->ReadTengxunData(pWebData)) {
				gl_WebRTDataContainer.PushTengxunData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
			}
			else {
				fSucceed = false;
				break;// ��������ݳ����⣬�׵����á�
			}
		}
	}
	return true;
}

bool CTengxunRTWebInquiry::PrepareNextInquiringStr(void) {
	CString strMiddle = _T("");
	ASSERT(gl_pChinaMarket->IsSystemReady());
	// ������һ���ι�Ʊʵʱ���ݡ�
	// ������Ѷʵʱ����ʱ��������������ڵĹ�Ʊ���룬�������᷵��v_pv_none_match="1";������ϵͳ���ϣ�
	// �ʶ�����ֻʹ����Ч��Ʊ���롣
	strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber); // ʹ�û�Ծ��Ʊ��
	CreateTotalInquiringString(strMiddle);
	return true;
}

CString CTengxunRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	ASSERT(gl_pChinaMarket->IsSystemReady());
	return gl_pChinaMarket->GetNextTengxunStockInquiringMiddleStr(lTotalNumber);
}

bool CTengxunRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d����Ѷʵʱ����\n", lNumberOfData);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����б���ѯ�Ĺ�Ʊ��Ϊ�����й�Ʊʱ����Ѷʵʱ��Ʊ�������᷵��һ��21���ַ������ַ�����v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTWebInquiry::IsTengxunRTDataInvalid(CWebData& WebDataReceived) {
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