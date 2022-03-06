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
	m_lInquiringNumber = 900; // 腾讯实时数据查询默认值
}

CTengxunRTWebInquiry::~CTengxunRTWebInquiry() {
}

bool CTengxunRTWebInquiry::TransferData(CWebDataPtr pWebData) {
	auto byteReaded = GetByteReaded();
	if (m_lContentLength > 0) {
		if (m_lContentLength != byteReaded) gl_systemMessage.PushErrorMessage(_T("网络数据长度不符：") + m_strInquire.Left(120));
	}
	m_sBuffer.resize(byteReaded);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // 使用std::move以加速执行速度
	m_sBuffer.resize(1024 * 1024); // 重新分配内存
	pWebData->SetBufferLength(byteReaded);
	pWebData->ResetCurrentPos();
	return true;
}

bool CTengxunRTWebInquiry::ProcessData(CWebDataPtr pWebData) {
	bool fSucceed = true;

	pWebData->ResetCurrentPos();
	if (!IsTengxunRTDataInvalid(*pWebData)) { // 处理这21个字符串的函数可以放在这里，也可以放在最前面。
		while (!pWebData->IsProcessedAllTheData()) {
			if (gl_fExitingSystem) return fSucceed;
			CWebRTDataPtr pRTData = make_shared<CWebRTData>();
			if (pRTData->ReadTengxunData(pWebData)) {
				gl_WebRTDataContainer.PushTengxunData(pRTData); // 将此实时数据指针存入实时数据队列
			}
			else {
				fSucceed = false;
				break;// 后面的数据出问题，抛掉不用。
			}
		}
	}
	return true;
}

bool CTengxunRTWebInquiry::PrepareNextInquiringStr(void) {
	CString strMiddle = _T("");
	ASSERT(gl_pChinaMarket->IsSystemReady());
	// 申请下一批次股票实时数据。
	// 申请腾讯实时数据时，如果遇到不存在的股票代码，服务器会返回v_pv_none_match="1";，导致系统故障，
	// 故而现在只使用有效股票代码。
	strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber); // 使用活跃股票池
	CreateTotalInquiringString(strMiddle);
	return true;
}

CString CTengxunRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	ASSERT(gl_pChinaMarket->IsSystemReady());
	return gl_pChinaMarket->GetNextTengxunStockInquiringMiddleStr(lTotalNumber);
}

bool CTengxunRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个腾讯实时数据\n", lNumberOfData);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTengxunRTWebInquiry::IsTengxunRTDataInvalid(CWebData& WebDataReceived) {
	char buffer[50]{};
	char* pBuffer = buffer;
	CString strInvalidStock = _T("v_pv_none_match=\"1\";\n"); // 此为无效股票查询到的数据格式，共21个字符

	WebDataReceived.GetData(pBuffer, 21, WebDataReceived.GetCurrentPos());
	buffer[21] = 0x000;
	CString str1 = buffer;

	if (str1.Compare(strInvalidStock) == 0) {
		ASSERT(WebDataReceived.GetBufferLength() == 21);
		return true;
	}
	else return false;
}