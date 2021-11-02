#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "TiingoWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

//https://api.tiingo.com/api/
//m_strWebDataInquireSuffix = _T("&token=859bd66ca24b2a81a2b5f4de6616e2c408b2a769"); // 密钥放在最后
//m_strWebDataInquireSuffix = _T("&token=c897a00b7cfc2adffc630d23befd5316a4683156"); // 密钥放在最后
//m_strWebDataInquireSuffix = _T("&token=fad87279362b9e580e4fb364a263cda3c67336c8"); // hxguo111@hotmail.com
CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T(""); // Tiingo有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。

	if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
		m_strWebDataInquireSuffix = _T("&token=fad87279362b9e580e4fb364a263cda3c67336c8"); // 调试版使用hxguo111@hotmail.com账户
#else
		m_strWebDataInquireSuffix = _T("&token=fad87279362b9e580e4fb364a263cda3c67336c8"); // 发行版使用hxguo111@hotmail.com账户
#endif // DEBUG
	}
	m_strConnectionName = _T("Tiingo");
	m_lInquiringNumber = 1; // Tiingo实时数据查询数量默认值
}

CTiingoWebInquiry::~CTiingoWebInquiry() {
}

bool CTiingoWebInquiry::PrepareNextInquiringStr(void) {
	CWebRTDataPtr pRTData = nullptr;

	CString strMiddle = _T("");

	// 申请下一批次股票实时数据。 此网络数据提取器使用TiingoMarket
	// 由于Tiingo提供各种数据，而每个数据分别设计提取器会导致出现太多的提取器，故而在此分类。

	// 1 准备前缀字符串
	// 2. 准备中间字符串
	// 3. 准备后缀字符串
	//
	CreateTotalInquiringString(m_strWebDataInquireMiddle);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CTiingoWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
	// do nothing

	return _T("");
}

bool CTiingoWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个Tiingo数据\n", lNumberOfData);
	return true;
}

void CTiingoWebInquiry::PrepareReadingWebData(void) {
	ASSERT(!gl_pWorldMarket->IsTiingoDataReceived());
}

void CTiingoWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_WebInquirer.GetTiingoDataSize() > 0) gl_WebInquirer.PopTiingoData();
	gl_pWorldMarket->SetTiingoInquiring(false); // 当工作线程出现故障时，需要清除Tiingo数据申请标志。
	gl_systemMessage.PushErrorMessage(_T("Tiingo工作线程出错"));
}

void CTiingoWebInquiry::UpdateAfterReadingWebData(void) {
	gl_pWorldMarket->SetTiingoDataReceived(true); // 接收完网络数据后，清除状态。
}

void CTiingoWebInquiry::StoreWebData(CWebDataPtr pWebDataReceived) {
	gl_WebInquirer.PushTiingoData(pWebDataReceived);
}