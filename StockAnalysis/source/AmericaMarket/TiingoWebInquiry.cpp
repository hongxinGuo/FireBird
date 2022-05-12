#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "TiingoWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
	m_strHeaders = _T("timeout:(5,200)\r\n");
	m_strWebDataInquirePrefix = _T(""); // Tiingo有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strWebDataInquireSuffix = _T("");
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

void CTiingoWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushTiingoData(pWebDataBeStored);
}