#include"pch.h"

#include"NeteaseDayLineDataSource.h"
#include"ProductNeteaseDayLine.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

CNeteaseDayLineDataSource::CNeteaseDayLineDataSource() {
	m_strInquiryFunction = _T("http://quotes.money.163.com/service/chddata.html?code=");
	m_strSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("NeteaseDayLine");

	ConfigureSession();

	Reset();
}

bool CNeteaseDayLineDataSource::Reset(void) {
	return true;
}

bool CNeteaseDayLineDataSource::UpdateStatus(void) {
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
// 抓取日线数据.开始于11:45:01
//
// 由于可能会抓取全部5000个左右日线数据，所需时间超过10分钟，故而9:15:00第一次重置系统时不去更新，而在9:25:00第二次重置系统后才开始。
// 为了防止与重启系统发生冲突，实际执行时间延后至11:45:01,且不是下载实时数据的工作时间
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineDataSource::Inquire(const long lCurrentTime) {
	if (gl_systemStatus.IsWebBusy()) return false; // 网络出现问题时，不申请网易日线数据。
	if (gl_pChinaMarket->IsSystemReady() && gl_pChinaMarket->IsDayLineNeedUpdate() && gl_pChinaMarket->IsDummyTime()
		&& (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			const auto product = make_shared<CProductNeteaseDayLine>();
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineDataSource::PrepareNextInquiringString(void) {
	// 准备网易日线数据申请格式
	CString strMiddle = gl_pChinaMarket->CreateNeteaseDayLineInquiringStr();
	if (strMiddle.GetLength() > 0) {
		char buffer2[200];
		const CString strStockCode = XferNeteaseToStandard(strMiddle);
		SetDownLoadingStockCode(strStockCode);
		gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(strStockCode);
		strMiddle += _T("&start=19900101&end=");
		sprintf_s(buffer2, _T("%8d"), gl_pChinaMarket->GetMarketDate());
		strMiddle += buffer2;
		CreateTotalInquiringString(strMiddle);
		gl_pChinaMarket->CheckValidOfNeteaseDayLineInquiringStr(strMiddle);

		return true;
	}
	return false;
}

void CNeteaseDayLineDataSource::CreateTotalInquiringString(CString strMiddle) {
	m_strInquiry = m_strInquiryFunction + strMiddle + m_strSuffix + m_strInquiryToken;
}

void CNeteaseDayLineDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 60000); // 正常情况下Netease日线数据接收时间不超过1秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 60000); // 设置接收超时时间为5秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

void CNeteaseDayLineDataSource::UpdateStatusAfterSucceed(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// 这里的strStockCode为标准制式：600000.SS，000001.SZ，
/// </summary>
/// <param name="strStockCode"></param>
void CNeteaseDayLineDataSource::SetDownLoadingStockCode(CString strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}
