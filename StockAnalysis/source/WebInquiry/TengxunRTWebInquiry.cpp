#include"pch.h"

#include"ChinaMarket.h"

#include "TengxunRTWebInquiry.h"
#include"TengxunRTDataSource.h"

using namespace std;
#include<thread>

CTengxunRTWebInquiry::CTengxunRTWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("TengxunRT");
	m_lInquiringNumber = 900; // 腾讯实时数据查询默认值

	ConfigerateSession();
}

CTengxunRTWebInquiry::~CTengxunRTWebInquiry() {
}

bool CTengxunRTWebInquiry::PrepareNextInquiringString(void) {
	CString strMiddle = _T("");
	ASSERT(gl_pChinaMarket->IsSystemReady());
	// 申请下一批次股票实时数据。
	// 申请腾讯实时数据时，如果遇到不存在的股票代码，服务器会返回v_pv_none_match="1";，导致系统故障，
	// 故而现在只使用有效股票代码。
	strMiddle = GetNextInquiringMiddleString(m_lInquiringNumber); // 使用活跃股票池
	CreateTotalInquiringString(strMiddle);
	return true;
}

CString CTengxunRTWebInquiry::GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) {
	ASSERT(gl_pChinaMarket->IsSystemReady());
	return gl_pChinaMarket->GetNextTengxunStockInquiringMiddleStr(lTotalNumber);
}

void CTengxunRTWebInquiry::ConfigerateSession(void)
{
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000); // 正常情况下Tengxun实时数据接收时间大致为300毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 1000); // 设置接收超时时间为1000毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 300); // 设置发送超时时间为100毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
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

void CTengxunRTWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_pTengxunRTDataSource->StoreReceivedData(pWebDataBeStored);
}

void CTengxunRTWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_pTengxunRTDataSource->GetReceivedDataSize() > 0) gl_pTengxunRTDataSource->GetReceivedData();
	gl_pTengxunRTDataSource->SetInquiring(false); // 当工作线程出现故障时，需要清除Quandl数据申请标志。
}

void CTengxunRTWebInquiry::UpdateStatusAfterReadingWebData(void) {
	gl_pTengxunRTDataSource->SetDataReceived(true);
}