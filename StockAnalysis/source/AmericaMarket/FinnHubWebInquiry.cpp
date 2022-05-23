#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T(""); // finnhub有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("Finnhub");
	m_lInquiringNumber = 1; // Finnhub实时数据查询数量默认值
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {
}

bool CFinnhubWebInquiry::PrepareNextInquiringStr(void) {
	CWebRTDataPtr pRTData = nullptr;

	CString strMiddle = _T("");

	// 申请下一批次股票实时数据。 此网络数据提取器使用FinnhubMarket
	// 由于Finnhub提供各种数据，而每个数据分别设计提取器会导致出现太多的提取器，故而在此分类。

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
CString CFinnhubWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
	// do nothing
	return _T("");
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个Finnhub数据\n", lNumberOfData);
	return true;
}

void CFinnhubWebInquiry::PrepareReadingWebData(void) {
	ASSERT(!gl_pWorldMarket->IsFinnhubDataReceived());
}

void CFinnhubWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_WebInquirer.FinnhubDataSize() > 0) gl_WebInquirer.PopFinnhubData();
	gl_pWorldMarket->SetFinnhubInquiring(false); // 当工作线程出现故障时，需要清除Quandl数据申请标志。
}

void CFinnhubWebInquiry::UpdateAfterReadingWebData(void) {
	gl_pWorldMarket->SetFinnhubDataReceived(true); // 接收完网络数据后，清除状态。
}

void CFinnhubWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushFinnhubData(pWebDataBeStored);
}