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
	m_lInquiringNumber = 900; // 腾讯实时数据查询默认值
}

CTengxunRTWebInquiry::~CTengxunRTWebInquiry() {
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

CString CTengxunRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fCheckActiveStock) {
	return gl_pChinaMarket->GetTengxunInquiringStockStr(lTotalNumber, gl_pChinaMarket->GetTotalStock());
}

bool CTengxunRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个腾讯实时数据\n", lNumberOfData);
	return true;
}

void CTengxunRTWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushTengxunRTData(pWebDataBeStored);
}