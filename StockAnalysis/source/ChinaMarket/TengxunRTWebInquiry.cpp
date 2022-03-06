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

void CTengxunRTWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushTengxunRTData(pWebDataBeStored);
}